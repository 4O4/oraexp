#include "worksheetresultsettab.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "connection_page/data_exporter/dataexportdialog.h"
#include "connection_page/data_exporter/dataexporterthread.h"
#include "connection_page/data_exporter/exporters/dataexporterbase.h"
#include "models/resultsettablemodel.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

WorksheetResultsetTab::WorksheetResultsetTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent),
    queryScheduler(0),
    exportDialog(0)
{
    QVBoxLayout *layout=new QVBoxLayout();

    createToolbar();

    layout->addWidget(toolbar);

    resultsTable=new DataTable();
    resultsTable->setAlternatingRowColors(true);
    layout->addWidget(resultsTable);

    layout->setContentsMargins(2, 0, 2, 0);
    setLayout(layout);

    connect(resultsTable, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
}

WorksheetResultsetTab::~WorksheetResultsetTab()
{
    delete exportDialog;
}

void WorksheetResultsetTab::addTabSpecificToolbarButtons()
{
    dataExportAction = toolbar->addAction(IconUtil::getIcon("export"), tr("Export"), this, SLOT(exportData()));
    dataExportAction->setEnabled(false);
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetResultsetTab::getTabType() const
{
    return WorksheetResultPane::ResultsetTab;
}

void WorksheetResultsetTab::showQueryResults(IQueryScheduler *, const QueryResult &)
{
    Q_ASSERT(false);
}

void WorksheetResultsetTab::displayResultset(IQueryScheduler *queryScheduler, Resultset *rs)
{
    progressBarAction->setVisible(true);
    dataExportAction->setEnabled(false);
    resultsTable->setResultset(queryScheduler, rs, QHash<int,StatementDesc*>());

    this->queryScheduler = queryScheduler;
}

void WorksheetResultsetTab::firstFetchCompleted()
{
    progressBarAction->setVisible(false);

    dataExportAction->setEnabled(true);
}

void WorksheetResultsetTab::exportData()
{
    if(exportDialog==0){
        exportDialog = new DataExportDialog();
    }

    int startRow, startColumn, endRow, endColumn;
    resultsTable->getSelectedRange(&startRow, &startColumn, &endRow, &endColumn);
    exportDialog->exportWidget()->setSelectedRange(startRow, startColumn, endRow, endColumn);

    if(exportDialog->exec()){
        DataExporterBase *exporter = exportDialog->exportWidget()->createExporter();
        startExport(exporter);
    }
}

void WorksheetResultsetTab::startExport(DataExporterBase *exporter)
{
    Q_ASSERT(queryScheduler);

    ResultsetTableModel *tableModel = qobject_cast<ResultsetTableModel*>(resultsTable->model());
    Q_ASSERT(tableModel);

    bool fetchToEnd=false;
    if(!tableModel->isAllDataFetched() && exporter->startRow==-1){
        QMessageBox::StandardButton result = QMessageBox::question(this->window(), tr("Not all records fetched"),
                              tr("Please, note that not all available records are currently fetched.\n"
                                 "If you want to export the rest of available records as well press Yes.\n"
                                 "If you want to export only records currently in grid press No."),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes);
        if(result == QMessageBox::Yes){
            fetchToEnd = true;
            tableModel->setAllDataFetched();
        }
    }

    QStringList columnTitles;
    for(int i=0; i<tableModel->columnCount(); ++i){
        columnTitles.append(tableModel->headerData(i, Qt::Horizontal).toString());
    }

    DataExporterThread *thread = new DataExporterThread(exporter, columnTitles,
                                                        tableModel->getModelData(),
                                                        tableModel->getResultset(),
                                                        fetchToEnd, this);
    connect(thread, SIGNAL(recordsExported(int)), this, SLOT(recordsExported(int)));
    connect(thread, SIGNAL(exportComplete()), this, SLOT(exportComplete()));
    connect(thread, SIGNAL(exportError(QString)), this, SLOT(exportError(QString)));

    this->queryScheduler->increaseRefCount();
    progressBarAction->setVisible(true);
    dataExportAction->setEnabled(false);
    tableModel->setFetchInProgress(true);

    thread->start();
}

void WorksheetResultsetTab::recordsExported(int count)
{
    qDebug() << "exported" << count << "rows";
}

void WorksheetResultsetTab::exportComplete()
{
    this->queryScheduler->decreaseRefCount();
    progressBarAction->setVisible(false);
    dataExportAction->setEnabled(true);

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
