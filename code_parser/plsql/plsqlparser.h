#ifndef PLSQLPARSER_H
#define PLSQLPARSER_H

#include "../codeparser.h"
#include "plsqlscanner.h"

class PlSqlParser : public CodeParser
{
public:
    PlSqlParser(CodeScanner *scanner);

    virtual ParsingTable *getParsingTable();
};

#endif // PLSQLPARSER_H