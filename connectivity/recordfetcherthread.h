#ifndef RECORDFETCHERTHREAD_H
#define RECORDFETCHERTHREAD_H

#include "ociexception.h"
#include <QThread>
#include <QHash>

class DbConnection;
class Resultset;
class StatementDesc;
class Statement;

class RecordFetcherThread : public QThread
{
    Q_OBJECT
public:
    explicit RecordFetcherThread(DbConnection *db, Resultset *rs, int recordCountToFetch,
                                 QHash<int, StatementDesc*> dynamicQueries,
                                 QObject *parent);

    void run();

    void setFetchRange(int startIx, int count);
    void setFetchInChunks(bool fetchInChunks);

signals:
    void recordsFetched(const QList<QStringList> &records);
    void fetchComplete();
    void fetchError(const OciException &ex);

private:
    void replaceValuesWithDynamicQueries(QList<QString> &oneRow);

    DbConnection *db;
    Resultset *rs;
    int fetchStartIx;
    int recordCountToFetch;
    bool fetchInChunks;
    QHash<int, StatementDesc*> dynamicQueries;

};

#endif // RECORDFETCHERTHREAD_H
