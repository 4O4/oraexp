#include "connectionpage.h"
#include "connectionpageconnectwidget.h"
#include "connectionpageconnectedwidget.h"
#include "beans/dbconnectioninfo.h"
#include "util/iconutil.h"
#include <QtGui>

ConnectionPage::ConnectionPage(const QString &connectionUuid, QWidget *parent) :
    QWidget(parent),
    mainWidget(0),
    uiManager(this),
    busy(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    connectWidget = new ConnectionPageConnectWidget(connectionUuid);
    mainLayout->addWidget(connectWidget);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(connectWidget, SIGNAL(busyStateChanged(bool)), this, SLOT(childBusyStateChanged(bool)));
    connect(connectWidget, SIGNAL(connected(DbConnection*,DbConnectionInfo*)), this, SLOT(connected(DbConnection*,DbConnectionInfo*)));
}

ConnectionPage::~ConnectionPage()
{

}

void ConnectionPage::focusReady()
{
    Q_ASSERT(connectWidget);

    connectWidget->focusReady();
}

bool ConnectionPage::isConnected() const
{
    return (mainWidget != 0);
}

DbUiManager *ConnectionPage::getUiManager()
{
    return &this->uiManager;
}

void ConnectionPage::addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title)
{
    Q_ASSERT(mainWidget);

    mainWidget->addTab(tab, icon, title);
}

void ConnectionPage::addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title)
{
    Q_ASSERT(mainWidget);

    mainWidget->addWindow(window, icon, title);
}

void ConnectionPage::beforeClose()
{
    if(!mainWidget){ //not connected yet
        return;
    }

    mainWidget->beforeClose();
}

void ConnectionPage::closeTab(QWidget *widget)
{
    Q_ASSERT(mainWidget);

    mainWidget->closeTab(widget);
}

ConnectionPageTab *ConnectionPage::currentConnectionPageTab() const
{
    if(mainWidget){
        return mainWidget->currentConnectionPageTab();
    }else{
        return 0;
    }
}

int ConnectionPage::tabCount() const
{
    if(mainWidget){
        return mainWidget->tabCount();
    }else{
        return 0;
    }
}

int ConnectionPage::indexOf(const ConnectionPageTab *tab) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->indexOf(tab);
}

ConnectionPageTab *ConnectionPage::tabAt(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabAt(index);
}

QIcon ConnectionPage::tabIcon(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabIcon(index);
}

QString ConnectionPage::tabText(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabText(index);
}

bool ConnectionPage::activateChildWidget(ConnectionPageObject *obj)
{
    Q_ASSERT(mainWidget);

    return mainWidget->activateChildWidget(obj);
}

bool ConnectionPage::isDbTreePaneVisible() const
{
    Q_ASSERT(mainWidget);

    return mainWidget->isDbTreePaneVisible();
}

bool ConnectionPage::isCodeStructurePaneVisible() const
{
    Q_ASSERT(mainWidget);

    return mainWidget->isCodeStructurePaneVisible();
}

QList<CtrlTabData *> ConnectionPage::getCtrlTabData() const
{
    if(mainWidget){
        return mainWidget->getCtrlTabData();
    }else{
        return QList<CtrlTabData*>();
    }
}

void ConnectionPage::setCurrentTab(QWidget *widget)
{
    Q_ASSERT(mainWidget);

    mainWidget->setCurrentTab(widget);
}

void ConnectionPage::setCurrentIndex(int ix)
{
    Q_ASSERT(mainWidget);

    mainWidget->setCurrentIndex(ix);
}

ConnectionPageTab *ConnectionPage::findTabById(const QString &tabId) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->findTabById(tabId);
}

QList<ConnectionPageTab *> ConnectionPage::getTabsByType(const QString &className) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->getTabsByType(className);
}

QList<ConnectionPageTab *> ConnectionPage::getTabsByConnection(DbConnection *db, const QString &className, int limit)
{
    Q_ASSERT(mainWidget);

    return mainWidget->getTabsByConnection(db, className, limit);
}

CodeStructurePane *ConnectionPage::getCodeStructurePane() const
{
    Q_ASSERT(mainWidget);

    return mainWidget->getCodeStructurePane();
}

const ConnectionPool *ConnectionPage::getConnectionPool() const
{
    Q_ASSERT(mainWidget);

    return mainWidget->getConnectionPool();
}

OraExp::ConnectionEnvironment ConnectionPage::getConnectionEnvironment() const
{
    Q_ASSERT(mainWidget);

    return this->currentEnvironment;
}

void ConnectionPage::closeTab(int index)
{
    Q_ASSERT(mainWidget);

    mainWidget->closeTab(index);
}

void ConnectionPage::toggleDbTreePane()
{
    Q_ASSERT(mainWidget);

    mainWidget->toggleDbTreePane();
}

void ConnectionPage::toggleCodeStructurePane()
{
    Q_ASSERT(mainWidget);

    mainWidget->toggleCodeStructurePane();
}

void ConnectionPage::changeTabCaption(ConnectionPageTab *tab, const QString &caption)
{
    Q_ASSERT(mainWidget);

    mainWidget->changeTabCaption(tab, caption);
}

void ConnectionPage::connected(DbConnection *db, DbConnectionInfo *connectionInfo)
{
    setUpdatesEnabled(false);

    mainWidget = new ConnectionPageConnectedWidget(db, &uiManager);
    uiManager.setConnection(db);
    currentEnvironment = connectionInfo->environment;

    connectWidget->deleteLater();

    layout()->addWidget(mainWidget);

    connect(mainWidget, SIGNAL(connectionPageStateChanged()), this, SIGNAL(connectionPageStateChanged()));
    connect(mainWidget, SIGNAL(busyStateChanged(bool)), this, SLOT(childBusyStateChanged(bool)));

    setUpdatesEnabled(true);

    emit setTitle(this, connectionInfo);

    emit connectionPageStateChanged();
}

void ConnectionPage::childBusyStateChanged(bool busy)
{
    this->busy = busy;

    emit busyStateChanged(this, busy);
}
