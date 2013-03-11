#ifndef DATAEXPORTERTHREAD_H
#define DATAEXPORTERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QTextStream>
#include <QSharedPointer>
#include "beans/resultsetcolumnmetadata.h"

class Resultset;
class DataExporterBase;

class DataExporterThread : public QThread
{
    Q_OBJECT
public:
    explicit DataExporterThread(DataExporterBase *exporter,
                                QList<QStringList> alreadyFetchedData,
                                Resultset *rs,
                                bool fetchToEnd,
                                QObject *parent);

    virtual ~DataExporterThread();
    
    void run();

    void stop();

signals:
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);

private:
    DataExporterBase *exporter;
    QList<QStringList> alreadyFetchedData;
    Resultset *rs;
    bool fetchToEnd;
    bool stopped;

    void exportToStream(QTextStream &out);
    
};

#endif // DATAEXPORTERTHREAD_H