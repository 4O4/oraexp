#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectionspane.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "util/settingshelper.h"
#include "connection_page/connectionpage.h"
#include "app_menu/appmenu.h"
#include "app_menu/appfilemenu.h"
#include <QtGui>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/database.png"));

    connectionsPane=new ConnectionsPane();
    this->setCentralWidget(connectionsPane);

    createMenu();

    DbConnection::initEnvironment();

    setWindowIcon(IconUtil::getIcon("database"));
    setWindowTitle("Oracle Explorer");

    SettingsHelper::restoreWindowPosition(this, "MainWindow");
    SettingsHelper::loadStaticApplicationSettings();

    //connectionsPane->installEventFilter(this);

    connect(connectionsPane, SIGNAL(canClose()), this, SLOT(canClose()));

    showConnectDialog();
}

MainWindow::~MainWindow()
{   
    AppMenu::cleanUp();

    delete ui;

    delete connectionsPane; //to ensure that child DbConnection destructors will be called before destroyEnvironment
    DbConnection::destroyEnvironment();
}

void MainWindow::createMenu()
{
    AppMenu::defaultInstance()->setupMenu(this, this->connectionsPane);
}

void MainWindow::showConnectDialog()
{
    this->connectionsPane->addConnection();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->setAccepted(false);

    connectionsPane->closeAll();
}

void MainWindow::canClose()
{
    SettingsHelper::saveWindowPosition(this, "MainWindow");
    SettingsHelper::saveStaticApplicationSettings();
    AppMenu::defaultInstance()->getFileMenu()->saveRecentFileList();

    qApp->quit();
}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==connectionsPane && ev->type()==QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
        if(keyEvent->key()==Qt::Key_Tab && (keyEvent->modifiers() & Qt::ControlModifier)==Qt::ControlModifier){
            qDebug("Ctrl+Tab pressed");
            ev->setAccepted(true);
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, ev);
}*/
