#include "dbobjectddlviewer.h"
#include "widgets/multieditorwidget.h"
#include "connectivity/dbconnection.h"
#include "util/queryexectask.h"
#include "util/queryutil.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/widgethelper.h"
#include "dialogs/tableddloptionsdialog.h"
#include "widgets/codestructurecombobox.h"
#include <QtGui>

DbObjectDdlViewer::DbObjectDdlViewer(bool addSettingsButton, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab("", uiManager, parent),
    addSettingsButton(addSettingsButton)
{
    ddlOptions.sqlTerminators="y";
    ddlOptions.segmentAttributes="n";
    ddlOptions.storage="n";
    ddlOptions.tablespace="n";
    ddlOptions.constraints="y";
    ddlOptions.constraintsAsAlter="n";
    ddlOptions.refConstraints="y";
    ddlOptions.oid="n";
    ddlOptions.sizeByteKeyword="n";
}

void DbObjectDdlViewer::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectViewerGenericTab::setQueryScheduler(queryScheduler);

    multiEditor->setQueryScheduler(queryScheduler);
}

void DbObjectDdlViewer::createMainWidget(QLayout *layout)
{
    multiEditor=new MultiEditorWidget(uiManager, DbUtil::isPLSQLProgramUnit(itemType));
    multiEditor->setReadOnly(true);

    layout->addWidget(multiEditor);

    connect(multiEditor, SIGNAL(switchToPair()), this, SIGNAL(switchToPair()));
}

void DbObjectDdlViewer::loadData()
{
    multiEditor->currentTextEditor()->clear();

    QList<Param*> params;
    if(itemType==DbTreeModel::Table){
        queryName="get_object_ddl";
        params.append(ddlOptions.getQueryParams());
    }else{
        queryName="get_object_source_code";
        params = QList<Param*>() <<
                                    new Param("target_owner", this->schemaName) <<
                                    new Param("wrap", 0) <<
                                    new Param("sql_terminator", 1);
    }
    params.append(new Param(":object_type", dbObjectTypeName.isEmpty() ?
                                                DbUtil::getDbObjectTypeNameByNodeType(this->itemType)
                                                :
                                                dbObjectTypeName.replace(' ', '_')));
    params.append(getQueryParams());

    queryScheduler->enqueueQuery(queryName,
                                 params,
                                 this,
                                 "get_object_ddl",
                                 "ddlQueryCompleted",
                                 "ddlFetched",
                                 "fetchCompleted",
                                 true);
}

void DbObjectDdlViewer::ddlQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        if(result.exception.getErrorCode()==31603){
            multiEditor->setInitialText(tr("--Object does not exist or you do not have permission to view it."));
        }else{
            QMessageBox::critical(this->window(), tr("Error retrieving table DDL"), result.exception.getErrorMessage());
        }

        queryCompleted();
    }
}

void DbObjectDdlViewer::ddlFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table DDL"), fetchResult.exception.getErrorMessage());
    }else{
        multiEditor->setInitialText(fetchResult.oneRow.at(0));
    }
}

void DbObjectDdlViewer::fetchCompleted(const QString &)
{
    queryCompleted();
}

QList<QAction*> DbObjectDdlViewer::getSpecificToolbarButtons(QToolBar *toolbar)
{
    QList<QAction*> list;

    if(addSettingsButton){
        list.append(WidgetHelper::createSeparatorAction(this));

        QAction *settingsAction=new QAction(IconUtil::getIcon("settings"), tr("DDL options"), this);
        connect(settingsAction, SIGNAL(triggered()), this, SLOT(showDdlOptions()));
        list.append(settingsAction);
    }

    if(DbUtil::isSpecOrBodyType(this->itemType)){
        list.append(WidgetHelper::createSeparatorAction(this));

        CodeStructureComboBox *codeStructureComboBox = new CodeStructureComboBox();
        list.append(toolbar->addWidget(codeStructureComboBox));

        connect(multiEditor, SIGNAL(codeStructureModelAvailable(CodeStructureModel*)), codeStructureComboBox, SLOT(codeStructureModelAvailable(CodeStructureModel*)));
        connect(codeStructureComboBox, SIGNAL(selectionChanged(int,int)), multiEditor, SLOT(selectRegion(int,int)));
    }

    list.append(WidgetHelper::addStretchToToolbar(toolbar));

    list.append(multiEditor->addSplittingActions(toolbar));

    return list;
}

void DbObjectDdlViewer::setDbObjectTypeName(const QString &dbObjectTypeName)
{
    this->dbObjectTypeName = dbObjectTypeName;
}

void DbObjectDdlViewer::setPairEditorWidget(MultiEditorWidget *multiEditor)
{
    this->multiEditor->setPairEditorWidget(multiEditor);
}

MultiEditorWidget *DbObjectDdlViewer::getEditorWidget() const
{
    return this->multiEditor;
}

void DbObjectDdlViewer::showDdlOptions()
{
    TableDdlOptionsDialog dialog(this);
    dialog.setOptions(ddlOptions);
    if(dialog.exec()){
        ddlOptions=dialog.getOptions();
        refreshInfo();
    }
}
