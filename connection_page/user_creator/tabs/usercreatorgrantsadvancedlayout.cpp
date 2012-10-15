#include "usercreatorgrantsadvancedlayout.h"
#include "interfaces/iqueryscheduler.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/rolegrantsmodel.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"
#include <QtGui>

UserCreatorGrantsAdvancedLayout::UserCreatorGrantsAdvancedLayout(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    tab = new QTabWidget();
    tab->setTabPosition(QTabWidget::South);

    rolesTable = new DataTableAndToolBarWidget();
    tab->addTab(rolesTable, tr("Roles"));

    privTable = new DataTableAndToolBarWidget();
    tab->addTab(privTable, tr("System privileges"));

    quotasTable = new DataTableAndToolBarWidget();
    tab->addTab(quotasTable, tr("Quotas"));

    objectPrivTable = new DataTableAndToolBarWidget();
    tab->addTab(objectPrivTable, tr("Object privileges"));

    mainLayout->addWidget(tab);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void UserCreatorGrantsAdvancedLayout::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    customizeRolesTable();
}

void UserCreatorGrantsAdvancedLayout::customizeRolesTable()
{
    DataTable *table=rolesTable->table();

    QStringList columnNames;
    columnNames.append(tr("Role"));
    columnNames.append(tr("Grantable"));
    columnNames.append(tr("Default"));

    RoleGrantsModel *tableModel=new RoleGrantsModel(columnNames, this);
    rolesTable->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);
    table->setColumnWidth(RoleGrantsModel::RoleName, 250);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *roleListDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                "get_role_list", "users",
                                                                this, true);

    table->setItemDelegateForColumn(RoleGrantsModel::RoleName, roleListDelegate);

    BooleanDelegate *isGrantableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(RoleGrantsModel::IsGrantable, isGrantableDelegate);

    BooleanDelegate *isDefaultDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(RoleGrantsModel::IsDefault, isDefaultDelegate);


    /*
    if(isEditMode()){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));*/
}
