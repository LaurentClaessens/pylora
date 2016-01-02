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

#ifndef  __GIT_WINDOWS_H__
#define  __GIT_WINDOWS_H__

#include <QDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "GitRepository.h"
#include "HashTable.h"
#include "Configuration.h"


class UntrackedLine;

QString modified_text(GitRepository rep);

class GitWindow : public QDialog
{
    Q_OBJECT

    friend class AddIgnoreLayout;
    friend class UntrackedLine;

    private slots :
        void launch_git_diff();
        void launch_git_commit();
        void launch_edit_gitignore();
        void apply_add_ignore_changes();
        void open_terminal();
    private:
        // 0 : no action. 1 : add, 2 : gitignore
        HashTable<path,UntrackedLine*> add_ignore_status;

        // GitRepository is not 'const' because the status of the repository
        // is going to change (even if the logical object is probably const)
        GitRepository repo;
        QHBoxLayout* untracked_line(path file); 
        QString modified_text();
        const Configuration* config_ptr;
        void addFormatButton(string,QLayout*);
    public:
        GitWindow(const GitRepository repo,const Configuration*,QWidget* parent=0);
};

class AddIgnoreLayout : public QVBoxLayout
{
    Q_OBJECT

    private:
        QHBoxLayout* untracked_line(path);
        GitWindow* parent;
    public:
        AddIgnoreLayout(GitWindow*);
};

class UntrackedLine : public QHBoxLayout
{
    Q_OBJECT

    private : 
        path file;
        string printed_path;   
        GitWindow* parent;
        QCheckBox*  box_add;
        QCheckBox*  box_ignore;
        QCheckBox*  box_noaction;
        QLabel* label_filename;
    public:
        UntrackedLine(path,GitWindow*);
        int getStatus();       // 0 : no action. 1 : add, 2 : gitignore

        // if 'file' is a directory, we want to work with "file/*"
        string getActualPath();
        void setEnabled(bool);
};

// A button that proposes to add files in .gitignore.
// The list of added files is based on the passed format (vim,latex,c++,etc.)
// See GitRepository::append_format_to_gitignore
class FormatButton : public QPushButton
{
    Q_OBJECT

    private slots:
        void add_to_gitignore();
    private :
        GitRepository repo;
        const string format;
    public:
        FormatButton(GitRepository,string);
};

// A small bar that allows to write a quick comment and launch 'git commit -a'
class QuickCommitLayout : public QHBoxLayout
{
    Q_OBJECT

    private:
        GitRepository repo;
        QLineEdit* edit_line=new QLineEdit();
    private slots:
        void do_commit();
    public:
        QuickCommitLayout(GitRepository);
};

#endif   // __GIT_WINDOWS_H__
