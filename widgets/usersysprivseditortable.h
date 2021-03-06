#ifndef USERSYSPRIVSEDITORTABLE_H
#define USERSYSPRIVSEDITORTABLE_H

#include "datatableandtoolbarwidget.h"
#include "beans/privgrantinfo.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class SysPrivGrantsModel;

class UserSysPrivsEditorTable : public DataTableAndToolBarWidget
{
    Q_OBJECT
public:
    explicit UserSysPrivsEditorTable(DbObjectCreator::CreatorMode creatorMode, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    QList<PrivGrantInfo> getList() const;
    void populateTable(QList<PrivGrantInfo> *originalItemList);

    void removeIncorrectRows();

signals:
    void ddlChanged();

protected slots:
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);
    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

private:
    IQueryScheduler *queryScheduler;

    DbObjectCreator::CreatorMode creatorMode;
    QList<PrivGrantInfo> *originalItemList;

    void customizeTable();
    static void setRowData(int rowIx, SysPrivGrantsModel *model, PrivGrantInfo *info);
    
};

#endif // USERSYSPRIVSEDITORTABLE_H
