#include "dbobjecttriggersviewer.h"
#include "util/queryutil.h"
#include "util/queryexectask.h"
#include "codeeditor/codeeditor.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "util/dbutil.h"
#include <QtGui>

DbObjectTriggersViewer::DbObjectTriggersViewer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("get_object_triggers_for_detailed_view", uiManager, parent), sourceRetrieveError(false)
{
    setIconColumn("OWNER", "OWNER_ICON_COLUMN");
    setIconColumn("TRIGGER_NAME", "TR_ICON_COLUMN");
    setObjectListMode(0, 1, -1, "", "TRIGGER");
}

void DbObjectTriggersViewer::createMainWidget(QLayout *layout)
{
    DbObjectViewerGenericTab::createMainWidget(layout);

    layout->removeWidget(dt);

    editor=new CodeEditor();
    editor->setReadOnly(true);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(dt);
    splitter->addWidget(editor);
    layout->addWidget(splitter);

    connect(dt, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged()));
}

void DbObjectTriggersViewer::currentRowChanged()
{
    if(sourceRetrieveError){
        return;
    }

    editor->clear();

    QModelIndex currentIndex=dt->currentIndex();
    if(!currentIndex.isValid()){
        return;
    }

    QueryExecTask task;
    task.query=QueryUtil::getQuery("get_program_source");
    task.params.append(new Param(":owner", dt->model()->index(currentIndex.row(), 0).data().toString()));
    task.params.append(new Param(":name", dt->model()->index(currentIndex.row(), 1).data().toString()));
    task.params.append(new Param(":type", QString("TRIGGER")));

    task.requester=this;
    task.queryCompletedSlotName="sourceFetchQueryCompleted";
    task.fetchSlotName="sourceRecordAvailable";
    task.fetchCompletedSlotName="triggerBodyFetched";
    task.taskName="get_trigger_source";

    queryScheduler->enqueueQuery(task);
}

void DbObjectTriggersViewer::sourceFetchQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        sourceRetrieveError=true;
        QMessageBox::critical(this->window(), tr("Error retrieving trigger source"), result.exception.getErrorMessage());
    }
}

void DbObjectTriggersViewer::sourceRecordAvailable(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching trigger source"), fetchResult.exception.getErrorMessage());
        sourceRetrieveError=true;
        return;
    }

    QString line=fetchResult.oneRow.at(0);
    //if(line.endsWith("\r\n")){
    //    line=line.left(line.size()-2);
    //}
    editor->addText(line);
}

void DbObjectTriggersViewer::triggerBodyFetched(const QString &)
{
    editor->horizontalScrollBar()->setValue(0);
    editor->verticalScrollBar()->setValue(0);
}

void DbObjectTriggersViewer::clearInfo()
{
    editor->clear();

    DbObjectViewerGenericTab::clearInfo();
}

QList<Param *> DbObjectTriggersViewer::getQueryParams()
{
    return DbObjectViewerGenericTab::getQueryParams() <<
               new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(itemType));
}
