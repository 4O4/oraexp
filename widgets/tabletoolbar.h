#ifndef TABLETOOLBAR_H
#define TABLETOOLBAR_H

#include <QToolBar>

class DataTable;
class GenericEditableTableModel;
class QToolButton;
class QMenu;

class TableToolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit TableToolbar(DataTable *table, QWidget *parent = 0);

    void addAdvancedOptionsButton(QObject *receiver, const char *slotName);

private:
    DataTable *table;

    QAction *addMoreRowsAction;
    QAction *insertRowAction;
    QAction *deleteCurrentRowAction;

    QAction *moveUpAction;
    QAction *moveDownAction;

private slots:
    void addMoreRows();
    void addCopyOf();
    void insertRow();
    void deleteCurrentRow();

    void moveRowUp();
    void moveRowDown();

    void populateAddActionMenu();

private:
    void moveRows(bool moveUp) const;

    QMenu *addActionMenu;

};

#endif // TABLETOOLBAR_H
