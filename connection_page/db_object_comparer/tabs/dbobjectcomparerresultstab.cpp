#include "dbobjectcomparerresultstab.h"
#include "widgets/datatable.h"
#include <QtGui>

DbObjectComparerResultsTab::DbObjectComparerResultsTab(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent),
    uiManager(uiManager),
    noText(true)
{

}

void DbObjectComparerResultsTab::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0, 2, 0, 2);

    QSplitter *splitter=new QSplitter(Qt::Vertical);

    changeListTable=createChangeListTable();

    splitter->addWidget(changeListTable);

    editor=new CodeEditorAndSearchPaneWidget();
    editor->editor()->setUndoRedoEnabled(false);
    editor->setVisible(false);
    splitter->addWidget(editor);

    //splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    setLayout(layout);
}

DataTable *DbObjectComparerResultsTab::createChangeListTable()
{
    DataTable *changeListTable=new DataTable();

    tableModel=new QStandardItemModel(0, 3, this);
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Object name") << tr("Object type") << tr("Status"));
    changeListTable->setModel(tableModel);
    changeListTable->horizontalHeader()->setDefaultSectionSize(300);
    changeListTable->horizontalHeader()->setStretchLastSection(true);
    changeListTable->setSortingEnabled(true);
    changeListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    changeListTable->setUiManager(uiManager);

    return changeListTable;
}

void DbObjectComparerResultsTab::clearText()
{
    tableModel->setRowCount(0);
    editor->editor()->clear();
    noText=true;
}

void DbObjectComparerResultsTab::addText(const QString &text)
{
    if(noText && text.size()>1 && text.startsWith('\n')){
        noText=false;
        editor->editor()->addText(text.mid(1));
        editor->setVisible(true);
    }else{
        editor->editor()->addText(text);
    }
}

void DbObjectComparerResultsTab::setCurrentSchema(const QString &)
{

}
