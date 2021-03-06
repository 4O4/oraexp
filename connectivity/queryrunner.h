#ifndef QUERYRUNNER_H
#define QUERYRUNNER_H

#include <QRunnable>
#include "queryresult.h"
#include "ociexception.h"
#include "util/param.h"
#include "util/queryexectask.h"

class DbConnection;

class QueryRunner : public QRunnable
{
public:
    explicit QueryRunner(DbConnection *db, const QueryExecTask &task, QObject *parent);
    ~QueryRunner();

    virtual void run();

private:
    DbConnection *db;
    QueryExecTask task;
    QObject *parent;

    void fetchResultsets(QueryResult &result, const QueryExecTask &task);
    void fetchResultset(QueryResult &result, const QueryExecTask &task, int rsIx, int rsCount);
    void invokeFetchCompletedSlot(const QueryResult &result, const QueryExecTask &task, int resultsetIx = -1, int resultsetCount = -1);
    bool checkPointer(QObject *object, const QString &context);
};

#endif // QUERYRUNNER_H
