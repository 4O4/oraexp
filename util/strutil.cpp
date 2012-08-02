#include "strutil.h"
#include <QRegExp>
#include <iostream>

using namespace std;

QString toQString(const dtext *ociString){
    return QString::fromWCharArray(ociString);
}

dtext* toOciString(const QString &qString){
    dtext *data = new dtext[qString.size()+1]; //must be freed by caller
    qString.toWCharArray(data);
    data[qString.size()]='\0';

    return data;
}

int getEditorIntValue(const QString &value)
{
    return value.isEmpty() ? -1 : value.toInt();
}

QString getEditorStrValue(int value)
{
    return value==-1 ? "" : QString::number(value);
}

QStringList splitTrimmed(const QString &text, bool convertToUpperCase)
{
    QStringList result;
    if(!text.isEmpty()){
        QStringList list=text.split(',');
        for(int i=0; i<list.count(); ++i){
            if(convertToUpperCase){
                result.append(list.at(i).trimmed().toUpper());
            }else{
                result.append(list.at(i).trimmed());
            }
        }
    }
    return result;
}

QString humanize(const QString &input)
{
    if(input.isEmpty()){
        return input;
    }

    QString result = input.toLower();

    result[0]=result.at(0).toUpper();
    int underscoreIx=result.indexOf('_');
    while(underscoreIx!=-1){
        if(result.length()>underscoreIx/*+1*/){
            result[underscoreIx]=' ';
            //result[underscoreIx+1]=result.at(underscoreIx+1).toUpper();
        }

        underscoreIx=result.indexOf('_', underscoreIx+1);
    }

    return result;
}

QString optionText(int option, const QString &option1,
                               const QString &option2)
{
    if(option==1){
        return option1;
    }else if(option==2){
        return option2;
    }else{
        return QString();
    }
}

QString optionText(int option, const QString &option1,
                               const QString &option2,
                               const QString &option3)
{
    if(option==1){
        return option1;
    }else if(option==2){
        return option2;
    }else if(option==3){
        return option3;
    }else{
        return QString();
    }
}

int numberAfterLastUnderscore(const QString &s)
{
    bool ok;
    int result=s.mid(s.lastIndexOf('_')+1).toInt(&ok);
    Q_ASSERT(ok);

    return result;
}

bool existsInCollection(const QString &commaSeparatedString, const QStringList &collection)
{
    QStringList splittedList=commaSeparatedString.split(',');
    foreach(const QString &splittedItem, splittedList){
        if(collection.contains(splittedItem)){
            return true;
        }
    }

    return false;
}

bool allExistInCollection(const QString &commaSeparatedString, const QStringList &collection)
{
    QStringList splittedList=commaSeparatedString.split(',');
    foreach(const QString &splittedItem, splittedList){
        if(!collection.contains(splittedItem)){
            return false;
        }
    }

    return true;
}

bool hasMatch(const QString &str, const QStringList &collection)
{
    foreach(const QString &item, collection){
        QRegExp r(item, Qt::CaseSensitive, QRegExp::RegExp2);
        if(r.exactMatch(str)){
            return true;
        }
    }

    return false;
}

void addEOL(QString &str)
{
    if(!str.isEmpty()){
        str.append("\n");
    }
}

QString formatMsecs(int msecs)
{
    int secs = msecs / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;
    return QString("%1:%2:%3")
    .arg(hours, 2, 10, QLatin1Char('0'))
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(secs, 2, 10, QLatin1Char('0'));
}

QString joinEnclosed(const QStringList &list, const QString &join, const QString &enclose)
{
    QString result;

    for(int i=0; i<list.size(); ++i){
        if(i>0){
            result.append(join);
        }
        result.append(enclose).append(list.at(i)).append(enclose);
    }

    return result;
}

QString ensureStartsWith(QString str, const QString &startWith)
{
    QRegExp regexp(QString("^\b%1\b").arg(startWith), Qt::CaseInsensitive, QRegExp::RegExp2);
    int alreadyStartsWith=regexp.indexIn(str.replace("\n"," "));
    if(alreadyStartsWith>-1){
        return str;
    }else{
        return QString("%1 %2").arg(startWith, str);
    }
}

void print(const char *msg)
{
    cout << msg << endl;
}
