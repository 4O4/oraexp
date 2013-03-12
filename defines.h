#ifndef DEFINES_H
#define DEFINES_H

#include <QPair>

#define MAX_IDENTIFIER_LENGTH 30

#define DATA_TYPE_ORIGINAL_VALUE_ROLE_ID 1000

#define SCHEMA_COMPARER_CHUNK_SIZE 100

#define DB_DATE_FORMAT "YYYY-MM-DD HH24:MI:SS"
#define W_DB_DATE_FORMAT L"YYYY-MM-DD HH24:MI:SS"

#define DB_TIMESTAMP_FORMAT "YYYY-MM-DD HH24:MI:SS.FF9"
#define W_DB_TIMESTAMP_FORMAT L"YYYY-MM-DD HH24:MI:SS.FF9"

#define DB_TZ_TIMESTAMP_FORMAT "YYYY-MM-DD HH24:MI:SS.FF9TZH:TZM"
#define W_DB_TZ_TIMESTAMP_FORMAT L"YYYY-MM-DD HH24:MI:SS.FF9TZH:TZM"

typedef QPair<QString,QString> NameQueryPair;
typedef QPair<QString,QString> StringPair;

#define PLACEHOLDER_DEFAULT QObject::tr("default")

#define DB_PREFETCH_SIZE 50

#endif // DEFINES_H
