#include "dataexporterbase.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/strutil.h"
#include "util/filesystemutil.h"
#include <QFile>

DataExporterBase::DataExporterBase() :
    encoding("UTF-8"),
    includeNullText(false), includeColumnHeaders(false), quoteColumnHeaders(true),
    stringQuoting("\""), numberQuoting("\""), lineEnding(getPlatformLineEnding()), columnDelimiter(","),
    delimiterAfterLastColumn(false),
    isXmlFile(false), textStream(0), file(0), streamOpenMode(QIODevice::WriteOnly)
{
}

DataExporterBase::~DataExporterBase()
{
    qDeleteAll(stringReplacements);

    reset();
}

void DataExporterBase::reset()
{
    delete textStream;
    delete file;

    textStream = 0;
    file = 0;
}

void DataExporterBase::setStreamOpenMode(QIODevice::OpenMode openMode)
{
    this->streamOpenMode=openMode;
}

void DataExporterBase::prepareColumnHeaders(QStringList &headers)
{
    if(!includeColumnHeaders || !quoteColumnHeaders){
        return;
    }

    QString quotingSymbol = stringQuoting.isEmpty() ? "\"" : stringQuoting;

    for(int i=0; i<headers.size(); ++i){
        QString &s = headers[i];
        if(!s.startsWith(quotingSymbol)){
            s.prepend(quotingSymbol);
        }
        if(!s.endsWith(quotingSymbol)){
            s.append(quotingSymbol);
        }
    }
}

void DataExporterBase::prepareRow(QStringList &row)
{
    for(int i=0; i<row.size(); ++i){
        QString &s=row[i];

        prepareField(s, i);
    }
}

void DataExporterBase::prepareField(QString &fieldValue, int fieldIx)
{
    if(!stringReplacements.isEmpty()){
        for(int i=0; i<stringReplacements.size(); ++i){
            QPair<QString,QString>* pair=stringReplacements.at(i);
            fieldValue.replace(pair->first, pair->second);
        }
    }

    if(includeNullText && fieldValue.isEmpty()){
        fieldValue="NULL";
    }

    if(!stringQuoting.isEmpty() &&
            (columnMetadata.isNull() || columnMetadata->isTextColumn(startColumn+fieldIx+1))){
        fieldValue.replace(stringQuoting, QString("%1%1").arg(stringQuoting)); //first escape quoting characters inside string
        fieldValue.prepend(stringQuoting);
        fieldValue.append(stringQuoting);
    }else if(!numberQuoting.isEmpty() &&
             columnMetadata->getColumnDataType(startColumn+fieldIx+1)==OraExp::CDTNumeric){
        fieldValue.prepend(numberQuoting);
        fieldValue.append(numberQuoting);
    }

    if(this->isXmlFile){
        fieldValue = xmlEncode(fieldValue);
    }
}

void DataExporterBase::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    Q_UNUSED(headers);
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(out);
}


QTextStream *DataExporterBase::createOutputStream(QString &errorMessage)
{
    Q_ASSERT(file==0);
    Q_ASSERT(textStream==0);

    if(!this->filename.isEmpty() && FileSystemUtil::createTextStream(this->filename, this->encoding, this->bom,
                                     streamOpenMode, &textStream, &file, &errorMessage)){
        return textStream;
    }else if(filename.isEmpty()){
        textStream = new QTextStream(&this->stringBuffer, QIODevice::WriteOnly);
        return textStream;
    }else{
        return 0;
    }
}

QStringList DataExporterBase::getColumnTitles() const
{
    return columnTitles.isEmpty() ? columnMetadata->columnTitles : columnTitles;
}

void DataExporterBase::setTextStreamProperties()
{
    FileSystemUtil::setTextStreamProperties(this->textStream, this->encoding, this->bom);
}
