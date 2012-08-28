#include "paircodecreator.h"
#include "util/dbutil.h"
#include <QtGui>


PairCodeCreator::PairCodeCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType objectType,
                                 DbUiManager *uiManager, QWidget *parent) : ConnectionPageTab(uiManager, parent), initializedCount(0)

{
    specCreatorWidget = new CodeCreatorWidget(schemaName, objectName, DbUtil::getSpecType(objectType));
    specCreatorWidget->setHasSpecBodySwitcher(true, true);
    connect(specCreatorWidget, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToBody()));

    bodyCreatorWidget = new CodeCreatorWidget(schemaName, objectName, DbUtil::getBodyType(objectType));
    bodyCreatorWidget->setHasSpecBodySwitcher(true, false);
    connect(bodyCreatorWidget, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToSpec()));

    isSpecType = DbUtil::isSpecType(objectType);
}

void PairCodeCreator::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,2,0,0);

    specCreatorWidget->createUi();
    bodyCreatorWidget->createUi();

    connect(specCreatorWidget, SIGNAL(objectInfoLoaded()), this, SLOT(creatorInitialized()));
    connect(bodyCreatorWidget, SIGNAL(objectInfoLoaded()), this, SLOT(creatorInitialized()));

    tab=new QStackedWidget();
    tab->setContentsMargins(0,0,0,0);
    tab->addWidget(specCreatorWidget);
    tab->addWidget(bodyCreatorWidget);
    tab->setCurrentIndex(isSpecType ? 0 : 1);

    mainLayout->addWidget(tab);
    setLayout(mainLayout);
}

void PairCodeCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    specCreatorWidget->setQueryScheduler(this);
    bodyCreatorWidget->setQueryScheduler(this);
}

void PairCodeCreator::creatorInitialized()
{
    if(++initializedCount==2){
        emitInitCompletedSignal();
    }
}

void PairCodeCreator::switchToSpec()
{
    tab->setCurrentIndex(0);
}

void PairCodeCreator::switchToBody()
{
    tab->setCurrentIndex(1);
}

CodeCreatorWidget *PairCodeCreator::currentCreator() const
{
    return tab->currentIndex()==0 ? specCreatorWidget : bodyCreatorWidget;
}