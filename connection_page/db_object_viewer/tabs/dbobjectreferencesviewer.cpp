#include "dbobjectreferencesviewer.h"
#include "util/dbutil.h"

DbObjectReferencesViewer::DbObjectReferencesViewer(bool references, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerGenericTab(references ? "get_object_references_for_detailed_view" : "get_object_dependecies_for_detailed_view",
                             uiManager, parent)
{
    setIconColumn("OWNER", "OWNER_ICON_COLUMN");
    setIconColumn("NAME", "OBJ_ICON_COLUMN");
    setObjectListMode(0,1,2);
}

QList<Param *> DbObjectReferencesViewer::getQueryParams()
{
    DbTreeModel::DbTreeNodeType type=itemType;

    if(type==DbTreeModel::PackageSpec){
        type=DbTreeModel::Package;
    }else if(type==DbTreeModel::TypeSpec){
        type=DbTreeModel::Type;
    }

    return DbObjectViewerGenericTab::getQueryParams() <<
           new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(type));
}
