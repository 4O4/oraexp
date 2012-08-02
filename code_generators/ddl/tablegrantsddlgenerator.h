#ifndef TABLEGRANTSDDLGENERATOR_H
#define TABLEGRANTSDDLGENERATOR_H

#include "beans/tableinfo.h"

class TableGrantsDdlGenerator
{
public:
    TableGrantsDdlGenerator();

    static QString generateDdl(const TableInfo &tableInfo);
    static QList<NameQueryPair> generateAlterDdl(const TableInfo &tableInfo);
    static QString generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo);
};

#endif // TABLEGRANTSDDLGENERATOR_H
