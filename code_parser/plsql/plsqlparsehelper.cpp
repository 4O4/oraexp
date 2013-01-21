#include "plsqlparsehelper.h"
#include "plsqlscanner.h"
#include "../stringreader.h"
#include "plsqltokens.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include <QDebug>

QStringList PlSqlParseHelper::getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes)
{
    QStringList results;

    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));


    int token = PLS_E_O_F;
    bool waitingForName=false;
    bool isCursor=false;
    int openKeywordIx=PlSqlParsingTable::getInstance()->getKeywordIx("OPEN");

    do{
        token = scanner->getNextToken();

        if(token==openKeywordIx){
            isCursor=true;
            waitingForName=false;
        }else if(token==PLS_COLON){ //waiting for bind variable name
            waitingForName=true;
        }else if(waitingForName && (token==PLS_ID || token==PLS_DOUBLEQUOTED_STRING)){
            waitingForName=false;

            //must keep single entry for non double quoted strings regardless of case
            //and must keep single entry for double quoted strings taking into account case
            QString paramName = scanner->getTokenLexeme();
            if(!results.contains(paramName,
                                 token==PLS_DOUBLEQUOTED_STRING ?
                                        Qt::CaseSensitive :
                                        Qt::CaseInsensitive)){
                results.append(token==PLS_DOUBLEQUOTED_STRING ? paramName : paramName.toUpper());
            }

            if(suggestedParamTypes!=0){
                if(isCursor){
                    suggestedParamTypes->append(BindParamInfo::Cursor);
                }else {
                    QStringList nameParts=paramName.split('_');
                    if(nameParts.contains("DATE", Qt::CaseInsensitive) ||
                            nameParts.contains("TIME", Qt::CaseInsensitive) ||
                            nameParts.contains("DATETIME", Qt::CaseInsensitive)){
                        suggestedParamTypes->append(BindParamInfo::Date);
                    }else{
                        suggestedParamTypes->append(BindParamInfo::StringOrNumber);
                    }
                }
            }
            isCursor=false;
        }else if(waitingForName || isCursor){
            waitingForName=false;
            isCursor=false;
        }

    }while(token!=PLS_E_O_F && token!=PLS_ERROR);

    qDebug() << results;

    return results;
}

PlSqlParseHelper::PlSqlParseHelper()
{
}