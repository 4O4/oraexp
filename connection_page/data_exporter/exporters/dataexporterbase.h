#ifndef DATAEXPORTERBASE_H
#define DATAEXPORTERBASE_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QPair>
#include <QSharedPointer>

class ResultsetColumnMetadata;
class QFile;

class DataExporterBase
{
public:
    enum ExportFormat{
        CSV,
        Excel,
        HTML,
        XML,
        Insert
    };

    DataExporterBase();
    virtual ~DataExporterBase();

    virtual void reset();

    QString filename;
    QString encoding;
    bool bom;

    QString stringBuffer;

    int startRow;
    int startColumn;
    int endRow;
    int endColumn;

    bool includeNullText;
    bool includeColumnHeaders;
    bool quoteColumnHeaders;

    QString stringQuoting;
    QString numberQuoting;

    QString lineEnding;
    QString columnDelimiter;
    bool delimiterAfterLastColumn;

    bool isXmlFile;

    QList< QPair<QString,QString>* > stringReplacements;

    void setStreamOpenMode(QIODevice::OpenMode openMode);

    void prepareColumnHeaders(QStringList &headers);
    void prepareRow(QStringList &row);

    virtual void startDocument(QTextStream &out){Q_UNUSED(out);}
    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out) = 0;
    virtual void endDocument(QTextStream &out){Q_UNUSED(out);}

    virtual QString getMimeType() const = 0;

    virtual QTextStream *createOutputStream(QString &errorMessage);

    QSharedPointer<ResultsetColumnMetadata> columnMetadata;

    QStringList columnTitles;

    QStringList getColumnTitles() const;

protected:
    void setTextStreamProperties();
    QTextStream *textStream;

    virtual void prepareField(QString &fieldValue, int fieldIx);

private:
    QFile *file;
    QIODevice::OpenMode streamOpenMode;

};

#endif // DATAEXPORTERBASE_H
