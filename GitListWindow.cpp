#include <iostream>
#include <string>
#include <QtGui>

#include "GitListWindow.h"
#include "GitWindow.h"


// GIT BUTTON

GitButton::GitButton(GitRepository r,GitListWindow* glw) : 
    QPushButton(QString::fromStdString(r.getPathName())), 
    repo(r),
    parent(glw)
    {}

void GitButton::launchGitWindow()
{
    std::cout<<"Clicked for git window in "<<repo.getPathName()<<std::endl;
    parent->launchGitWindow(repo);
}

// GIT LIST WINDOW
void GitListWindow::launchGitWindow(GitRepository repo)
{
    GitWindow* git_window = new GitWindow(repo,config_ptr,this);
    git_window->show();
    git_window->activateWindow();
    qApp->processEvents();
}

void GitListWindow::addGitButton(GitRepository repo)
{
    GitButton* button=new GitButton(repo,this);

    connect(button,SIGNAL(clicked()),button,SLOT(launchGitWindow()));
    button->show();
    main_layout->addWidget(button);
    qApp->processEvents();
}

GitListWindow::GitListWindow(const Configuration* conf):
    QMainWindow(),
    config_ptr(conf)
{
    main_layout=new QVBoxLayout;
    QWidget* button_widget=new QWidget(this);
    QScrollArea* scroll_area=new QScrollArea;
    QPushButton* exit_button=new QPushButton("exit");

    connect( exit_button,SIGNAL(clicked()),this,SLOT(close())  );

    main_layout->addWidget(exit_button);
    button_widget->setLayout(main_layout);

    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(button_widget);
    
    setCentralWidget(scroll_area);
}
