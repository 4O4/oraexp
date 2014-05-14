#ifndef PLSQLPARSINGTABLE_H
#define PLSQLPARSINGTABLE_H

#include "../parsingtable.h"
#include "plsqltokens.h"

class PlSqlParsingTable : public ParsingTable
{
public:
    static PlSqlParsingTable *getInstance();

    bool isSectionSeparator(int token) const;
protected:
    PlSqlParsingTable();

private:
    void populateTable();
    void populateKeywords();
    void populateReservedWords();
    void populateRuleNames();
    void populateRuleOptions();
    void initializeActions();
    void createStates();

    static PlSqlParsingTable *instance;
};

#endif // PLSQLPARSINGTABLE_H
