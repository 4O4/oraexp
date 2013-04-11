#ifndef DBOBJECTCOMPAREROPTIONSTAB_H
#define DBOBJECTCOMPAREROPTIONSTAB_H

#include <QWidget>
#include "beans/dbobjectcomparisonoptions.h"

class DbConnection;
class QVBoxLayout;

class DbObjectComparerOptionsTab : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectComparerOptionsTab(QWidget *parent = 0);

    virtual void createUi(){}

    virtual void targetConnectionEstablished(DbConnection *sourceDb, DbConnection *targetDb);

    virtual DbObjectComparisonOptions *getOptions() = 0;

    virtual bool validate();

protected:
    QVBoxLayout *createSingleColumnOptionsPane(QVBoxLayout *layout);
    
};

#endif // DBOBJECTCOMPAREROPTIONSTAB_H
