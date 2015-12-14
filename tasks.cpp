/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This is part of 'lora': you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*/
//
//

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include "tasks.h"

// Create the tree up to the directory to which 'rep_path' belongs to.
// create_directory_tree("/home/foo/bar/lol.txt") creates /home/foo/bar
void create_directory_tree(const path rep_path)
{
    const path parent_path=rep_path.parent_path();
    if (is_directory( parent_path ) ) {}
    else { create_directory_tree(parent_path);  } 
    create_directory(rep_path); 
    assert( is_directory(rep_path) );
}
// Create the tree up to the directory containing the given file name.
void create_file_tree(const path file_path)
{
    const path parent_path=file_path.parent_path();
    if (is_directory( parent_path ) ) {}
    else { create_directory_tree(parent_path);  } 
    assert( is_directory(parent_path) );
}

void my_copy_file(path from_path,path to_path)
{
    assert( is_regular_file(from_path) );
    assert( !is_regular_file(to_path) );

    time_t t_ori=last_write_time(from_path);

    std::cout<<"Copy "<<from_path<<" --> "<<to_path;    // no end-line here because the size of the task list will be displayed by the 'run_next' function.
    copy_file(from_path,to_path);
    last_write_time( to_path,t_ori );
    time_t t_done=last_write_time(to_path);

    if( t_ori!=t_done )
    {
        std::cout<<"The last_write_time did not copy well for "+from_path.string() <<std::endl;
        throw std::string("The last_write_time did not copy well for "+from_path.string());
    };
}

GenericTask::GenericTask(){ };
GenericTask::~GenericTask(){ };


FileCopyTask::FileCopyTask(pathTriple triple):GenericTask()
    {
        this->orig_path=triple.orig;
        this->bak_path=triple.bak;
        this->purge_modified_path=triple.purge;
    }

bool FileCopyTask::run() const
{
        assert(is_regular_file(orig_path));

        std::vector<path> test_list;
        test_list.push_back( orig_path );
        test_list.push_back( bak_path );

        if (is_regular_file(bak_path))
        {
            create_directory_tree(purge_modified_path.parent_path());
            rename( bak_path,purge_modified_path );
            assert( is_regular_file(purge_modified_path) );
        }
        my_copy_file(  orig_path,bak_path  );

        assert( is_regular_file(orig_path) );
        assert( is_regular_file(bak_path) );
        return true;
}
FileMoveTask::FileMoveTask(const path orig,const path destination): orig_path(orig), destination_path(destination) {}

bool FileMoveTask::run() const
{
    assert( is_regular_file(orig_path) );
    assert( !is_regular_file(destination_path) );
    std::cout<<"(purge file) "<<orig_path<<" --> "<<destination_path<<std::endl;
    create_file_tree(destination_path);

    rename(orig_path,destination_path);

    assert( !is_regular_file(orig_path) );
    assert( is_regular_file(destination_path) );
    return true;
}
DirectoryMoveTask::DirectoryMoveTask(const path orig,const path destination): orig_path(orig), destination_path(destination) {}

bool DirectoryMoveTask::run() const{
    std::cout<<"(purge rep) "<<orig_path<<" --> "<<destination_path<<std::endl;
    assert( is_directory(orig_path) );
    assert( !is_directory(destination_path) );

    create_directory_tree(destination_path);
    rename(orig_path,destination_path);

    assert( !is_directory(orig_path) );
    assert( is_directory(destination_path) );
    return true;
}


FinalTask::FinalTask(){}
bool FinalTask::run() const { return false; }

void TaskList::push_back(GenericTask* gt) { queue.push_back(gt); }
GenericTask* TaskList::front() { return queue.front(); }
void TaskList::pop_front() {queue.pop_front();}
int TaskList::size() const { return queue.size(); }
