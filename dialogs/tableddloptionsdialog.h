#ifndef TABLEDDLOPTIONSDIALOG_H
#define TABLEDDLOPTIONSDIALOG_H

#include <QDialog>
#include "beans/objectddloptions.h"

class TableDbmsDdlOptionsWidget;

class TableDdlOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableDdlOptionsDialog(QWidget *parent = 0);

    ObjectDdlOptions getOptions() const;
    void setOptions(const ObjectDdlOptions &options);

private:
    TableDbmsDdlOptionsWidget *optionsWidget;

};

#endif // TABLEDDLOPTIONSDIALOG_H
