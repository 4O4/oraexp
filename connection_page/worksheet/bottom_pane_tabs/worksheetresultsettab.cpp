#include "worksheetresultsettab.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/data_exporter/dataexportdialog.h"
#include "connection_page/data_exporter/dataexporterthread.h"
#include "connection_page/data_exporter/exporters/dataexporterbase.h"
#include "models/resultsettablemodel.h"
#include "interfaces/iqueryscheduler.h"
#include "controllers/datatableeditcontroller.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include <QtGui>

WorksheetResultsetTab::WorksheetResultsetTab(DbUiManager *uiManager, QWidget *parent) :
    WorksheetBottomPaneTab(parent),
    uiManager(uiManager),
    queryScheduler(0),
    exporterThread(0),
    exportDialog(0),
    cursorClosed(false)
{
    QVBoxLayout *layout=new QVBoxLayout();

    resultsTable=new DataTable();
    resultsTable->setAlternatingRowColors(true);
    editController = new DataTableEditController(resultsTable);

    createToolbar();
    statusBarLabel = new QLabel();
    labelAction = toolbar->addWidget(statusBarLabel);
    stopProgressButton = toolbar->addAction(IconUtil::getIcon("stop"),
                                            tr("Stop"),
                                            this,
                                            SLOT(stopProgress()));

    layout->addWidget(toolbar);
    layout->addWidget(resultsTable);

    layout->setContentsMargins(2, 0, 2, 0);
    setLayout(layout);

    connect(resultsTable, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
    connect(editController, SIGNAL(refreshRequired()), this, SLOT(reloadQuery()));
}

WorksheetResultsetTab::~WorksheetResultsetTab()
{
    cleanup();
}

void WorksheetResultsetTab::addTabSpecificToolbarButtons()
{
    dataExportAction = toolbar->addAction(IconUtil::getIcon("export"), tr("Export"), this, SLOT(exportData()));
    dataExportAction->setEnabled(false);

    countRowsAction = toolbar->addAction(IconUtil::getIcon("count"), tr("Count rows"), this, SLOT(countRows()));
    countRowsAction->setEnabled(false);

    toolbar->addSeparator();

    QList<QAction*> editingActions = editController->createEditActions();
    foreach(QAction *action, editingActions){
        toolbar->addAction(action);
    }
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetResultsetTab::getTabType() const
{
    return WorksheetResultPane::ResultsetTab;
}

void WorksheetResultsetTab::showQueryResults(IQueryScheduler *, const QueryResult &)
{
    Q_ASSERT(false);
}

void WorksheetResultsetTab::displayResultset(IQueryScheduler *queryScheduler, Statement *stmt, int rsIx)
{
    cleanup();

    this->queryScheduler = queryScheduler;

    Resultset *rs = stmt->rsAt(rsIx);

    this->lastQuery = stmt->getStatementType()==OraExp::QueryTypeSelect ? stmt->getQuery() : "";
    this->lastParams = Param::cloneParams(stmt->getParams());

    //check if we need to make table editable
    QSharedPointer<ResultsetColumnMetadata> metadata = rs->getColumnMetadata();
    if(!lastQuery.isEmpty() && metadata->getColumnCount()>1 &&
            metadata->columnTitles.at(metadata->columnTitles.size()-1)=="ROWID" //last column is named rowid
            ){
        makeEditable(true);
    }else{
        makeEditable(false);
    }

    countRowsAction->setToolTip(lastQuery.isEmpty() ? tr("Cannot count when resultset is from cursor") : tr("Count rows"));

    cursorClosed = false;
    setInProgress(true);
    resultsTable->setResultset(queryScheduler, rs, QHash<int,StatementDesc*>());
}

void WorksheetResultsetTab::firstFetchCompleted()
{
    setInProgress(false);
}

void WorksheetResultsetTab::exportData()
{
    if(exportDialog==0){
        exportDialog = new DataExportDialog(this);
        exportDialog->exportWidget()->setTableName(this->queryScheduler->getDb()->getSchemaName(), "EXPORT_TABLE");
    }

    int startRow, startColumn, endRow, endColumn;
    resultsTable->getSelectedRange(&startRow, &startColumn, &endRow, &endColumn);
    exportDialog->exportWidget()->setSelectedRange(startRow, startColumn, endRow, endColumn);

    if(exportDialog->exec()){
        DataExporterBase *exporter = exportDialog->exportWidget()->createExporter();
        startExport(exporter);
    }
}

void WorksheetResultsetTab::countRows()
{
    if(queryScheduler->getDb()->isBusy()){
        QMessageBox::information(this->window(), tr("Connection busy"), tr("Connection is busy"));
        return;
    }

    QString queryToRun;
    queryToRun.append("select 'Row count: '||trim(to_char(count(0), '999G999G999G999G999G999G999')) from (").append(this->lastQuery).append(") t_alias");

    uiManager->showRecordCount(queryToRun, Param::cloneParams(this->lastParams));
}

void WorksheetResultsetTab::startExport(DataExporterBase *exporter)
{
    Q_ASSERT(queryScheduler);
    Q_ASSERT(exporterThread==0);

    ResultsetTableModel *tableModel = qobject_cast<ResultsetTableModel*>(resultsTable->model());
    Q_ASSERT(tableModel);

    bool fetchToEnd=false;
    if(!tableModel->isAllDataFetched() && exporter->startRow==-1){
        QMessageBox::StandardButton result = QMessageBox::question(this->window(), tr("Not all records fetched"),
                              tr("Not all records are fetched.\n"
                                 "Press Yes to fetch and export all records.\n"
                                 "Press No to export only fetched records."),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes);
        if(result == QMessageBox::Yes){
            fetchToEnd = true;
            tableModel->setAllDataFetched();
            cursorClosed = true;
            dataExportAction->setToolTip(tr("Export data (Cursor is closed)"));
        }
    }

    exporter->columnMetadata = tableModel->getColumnMetadata();
    exporterThread = new DataExporterThread(exporter,
                                            tableModel->getModelData(),
                                            tableModel->getResultset(),
                                            fetchToEnd, true, this);
    connect(exporterThread, SIGNAL(recordsExported(int)), this, SLOT(recordsExported(int)));
    connect(exporterThread, SIGNAL(exportComplete()), this, SLOT(exportComplete()));
    connect(exporterThread, SIGNAL(exportError(QString)), this, SLOT(exportError(QString)));

    this->queryScheduler->increaseRefCount();
    setInProgress(true, true, true);
    tableModel->setFetchInProgress(true);

    exporterThread->start();
}

void WorksheetResultsetTab::recordsExported(int count)
{
    statusBarLabel->setText(QString("  %1 records exported").arg(count));
}

void WorksheetResultsetTab::exportComplete()
{
    this->queryScheduler->decreaseRefCount();
    setInProgress(false);
    exporterThread=0;

    ResultsetTableModel *tableModel = qobject_cast<ResultsetTableModel*>(resultsTable->model());
    Q_ASSERT(tableModel);
    tableModel->setFetchInProgress(false);
}

void WorksheetResultsetTab::exportError(const QString &errorMessage)
{
    exportComplete();

    QMessageBox::critical(this->window(), tr("Error exporting data"),
                          tr("Following error occured while exporting data:\n%1").arg(errorMessage));
}

void WorksheetResultsetTab::stopProgress()
{
    Q_ASSERT(exporterThread);

    if(QMessageBox::question(this->window(),
                             tr("Stop export"),
                             tr("Do you want to stop exporting data?"),
                             QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok){

        if(exporterThread){ //thread may have completed its job by the time user presses OK button
            exporterThread->stop();
            stopProgressButton->setVisible(false);
        }
    }
}

void WorksheetResultsetTab::reloadQuery()
{
    resultsTable->displayQueryResults(this->queryScheduler, this->lastQuery, Param::cloneParams(this->lastParams));
}

void WorksheetResultsetTab::cleanup()
{
    qDeleteAll(lastParams);
}

void WorksheetResultsetTab::setInProgress(bool inProgress, bool showsStatusMessage, bool cancellable)
{
    progressBarAction->setVisible(inProgress);
    dataExportAction->setEnabled(!inProgress && !cursorClosed);
    countRowsAction->setEnabled(!inProgress && !lastQuery.isEmpty());

    labelAction->setVisible(inProgress && showsStatusMessage);
    stopProgressButton->setVisible(inProgress && cancellable);
}

void WorksheetResultsetTab::makeEditable(bool editable)
{
    editController->clearColumnDelegates();
    editController->setQueryScheduler(this->queryScheduler);

    if(!editable){
        editController->enableEditActions(false);
        editController->setObjectName("","","");
        resultsTable->setEditable(false);
        resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        return;
    }

    QString schemaName, tableName, dblinkName;
    PlSqlParseHelper::findTableNameInSelectQuery(lastQuery, &schemaName, &tableName, &dblinkName);
    if(!tableName.isEmpty()){
        editController->enableEditActions(true);
        editController->setObjectName(schemaName, tableName, dblinkName);
        resultsTable->setEditable(true);
        resultsTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
}
