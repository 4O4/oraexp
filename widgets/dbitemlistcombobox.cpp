#include "dbitemlistcombobox.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/queryutil.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "interfaces/iqueryscheduler.h"
#include "defines.h"
#include <QtGui>

DbItemListComboBox::DbItemListComboBox(const QString &initialValue,
                                       const QString &iconName,
                                       bool setMaxWidth,
                                       bool prependEmptyValue,
                                       QWidget *parent) :
    QComboBox(parent), iconName(iconName), prependEmptyValue(prependEmptyValue), iconColumn(-1)
{
    setEditable(true);
    if(setMaxWidth){
        lineEdit()->setMaxLength(MAX_IDENTIFIER_LENGTH);
    }

    addItem(IconUtil::getIcon(iconName), initialValue);
    setCurrentIndex(0);
}

void DbItemListComboBox::loadItems(IQueryScheduler *queryScheduler, const QString &queryName, QList<Param*> params, const QString &dbLinkName)
{
    QueryExecTask task;
    task.query=QueryUtil::getQuery(queryName, queryScheduler->getDb());
    task.requester=this;
    task.params=params;
    task.taskName=queryName;
    task.queryCompletedSlotName="queryCompleted";
    task.fetchSlotName="itemFetched";
    task.fetchCompletedSlotName="fetchCompleted";
    task.dbLinkName=dbLinkName;
    queryScheduler->enqueueQuery(task);
}

void DbItemListComboBox::setIconColumn(int colNum)
{
    this->iconColumn=colNum;
}

void DbItemListComboBox::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error loading item list. Task name: %1").arg(result.taskName), result.exception.getErrorMessage());

        emit loadingCompelted();

        return;
    }
    if(prependEmptyValue){
        addItem(IconUtil::getIcon(iconName), "");
    }
}

void DbItemListComboBox::itemFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving item list. Task name: %1").arg(fetchResult.taskName), fetchResult.exception.getErrorMessage());
        return;
    }

    QString iconNameToAdd = iconColumn!=-1 ? fetchResult.oneRow.at(iconColumn) : iconName;

    addItem(IconUtil::getIcon(iconNameToAdd), fetchResult.oneRow.at(0));
}

void DbItemListComboBox::fetchCompleted(const QString &)
{
    bool hasSelection=lineEdit()->hasSelectedText();
    QString currentText=lineEdit()->text();
    removeItem(0);
    WidgetHelper::setComboBoxText(this, currentText);
    if(hasSelection){
        lineEdit()->selectAll();
    }

    emit loadingCompelted();
}
