#include "tabletoolbar.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

TableToolbar::TableToolbar(DataTable *table, QWidget *parent) :
    QToolBar(parent), table(table)
{
    setIconSize(QSize(16, 16));

    addMoreRowsAction=new QAction(IconUtil::getIcon("add"), tr("Add new row"), this);
    addActionMenu = new QMenu(tr("Add copy of row"), this);
    addMoreRowsAction->setMenu(addActionMenu);

    insertRowAction=new QAction(IconUtil::getIcon("insert_row"), tr("Insert row at current position"), this);
    deleteCurrentRowAction=new QAction(IconUtil::getIcon("delete"), tr("Delete selected rows"), this);

    moveUpAction=new QAction(IconUtil::getIcon("move_up"), tr("Move up"), this);
    moveDownAction=new QAction(IconUtil::getIcon("move_down"), tr("Move down"), this);

    addAction(addMoreRowsAction);
    addAction(insertRowAction);
    addAction(deleteCurrentRowAction);

    addSeparator();

    addAction(moveUpAction);
    addAction(moveDownAction);

    connect(addMoreRowsAction, SIGNAL(triggered()), this, SLOT(addMoreRows()));
    connect(addActionMenu, SIGNAL(aboutToShow()), this, SLOT(populateAddActionMenu()));

    connect(insertRowAction, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(deleteCurrentRowAction, SIGNAL(triggered()), this, SLOT(deleteCurrentRow()));

    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveRowUp()));
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveRowDown()));
}

void TableToolbar::addAdvancedOptionsButton(QObject *receiver, const char *slotName)
{
    addSeparator();
    QAction *advancedOptionsAction=addAction(IconUtil::getIcon("advanced_options"), tr("Show/Hide advanced options"));
    advancedOptionsAction->setCheckable(true);
    //addAction(advancedOptionsAction);

    connect(advancedOptionsAction, SIGNAL(toggled(bool)), receiver, slotName);
}

void TableToolbar::addMoreRows()
{
    QAbstractItemModel *model=table->model();
    model->insertRows(model->rowCount(), 1);
}

void TableToolbar::addCopyOf()
{
    QAction *action=qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    QVariant var=action->data();
    if(!var.isValid()){
        return;
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);

    model->addCopyOf(var.toInt());
}

void TableToolbar::insertRow()
{
    QItemSelectionModel *selection=table->selectionModel();
    QModelIndex currentIndex=selection->currentIndex();
    int rowToInsert=0;

    if(currentIndex.isValid()){
       rowToInsert=currentIndex.row();
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);

    if(rowToInsert>model->getFrozenRow()){
        model->insertRows(rowToInsert, 1);
    }else{
        QMessageBox::information(this->window(), tr("Not a valid position"),
                                 tr("Cannot insert row at this position"));
    }
}

void TableToolbar::deleteCurrentRow()
{
    WidgetHelper::deleteTableRow(table);
}

void TableToolbar::moveRowUp()
{
    moveRows(true);
}

void TableToolbar::moveRowDown()
{
    moveRows(false);
}

void TableToolbar::moveRows(bool moveUp) const
{
    QItemSelectionModel *selectionModel=table->selectionModel();
    Q_ASSERT(selectionModel);

    QModelIndex index=selectionModel->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this->window(), tr("No selection"),
                                 tr("Please, select one or more rows to move"));
        return;
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);


    //collect rows that will be moved
    QList<int> rowsToMove;
    for(int i=0; i<model->rowCount(); ++i){
        if((selectionModel->rowIntersectsSelection(i, QModelIndex()) || i==index.row())
                && i>model->getFrozenRow()){
            rowsToMove.append(i);
        }
    }

    QModelIndex indexToSetAsCurrent;
    selectionModel->clearSelection();
    if(moveUp){
        for(int i=0; i<model->rowCount(); ++i){
            if(i>0 && i>model->getFrozenRow()+1 && rowsToMove.contains(i) && !rowsToMove.contains(i-1)){
                model->moveRowUp(i);
                rowsToMove.removeOne(i);
                selectionModel->select(model->index(i-1, 0), QItemSelectionModel::Rows | QItemSelectionModel::Select);

                if(!indexToSetAsCurrent.isValid()){
                    indexToSetAsCurrent=model->index(i-1, index.column());
                }
            }
        }
    }else{
        for(int i=model->rowCount()-1; i>=0; --i){
            if(i<model->rowCount()-2 && rowsToMove.contains(i) && !rowsToMove.contains(i+1)){
                model->moveRowDown(i);
                rowsToMove.removeOne(i);
                selectionModel->select(model->index(i+1, 0), QItemSelectionModel::Rows | QItemSelectionModel::Select);

                indexToSetAsCurrent=model->index(i+1, index.column());
            }
        }
    }

    if(indexToSetAsCurrent.isValid()){
        selectionModel->setCurrentIndex(indexToSetAsCurrent, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }
}

void TableToolbar::populateAddActionMenu()
{
    qDeleteAll(addActionMenu->actions());

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    QAction *action;
    QString title;
    for(int i=0; i<model->rowCount(); ++i){
        title=model->data(model->index(i, model->getTitleColumn())).toString();
        if(model->isRowDataCorrect(i).isEmpty() && !title.isEmpty()){
            action=new QAction(title, this);
            action->setIcon(model->headerData(i, Qt::Vertical, Qt::DecorationRole).value<QPixmap>());
            action->setData(i);
            connect(action, SIGNAL(triggered()), this, SLOT(addCopyOf()));
            addActionMenu->addAction(action);
        }
    }

    if(addActionMenu->actions().isEmpty()){
        addActionMenu->addAction(tr("no item to copy"))->setEnabled(false);
    }
}
