/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This program is free software: you can redistribute it and/or modify
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


#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <boost/filesystem.hpp>             // the file filesystem.hpp is itself protected by #ifndef... #endif thus this is not included twice (by #include "newbaka.h")
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "newbaka.h"
#include "tasks.h"
#include "mainbackuploop.h"

using namespace boost::filesystem;
using namespace std;

// The line 'exclude=foo/bar' exclude the directory $HOME/foo/bar
// if you want to exclude a repertory that is not in $HOME, you have to write the full path in the configuration file.
MainBackupLoop read_configuration_file(const path cfg_path)
{
    assert(is_regular_file(cfg_path));
    ifstream cfg_file(cfg_path.c_str());
    string line;
    path bp,pp,sp;
    const path home_path=path(getenv("HOME"));
    vector<string> parts;
    vector<path> exclude;
    while (  std::getline(cfg_file,line) )
    {
        split( parts,line,boost::is_any_of("=") );
        switch (parts.size())
        {
         case 0 : 
             throw string("Malformed configuration file : "+line); 
             break;
         case 1 :       // probably empty line
             break;
         case 2 :
            path one=path(parts[1]);
            if (parts[0]=="starting")  { sp=one; }
            else if (parts[0]=="backup") { bp=one; }
            else if (parts[0]=="purge") { pp=one; }
            else if (parts[0]=="exclude") 
            {   
                if (one.is_absolute()){ exclude.push_back(one) ;}
                else { 
                    exclude.push_back(  home_path/one  ) ;
                    }
            }
            else { throw string("Unknown entry in the configuration file :"+parts[0]); }
            break;
        }
    }
    assert(is_directory(bp));
    assert(is_directory(pp));
    assert(is_directory(sp));

    MainBackupLoop config=MainBackupLoop(sp,bp,pp);
    config.add_exclude_path(exclude);

    return config;
}

path get_starting_path(int argc, char *argv[])
{
    path starting_path;
    path full_path;
    if (argc != 2)
    {
        throw string("You have to pass a repertory name or path.");
    }
    else
    {
        starting_path=path(argv[1]);
        if (starting_path.is_relative()){
            full_path=absolute(starting_path);
        }
        else {
            full_path=starting_path;
        }
    }
    full_path=canonical(full_path);
    cout<<"We are going to backup the repertory "<<full_path<<endl;
    return full_path;
}

bool run_next(deque<GenericTask*> &task_list){
    bool ret;
    ret=task_list.front()->run();       // equivalent to   (*task_list.front()).run()
    task_list.pop_front();
    cout<<task_list.size()<<" tasks remaining"<<endl;
    return ret;
}

void make_the_work(  deque<GenericTask*> &task_list)
{
    bool still=true;
    while (still)
    {
        if (task_list.size() != 0)
        {
            try{
             still=run_next(task_list);
               }
            catch (string err) { cout<<string("**** I got a bad news : ")<<err<<endl; }
        }
    }
}

int main(int argc, char *argv[])
{
try
    {    
    path starting_path=get_starting_path(argc,argv);
    MainBackupLoop a=read_configuration_file("backup.cfg");          // There is the file 'newbaka.cfg' as example.
    a.MakeBackup();
    //launching the thread that runs the tasks
    boost::thread scheduler( make_the_work, a.task_list );
    scheduler.join();
    }
catch (string err) { cout<<string("I got a bad news : ")<<err<<endl; }
}
