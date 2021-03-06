#ifndef DBUTIL_H
#define DBUTIL_H

#include "enums.h"
#include <QString>
#include <QStringList>
#include "navtree/dbtreemodel.h"
#include "beans/environmentinfo.h"
#ifndef CODE_PARSER_BUILD
#include <ocilib.h>
#endif

class Statement;

class ResultsetColumnMetadata;

class DbUtil
{
public:
    DbUtil();

#ifndef CODE_PARSER_BUILD
    static void checkForOciError(Statement *statement=0);
    static void throwOciException(OCI_Error *error, Statement *statement);
#endif

    static QString getConstraintTypeLetter(OraExp::ConstraintType constraintType);
    static QString getConstraintTypeName(int constraintType, bool translate=true);

    static OraExp::TableType getTableType(const QString &temporary, const QString &temporaryDuration,
                                          const QString &iotType, const QString &external);

    static QString getIndexTypeName(int indexType, bool translate=true);
    static QStringList getIndexTypeNames();
    static QStringList getIndexParallelOptionNames();
    static QString getIndexPartitioningTypeName(int partitioningType, bool translate=true);
    static QStringList getIndexPartitioningTypeNames();
    static void populatePrivilegeNames(QStringList &list, DbTreeModel::DbTreeNodeType objectType);
    static QString getPartitionTypeName(OraExp::PartitionType pType);
    static QStringList getConstraintTypeNames();
    static QList<QPixmap> getConstraintTypeIcons();
    static QStringList getOnDeleteActionNames();
    static QStringList getOracleLoaderDataTypeNames();
    static QStringList getOracleLoaderTrimTypes();
    static QStringList getDateTypeNames();
    static QStringList getExternalTableColumnTransformTypes();
    static QStringList getExternalTableLobTypeNames();
    static QStringList getGrantTypes();

    static QString getDbObjectDisplayNameByNodeType(DbTreeModel::DbTreeNodeType nodeType);
    static QString getDbObjectTypeNameByNodeType(DbTreeModel::DbTreeNodeType nodeType);
    static DbTreeModel::DbTreeNodeType getDbObjectParentNodeType(DbTreeModel::DbTreeNodeType nodeType);
    static DbTreeModel::DbTreeNodeType getDbObjectNodeTypeByTypeName(const QString &typeName);
    static QString getDbObjectIconNameByParentNodeType(DbTreeModel::DbTreeNodeType nodeType);
    static QString getTableIconName(bool isPk);

    static QString getSizeUnitText(OraExp::ExtentUnit unit);
    static OraExp::ExtentUnit getSizeUnitFromText(const QString &unit);
    static QStringList getSizeUnitTexts();

    static bool isLob(const QString &dataType);
    static QStringList getComparableDataTypes();

    static void escapeFieldValue(QString &fieldValue, ResultsetColumnMetadata *metadata, int colIx, bool guessDateFormat = false);
    static void escapeFieldValue(QString &fieldValue, OraExp::ColumnDataType dataType, bool isTextField, OraExp::ColumnSubType subType = OraExp::CSTUnknown, bool guessDateFormat = false);

    static OraExp::ColumnSubType getIntervalSubType(const QString &dataTypeName);
    static OraExp::ColumnSubType getTimestampSubType(const QString &dataTypeName);

    static unsigned int toOciTimestampSubType(OraExp::ColumnSubType tsSubType);
    static unsigned int toOciIntervalSubType(OraExp::ColumnSubType intSubType);

    static bool isStringType(const QString &dataTypeName);

    static bool isRawType(const QString &dataTypeName);

    static bool isNumericType(const QString &dataTypeName);
    static bool isNumericType(OraExp::ColumnDataType dataType);

    static bool isDateType(const QString &dataTypeName);
    static bool isDateType(OraExp::ColumnDataType dataType);

    static bool isIntervalType(const QString &dataTypeName);
    static bool isIntervalType(OraExp::ColumnDataType dataType);

    static bool isTimestampType(const QString &dataTypeName);
    static bool isTimestampType(OraExp::ColumnDataType dataType);

    static QString toDate(const QString &columnName, const QString &format = "");
    static QString toInterval(const QString &columnName, OraExp::ColumnSubType intSubType);
    static QString toTimestamp(const QString &columnName, OraExp::ColumnSubType tsSubType, const QString &format = "");

    static QString toDateOrIntervalOrTimestamp(const QString &columnName, const QString &dataType, const QString &format = "");
    static QString toIntervalOrTimestamp(const QString &columnName, const QString &dataType, const QString &format = "");
    static QString intervalOrTimestampToChar(const QString &columnName, const QString &dataType);

    static bool isPLSQLProgramUnit(DbTreeModel::DbTreeNodeType itemType);
    static bool isPLSQLProgramUnit(const QString &itemType);
    static DbTreeModel::DbTreeNodeType getSpecType(DbTreeModel::DbTreeNodeType itemType);
    static DbTreeModel::DbTreeNodeType getBodyType(DbTreeModel::DbTreeNodeType itemType);

    static bool isSpecType(DbTreeModel::DbTreeNodeType itemType);
    static bool isBodyType(DbTreeModel::DbTreeNodeType itemType);
    static bool isSpecOrBodyType(DbTreeModel::DbTreeNodeType itemType);

    static QString getExtentSizeClause(bool unlimited, uint size, OraExp::ExtentUnit sizeUnit);
    static void parseExtentSize(const QString &textToParse, bool *unlimited, qulonglong *quota, OraExp::ExtentUnit *units);

    static void getDateFormats(QList<QString> *dateFormats, QList<QString> *datePatterns);
    static void getTimeFormats(QList<QString> *timeFormats, QList<QString> *timePatterns);
    static QStringList getDateTimeFormats();

    static QString getSaveSuffixForItemType(DbTreeModel::DbTreeNodeType itemType);

    static QList<EnvironmentInfo> getEnvironmentList();

    //static DbTreeModel::DbTreeNodeType getPairNodeType(DbTreeModel::DbTreeNodeType nodeType);
};

#endif // DBUTIL_H
