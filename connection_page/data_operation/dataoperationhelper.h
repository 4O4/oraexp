#ifndef DATAOPERATIONHELPER_H
#define DATAOPERATIONHELPER_H

#include "interfaces/idbobjectcomparehelper.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/dataoperationoptions.h"
#include "connectivity/ociexception.h"
#include "beans/tableinfofordatacomparison.h"
#include "navtree/dbtreemodel.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/datacompareinfo.h"
#include <QObject>

class DbTreeModel;
class DataOperationThread;
class StopableThread;

class DataOperationHelper : public QObject, public IDbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit DataOperationHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                 const QString &targetSchema, IQueryScheduler *targetScheduler,
                                 DbTreeModel *model, DataOperationOptions *options,
                                 const QHash<QString, TableInfoForDataComparison> &tableOptions,
                                 QObject *parent);
    virtual ~DataOperationHelper();
    
    virtual void compare();
    virtual bool canStart() {return true;}

    virtual void stop();

signals:
    void statusChanged(const QString &newStatus);
    void completed();
    void comparisonError(const QString &taskName, const OciException &exception);
    void objectCountDetermined(int count);
    void chunkCompleted(int size);
    void queryTextAvailable(const QString &queryText);
    void compareInfoAvailable(const DataCompareInfo &info);
    void tableCompareSuccess(const QString &tableName);
    
protected slots:
    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);

    void tableComparisonCompleted();
    void tableComparisonError(const QString &taskName, const OciException &exception);

    void subComparisonError(const QString &taskName, const OciException &exception);

    void refConstraintsDisabled(const QueryResult &result);
    void refConstraintsEnabled(const QueryResult &result);

    void tableSortQueryCompleted(const QueryResult &result);
    void tableSortRecordFetched(const FetchResult &fetchResult);
    void tableSortFetchCompleted(const QString &);

protected:
    QString sourceSchema;
    IQueryScheduler *sourceScheduler;
    QString targetSchema;
    IQueryScheduler *targetScheduler;
    DbTreeModel *model;
    DataOperationOptions *options;
    QHash<QString, TableInfoForDataComparison> tableOptions;

    QString currentTableName;

    QList<QModelIndex> itemsToCompare;
    int currentItemIxToCompare;

    StopableThread *workerThread;

    QString tableNamesToDisableConstraints;

    QString lastExceptionTaskName;
    OciException lastException;

    QString enableRefContraintsQuery;

    bool needToDisableRefConstraints;

    int objectCount;

    void emitCompletedSignal();
    QStringList getItemsToCompare() const;
    void sortTableNames(const QStringList &tableNames);
    void fillItemsToCompare(const QStringList &tableNames);
    void loadTableColumns();
    void compareNextItem();
    virtual void startWorkerThread(const QString &tableName) = 0;
    void deleteWorkerThread();

    void startToCompare();

    void disableRefConstraints();
    void enableRefConstraints();
    bool allCompared() const;
};

#endif // DATAOPERATIONHELPER_H
