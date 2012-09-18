#include "sequencecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

SequenceContextMenuUtil::SequenceContextMenuUtil()
{
}

QList<QAction *> SequenceContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Sequences){
        QAction *createSequenceAction=new DbItemAction(IconUtil::getIcon("sequence_add"), QObject::tr("Create sequence"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createSequenceAction);
    }else if(itemType==DbTreeModel::Sequence){
        //alter
        QAction *alterSequenceAction=new DbItemAction(IconUtil::getIcon("sequence_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterSequenceAction);
    }

    return results;
}