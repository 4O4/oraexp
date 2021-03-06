#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableView>
#include "util/param.h"
#include "beans/iactionpropertysetter.h"
#include "connectivity/queryresult.h"

class DbConnection;
class Resultset;
class DbConnection;
class IQueryScheduler;
class StatementDesc;
class DbUiManager;
class ResultsetTableModel;
class DataExporterBase;

class DataTable : public QTableView, public IActionPropertySetter
{
    Q_OBJECT
public:
    explicit DataTable(QWidget *parent = 0);
    void setResultset(IQueryScheduler *queryScheduler,
                      Resultset *rs,
                      QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void displayQueryResults(IQueryScheduler *queryScheduler, const QString &query, QList<Param*> params,
                             QHash<int, StatementDesc*> dynamicQueries=QHash<int, StatementDesc*>());

    void clear();

    void setIconColumn(const QString &displayColumnName, const QString &iconColumnName);
    void setIconColumns(const QHash<QString, QString> &iconColumns);

    void setHumanizeColumnNames(bool humanize){this->humanizeColumnNames=humanize;}

    void resizeColumnAccountingForEditor(int column);
    void resizeColumnsAccountingForEditor();

    void setUiManager(DbUiManager *uiManager);
    void setObjectListMode(int schemaNameCol,
                           int objectNameCol,
                           int parentObjectNameCol,
                           int objectTypeCol,
                           const QString &objectListSchemaName=QString(),
                           const QString &objectListParentObjectName=QString(),
                           const QString &objectListObjectType=QString());

    void getSelectedRange(int *startRow, int *startColumn, int *endRow, int *endColumn);

    void displayMessage(const QString &prefix, const OciException &ex);

    void setMaxColumnWidth(int maxColumnWidth);

    virtual void setActionProperties(DynamicAction *action);

    void invokeDefaultActionForObject(int row);

    void setEditable(bool editable = true);

    void closeEditors(bool commit = true);

    bool objectListModeEnabled() const;

public slots:
    void resizeColumnsToFitContents();

signals:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void asyncQueryCompleted(const QueryResult &result);
    void asyncQueryError(const OciException &ex);
    void firstFetchCompleted();

    void contextMenuTriggered(QAction *action);

protected:
    void keyPressEvent ( QKeyEvent * event );
    //void resizeEvent ( QResizeEvent * event );

    //void mousePressEvent ( QMouseEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );

private slots:
    void queryCompleted(const QueryResult &result);
    void showContextMenu(const QPoint &pos, QModelIndex index);
    QList<QAction *> getActionsForObject(int row);
    void handleFirstFetchCompleted();

    void indexPressed ( const QModelIndex & index );

    void copyAs();

private:
    IQueryScheduler *queryScheduler;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<QString, QString> iconColumns;

    bool humanizeColumnNames;
    bool quietMode;

    void copyToClipboard(DataExporterBase *exporter);

    QString objectListSchemaName;
    QString objectListParentObjectName;
    QString objectListObjectType;
    int schemaNameCol;
    int objectNameCol;
    int parentObjectNameCol;
    int objectTypeCol;

    DbUiManager *uiManager;

    int maxColumnWidth;

    QString selectedObjectType;

    void deleteCurrentModel();
    void displayError(const QString &prefix, const OciException &ex);
    int getVisibleRecordCount() const;

    bool editable;

    QAction *createCopyMenu(bool withHeader) const;
};

#endif // DATATABLE_H
