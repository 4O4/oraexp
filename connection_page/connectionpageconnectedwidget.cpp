#include "connectionpageconnectedwidget.h"
#include "connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include "info_panel/infopanel.h"
#include "navtree/treepane.h"
#include  "dbuimanager.h"
#include "util/iconutil.h"
#include "util/settingshelper.h"
#include "util/dialoghelper.h"
#include "util/appconnectionmanager.h"
#include "beans/ctrltabdata.h"
#include "connectionpagewindowobject.h"
#include <iostream>
#include <QtGui>

using namespace std;

QByteArray ConnectionPageConnectedWidget::currentState;

ConnectionPageConnectedWidget::ConnectionPageConnectedWidget(DbConnection *db, DbUiManager *uiManager, QWidget *parent) :
    QMainWindow(parent), db(db), uiManager(uiManager), busyCounter(0)
{
    setDockNestingEnabled(true);

    treeDock=new QDockWidget(tr("Database objects"), this);
    treeDock->setObjectName("TreeViewDock");
    treeDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);
    treeDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);

    treePane=new TreePane(uiManager, treeDock);
    childObjects.append(treePane);
    treePane->setDeleteListener(this);
    connect(treePane, SIGNAL(busyStateChanged(ConnectionPageObject*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPageObject*,bool)));
    treePane->setConnection(db);
    AppConnectionManager::registerConnection(uiManager->getConnectionPage(), treePane, db);

    treeDock->setWidget(treePane);
    addDockWidget(Qt::LeftDockWidgetArea, treeDock);

    centralTab=new ConnectionPageTabWidget();
    centralTab->setTabsClosable(true);
    centralTab->setDocumentMode(true);
    centralTab->setMovable(true);

    setCentralWidget(centralTab);

    connect(treeDock, SIGNAL(visibilityChanged(bool)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(&connectionPool, SIGNAL(asyncConnectionReady(DbConnection*,void*,bool,OciException)),
            this, SLOT(asyncConnectionReady(DbConnection*,void*,bool,OciException)));
    connect(&connectionPool, SIGNAL(working(bool)), this, SLOT(connectionPoolWorking(bool)));

    QTimer::singleShot(0, this, SLOT(restoreWindowState()));
}

ConnectionPageConnectedWidget::~ConnectionPageConnectedWidget()
{
    delete centralTab; //to ensure child tabs are not using connection
    AppConnectionManager::deleteConnection(db);

    //register backup connection if there is one so that it can be properly unregistered and disconnected
    DbConnection *backupConnection = connectionPool.getBackupConnection();
    if(backupConnection){
        AppConnectionManager::registerConnection(uiManager->getConnectionPage(), 0, backupConnection);
    }
}

/*
DbUiManager *ConnectionPageConnectedWidget::getUiManager()
{
    return &uiManager;
}*/

void ConnectionPageConnectedWidget::beforeClose()
{
    //call beforClose method of each child and then delete

    int childCount = childObjects.size();
    while(childCount-- > 0){
        ConnectionPageObject *obj = childObjects.at(0);
        obj->beforeClose();

        if(!obj->isWindow()){
            ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);
            Q_ASSERT(tab);
            int ix = indexOf(tab);
            if(ix != -1){ //not tree pane
                centralTab->removeTab(ix);
            }
        }

        delete obj;
    }

    childObjects.clear();
}

void ConnectionPageConnectedWidget::closeTab(int index)
{
    QWidget *widgetToDelete=centralTab->widget(index);
    ConnectionPageTab *tabPage=dynamic_cast<ConnectionPageTab *>(widgetToDelete);

    if(tabPage==0){
        QMessageBox::critical(this->window(), tr("Wrong parent"),
                                 tr("Tabs must inherit from ConnectionPageTab"));
        return;
    }

    if(tabPage->isBusy()){
        QMessageBox::information(this->window(), tr("Tab busy"),
                                 tr("Cannot close tab while it is busy."));
        return;
    }

    if(tabPage->isModified()){
        int pressedButton = QMessageBox::question(this->window(), tr("Save changes?"),
                                                  tr("Do you want to save changes?"),
                                                  QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard,
                                                  QMessageBox::Save);
        if(pressedButton==QMessageBox::Cancel){
            return;
        }else if(pressedButton==QMessageBox::Save && !tabPage->saveAll()){
            return;
        }
    }

    tabPage->beforeClose();
    centralTab->removeTab(index);
    if(widgetToDelete!=0){
        widgetToDelete->deleteLater();
    }
}

void ConnectionPageConnectedWidget::closeTab(QWidget *widget)
{
    int tabIx=centralTab->indexOf(widget);
    closeTab(tabIx);
}

void ConnectionPageConnectedWidget::prepareObject(ConnectionPageObject *obj)
{
    childObjects.append(obj);
    obj->setDeleteListener(this);

    QObject *objRef = dynamic_cast<QObject*>(obj);
    Q_ASSERT(objRef);
    connect(objRef, SIGNAL(busyStateChanged(ConnectionPageObject*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPageObject*,bool)));

    if(obj->needsSeparateConnection()){
        connectionPool.requestConnection(this->db, obj);
    }else{
        //tab->setUpdatesEnabled(false);
        obj->setConnection(db);
        //tab->setUpdatesEnabled(true);
    }
}

void ConnectionPageConnectedWidget::addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title)
{
    tab->setTitle(title);
    tab->setEnabled(false);
    connect(tab, SIGNAL(initCompleted(ConnectionPageObject*)), this, SLOT(tabInitializationCompleted(ConnectionPageObject*)));

    tab->createUi();

    int newTabIx=centralTab->insertTab(centralTab->currentIndex()+1, tab, icon, title);
    //int newTabIx=centralTab->addTab(tab, icon, title);

    centralTab->setCurrentIndex(newTabIx);
    //centralTab->setTabBusy(newTabIx, true);
    connect(tab, SIGNAL(stateChanged()), this, SIGNAL(connectionPageStateChanged()));
    connect(tab, SIGNAL(captionChanged(ConnectionPageTab*,QString)), this, SLOT(changeTabCaption(ConnectionPageTab*,QString)));

    prepareObject(tab);
}

void ConnectionPageConnectedWidget::addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title)
{
    window->createUi();

    QWidget *widget = dynamic_cast<QWidget*>(window);
    Q_ASSERT(widget);

    widget->setWindowIcon(icon.isNull() ? IconUtil::getIcon("database") : icon);
    widget->setWindowTitle(title);

    ConnectionPageWindowObject *windowObject = static_cast<ConnectionPageWindowObject*>(window);

    if(windowObject->initiallyVisible()){
        windowObject->showAndActivate();
    }

    prepareObject(window);
}

void ConnectionPageConnectedWidget::beforeDelete(ConnectionPageObject *obj)
{
    Q_ASSERT(obj);
    Q_ASSERT(childObjects.contains(obj));

    childObjects.removeOne(obj);

    Q_ASSERT(childObjects.size() >= 0);
}

void ConnectionPageConnectedWidget::asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex)
{
    if(error){
        delete db;
        QMessageBox::critical(this->window(), tr("Error while connecting to database"), ex.getErrorMessage());
        //tab->decreaseRefCount();
    }else{
        ConnectionPageObject *obj = (ConnectionPageObject*)data;
        QWidget *widget = dynamic_cast<QWidget*>(obj);

        Q_ASSERT(widget);

        widget->setUpdatesEnabled(false);
        obj->setConnection(db);
        AppConnectionManager::registerConnection(uiManager->getConnectionPage(), obj, db);
        widget->setUpdatesEnabled(true);
    }
}

void ConnectionPageConnectedWidget::tabBusyStateChanged(ConnectionPageObject *obj, bool busy)
{
    ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);
    if(tab && tab!=treePane){
        centralTab->setTabBusy(tab, busy);
    }

    if(busy){
        ++busyCounter;
    }else{
        --busyCounter;
        Q_ASSERT(busyCounter >= 0);
    }

    if(busyCounter ==0 || busyCounter == 1){ //just got free or just got busy
        emit busyStateChanged(busy);
    }
}

void ConnectionPageConnectedWidget::tabInitializationCompleted(ConnectionPageObject *obj)
{
    ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);

    if(tab){
        tab->setEnabled(true);
        if(centralTab->currentWidget()==tab){
            tab->focusAvailable();
        }
    }
}

ConnectionPageTab *ConnectionPageConnectedWidget::currentConnectionPageTab() const
{
    ConnectionPageTab *cnPageTab=static_cast<ConnectionPageTab*>(centralTab->currentWidget());
    return cnPageTab;
}

int ConnectionPageConnectedWidget::indexOf(const ConnectionPageTab *tab) const
{
    for(int i=0; i<centralTab->count(); ++i){
        if(centralTab->widget(i) == tab){
            return i;
        }
    }

    return -1;
}

ConnectionPageTab *ConnectionPageConnectedWidget::tabAt(int index) const
{
    Q_ASSERT(index>=0 && index<centralTab->count());

    return static_cast<ConnectionPageTab*>(centralTab->widget(index));
}

QIcon ConnectionPageConnectedWidget::tabIcon(int index) const
{
    return centralTab->tabIcon(index);
}

QString ConnectionPageConnectedWidget::tabText(int index) const
{
    return centralTab->tabText(index);
}

bool ConnectionPageConnectedWidget::activateChildWidget(ConnectionPageObject *obj)
{
    ConnectionPageTab *cnPageTab = dynamic_cast<ConnectionPageTab*>(obj);

    if(!cnPageTab){
        return false;
    }

    if(cnPageTab == treePane){
        toggleTreePane();

        return true;
    }

    int ix = indexOf(cnPageTab);
    if(ix == -1){
        return false;
    }

    setCurrentIndex(ix);

    return true;
}

void ConnectionPageConnectedWidget::toggleTreePane()
{
    treeDock->setVisible(!treeDock->isVisible());
}

void ConnectionPageConnectedWidget::windowStateChanged()
{
    ConnectionPageConnectedWidget::currentState=saveState();
}

void ConnectionPageConnectedWidget::restoreWindowState()
{
    restoreState(ConnectionPageConnectedWidget::currentState);

    connectDockSignals(treeDock);

    //also add an empty worksheet
    //setting focus works correctly when adding from this slot
    uiManager->addWorksheet();
}

void ConnectionPageConnectedWidget::changeTabCaption(ConnectionPageTab *tab, const QString &caption)
{
    centralTab->setTabText(centralTab->indexOf(tab), caption);
}

void ConnectionPageConnectedWidget::connectionPoolWorking(bool isWorking)
{
    tabBusyStateChanged(0, isWorking);
}

void ConnectionPageConnectedWidget::connectDockSignals(QDockWidget *dockWidget)
{
    connect(dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(windowStateChanged()));
    connect(dockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(windowStateChanged()));
}

bool ConnectionPageConnectedWidget::isTreePaneVisible() const
{
    return treeDock->isVisible();
}

QList<ConnectionPageTab *> ConnectionPageConnectedWidget::getTabsByType(const QString &className) const
{
    QList<ConnectionPageTab *> results;

    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);
        if(cnPageTab->metaObject()->className()==className){
            results.append(cnPageTab);
        }

    }

    return results;
}

QList<ConnectionPageTab *> ConnectionPageConnectedWidget::getTabsByConnection(DbConnection *db, const QString &className, int limit)
{
    QList<ConnectionPageTab *> results;

    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);
        if(cnPageTab->getDb()==db && (className.isEmpty() || cnPageTab->metaObject()->className()==className)){
            results.append(cnPageTab);
        }

        if(limit!=-1 && results.size()==limit){
            break;
        }
    }

    return results;
}

QList<CtrlTabData *> ConnectionPageConnectedWidget::getCtrlTabData() const
{
    QList<CtrlTabData *> results;

    QList<QWidget*> history = centralTab->getTabChangeHistory();
    foreach(QWidget *tab, history){
        int tabIndex = centralTab->indexOf(tab);
        results.append(new CtrlTabData(centralTab->tabIcon(tabIndex),
                                       centralTab->tabText(tabIndex),
                                       tab));
    }

    return results;
}

void ConnectionPageConnectedWidget::setCurrentTab(QWidget *widget)
{
    centralTab->setCurrentWidget(widget);
}

void ConnectionPageConnectedWidget::setCurrentIndex(int ix)
{
    centralTab->setCurrentIndex(ix);
}

ConnectionPageTab *ConnectionPageConnectedWidget::findTabById(const QString &tabId) const
{
    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);

        if(cnPageTab->getTabId() == tabId){
            return cnPageTab;
        }
    }

    return 0;
}

const ConnectionPool *ConnectionPageConnectedWidget::getConnectionPool() const
{
    return &this->connectionPool;
}
