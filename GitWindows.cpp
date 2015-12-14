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

#include <string>
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "GitWindows.h"

GitWindows::GitWindows(GitRepository repo):repo(repo){};

void GitWindows::launch()
{
    std::cout<<repo.getPath()<<std::endl;
    QLabel* label= new QLabel("Bonjour");
    label->show();
    qApp->exec();
}
