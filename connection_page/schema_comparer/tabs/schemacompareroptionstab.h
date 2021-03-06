#ifndef SCHEMACOMPAREROPTIONSTAB_H
#define SCHEMACOMPAREROPTIONSTAB_H

#include <QWidget>
#include "beans/schemacomparisonoptions.h"
#include "connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h"

class QGroupBox;
class QCheckBox;
class QBoxLayout;
class QLineEdit;
class ExistingTableOptionsWidget;
class TableDdlExportOptionsWidget;
class StorageDdlExportOptionsWidget;
class SourceDdlExportOptionsWidget;
class SequenceDdlExportOptionsWidget;
class SequenceDdlDiffOptionsWidget;

class SchemaComparerOptionsTab : public DbObjectComparerOptionsTab
{
    Q_OBJECT
public:
    explicit SchemaComparerOptionsTab(QWidget *parent = 0);

    void setCanWrap(bool canWrap);
    void setCanFlashbackArchive(bool canFlashbackArchive);

    virtual void targetConnectionEstablished(DbConnection *sourceDb, DbConnection *targetDb);
    virtual DbObjectComparisonOptions *getOptions();

private:
    void createExistingTableOptionsPane(QBoxLayout *layout);
    void createNewTableOptionsPane(QBoxLayout *layout);
    void createNewObjectOptionsPane(QBoxLayout *layout);
    void createSourceOptionsPane(QBoxLayout *layout);
    void createSequenceOptionsPane(QBoxLayout *layout);
    //void createIndexOptionsPane(QBoxLayout *layout);
    void createOptionsPane(QBoxLayout *layout);

    QGroupBox *existingTableOptionsGroupBox;
    ExistingTableOptionsWidget *etOptionsWidget;

    QGroupBox *newTableOptionsGroupBox;
    TableDdlExportOptionsWidget *ntOptionsWidget;

    QGroupBox *newObjectOptionsGroupBox;
    StorageDdlExportOptionsWidget *noOptionsWidget;

    QGroupBox *sourceOptionsGroupBox;
    SourceDdlExportOptionsWidget *sourceOptionsWidget;

    QGroupBox *sequenceOptionsGroupBox;
    SequenceDdlExportOptionsWidget *seqExportOptionWidget;
    SequenceDdlDiffOptionsWidget *seqDiffOptionsWidget;

    /*QGroupBox *indexOptionsGroupBox;
    QCheckBox *indexPartitions;
    QCheckBox *indexLobStorage;
    QCheckBox *indexTablespace;*/

};

#endif // SCHEMACOMPAREROPTIONSTAB_H
