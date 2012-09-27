#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T15:14:48
#
#-------------------------------------------------

QT       += core gui

TARGET = OraExp
TEMPLATE = app

DEFINES += OCI_IMPORT_LINKAGE OCI_CHARSET_WIDE ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL

CONFIG(debug) {
     DEFINES += DEBUG
} else {
     DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT
}


SOURCES += main.cpp\
        mainwindow.cpp \
    connectdialog.cpp \
    connectioneditordialog.cpp \
    connectivity/dbconnection.cpp \
    mytablewidgetitem.cpp \
    util/iconutil.cpp \
    passwordentrydialog.cpp \
    connectionspane.cpp \
    connection_page/connectionpage.cpp \
    navtree/treepane.cpp \
    connection_page/worksheet/worksheet.cpp \
    codeeditor/codeeditor.cpp \
    connection_page/worksheet/worksheetresultpane.cpp \
    connection_page/worksheet/worksheetquerypane.cpp \
    navtree/dbtreemodel.cpp \
    navtree/dbtreeitem.cpp \
    navtree/table/dbtabletreeitem.cpp \
    connectivity/connection.cpp \
    util/strutil.cpp \
    navtree/table/dbtableparenttreeitem.cpp \
    connectivity/resultset.cpp \
    navtree/table/dbtablecolumnparentitem.cpp \
    navtree/table/dbtablecolumnitem.cpp \
    util/param.cpp \
    navtree/table/dbtableconstraintparentitem.cpp \
    util/queryutil.cpp \
    connectivity/ociexception.cpp \
    navtree/table/dbtableconstraintitem.cpp \
    navtree/table/dbconstraintcolumnitem.cpp \
    navtree/table/dbtableindexparentitem.cpp \
    navtree/index/dbindexitem.cpp \
    navtree/index/dbindexparentitem.cpp \
    navtree/table/dbtabletriggerparentitem.cpp \
    navtree/view/dbviewparentitem.cpp \
    navtree/view/dbviewitem.cpp \
    navtree/view/dbviewtriggerparentitem.cpp \
    navtree/package/dbpackageparentitem.cpp \
    navtree/package/dbpackageitem.cpp \
    navtree/package/dbpackagespecitem.cpp \
    navtree/package/dbpackagebodyitem.cpp \
    navtree/schema/dbschemaitem.cpp \
    navtree/schema/dballschemasitem.cpp \
    navtree/procedure/dbprocedureparentitem.cpp \
    navtree/procedure/dbprocedureitem.cpp \
    navtree/function/dbfunctionparentitem.cpp \
    navtree/function/dbfunctionitem.cpp \
    navtree/sequence/dbsequenceparentitem.cpp \
    navtree/sequence/dbsequenceitem.cpp \
    navtree/synonym/dbsynonymparentitem.cpp \
    navtree/synonym/dbsynonymitem.cpp \
    navtree/dblink/dbdblinkparentitem.cpp \
    navtree/dblink/dbdblinkitem.cpp \
    navtree/type/dbtypeparentitem.cpp \
    navtree/type/dbtypeitem.cpp \
    connection_page/dbuimanager.cpp \
    navtree/nodeaction.cpp \
    connection_page/table_info/tableinfotoolbar.cpp \
    connection_page/table_info/tabs/tablecolumnsviewer.cpp \
    connection_page/table_info/tabs/tabledataviewer.cpp \
    connection_page/table_info/tabs/tableconstraintsviewer.cpp \
    connection_page/table_info/tabs/tablegrantsviewer.cpp \
    connection_page/table_info/tabs/tabletriggersviewer.cpp \
    connection_page/table_info/tabs/tabledependenciesviewer.cpp \
    connection_page/table_info/tabs/tabledetailsviewer.cpp \
    connection_page/table_info/tabs/tablepartitionsviewer.cpp \
    connection_page/table_info/tabs/tableddlviewer.cpp \
    widgets/ondemandinfoviewerwidget.cpp \
    connection_page/table_info/tableinfoviewerwidget.cpp \
    widgets/datatable.cpp \
    models/resultsettablemodel.cpp \
    util/datetimewithtimezone.cpp \
    widgets/subtabwidget.cpp \
    widgets/nameeditor.cpp \
    connection_page/table_creator/tabs/tablecreatorgeneralinfo.cpp \
    connection_page/table_creator/tablecreatortab.cpp \
    connection_page/table_creator/tabs/tablecreatorcolumns.cpp \
    models/genericeditabletablemodel.cpp \
    delegates/datatypecomboboxdelegate.cpp \
    delegates/booleandelegate.cpp \
    connection_page/table_creator/tablecreatortabwithtableview.cpp \
    connection_page/table_creator/tabs/tablecreatorconstraints.cpp \
    delegates/schemaselectordelegate.cpp \
    delegates/comboboxdelegate.cpp \
    delegates/contrainttypedelegate.cpp \
    models/tableconstraintsmodel.cpp \
    delegates/ondeleteactiondelegate.cpp \
    delegates/indexbasedcomboboxdelegate.cpp \
    connectivity/dblocalcache.cpp \
    delegates/tableselectordelegate.cpp \
    delegates/checkboxlistdelegate.cpp \
    widgets/checkedcombobox.cpp \
    util/widgethelper.cpp \
    delegates/constraintselectordelegate.cpp \
    util/dbutil.cpp \
    dialogs/columnselectordialog.cpp \
    widgets/lineeditwithbutton.cpp \
    delegates/columnselectordelegate.cpp \
    delegates/identifiernamedelegate.cpp \
    delegates/lineeditwithbuttondelegate.cpp \
    delegates/plaintexteditordelegate.cpp \
    dialogs/plaintexteditordialog.cpp \
    connection_page/table_creator/tabs/tablecreatorcheckconstraints.cpp \
    connection_page/table_creator/tabs/tablecreatorindexes.cpp \
    models/tableindexesmodel.cpp \
    delegates/indexnameeditordelegate.cpp \
    delegates/numericdelegate.cpp \
    dialogs/storageparamsdialog.cpp \
    widgets/storageparamswidget.cpp \
    widgets/extentsizeeditorwidget.cpp \
    widgets/tablespacelistcombobox.cpp \
    beans/storageparams.cpp \
    connection_page/table_creator/tablecreatortabs.cpp \
    connection_page/table_creator/tablecreator.cpp \
    codeeditor/syntaxhighligher.cpp \
    codeeditor/linenumberarea.cpp \
    connectivity/queryresult.cpp \
    connection_page/worksheet/bottom_pane_tabs/worksheetinfotab.cpp \
    connection_page/worksheet/bottom_pane_tabs/worksheetresultsettab.cpp \
    connection_page/worksheet/bottom_pane_tabs/worksheetbottompanetab.cpp \
    widgets/comboboxwithbutton.cpp \
    dialogs/genericresultsetviewerdialog.cpp \
    widgets/tableadditionalattributeswidget.cpp \
    beans/tableadditionalattributes.cpp \
    dialogs/tableadditionalattributesdialog.cpp \
    models/tablecolumnsmodel.cpp \
    widgets/lobparamswidget.cpp \
    beans/lobparams.cpp \
    dialogs/lobparamsdialog.cpp \
    connection_page/table_creator/tabs/tablecreatorpartitions.cpp \
    connection_page/table_creator/tabs/tablecreatorgrants.cpp \
    models/tablegrantsmodel.cpp \
    widgets/centeredcheckboxwidget.cpp \
    connection_page/table_creator/tabs/tablecreatorexternalproperties.cpp \
    widgets/comboboxandlineeditwidget.cpp \
    widgets/oracleloaderdriverrecordpropertieswidget.cpp \
    widgets/oracleloaderdriverfieldpropertieswidget.cpp \
    widgets/tablecreatorexternalpropertiesgeneralinfowidget.cpp \
    widgets/lineeditandcheckboxwidget.cpp \
    widgets/tabletoolbar.cpp \
    widgets/datatableandtoolbarwidget.cpp \
    delegates/autoappenddelegate.cpp \
    models/externaltablefieldsmodel.cpp \
    models/externaltablecolumntransformsmodel.cpp \
    delegates/dynamiccomboboxdelegate.cpp \
    delegates/abstractcomboboxdelegate.cpp \
    widgets/oracledatapumpdriverproperties.cpp \
    widgets/indexorganizedtablepropertieswidget.cpp \
    dialogs/indexorganizedtablepropertiesdialog.cpp \
    beans/indexorganizedtableproperties.cpp \
    util/dialoghelper.cpp \
    dialogs/descriptiveerrordialog.cpp \
    navtree/nodepopulatorthread.cpp \
    navtree/dbtreeitemresult.cpp \
    navtree/loadingdbtreeitem.cpp \
    connection_page/connectionpagetab.cpp \
    connection_page/table_info/tableinfoviewertabs.cpp \
    connection_page/table_info/tableinfoviewer.cpp \
    util/asyncconnect.cpp \
    widgets/tablecreatorpartitionstab.cpp \
    widgets/lineeditwithautocompleter.cpp \
    util/queryexectask.cpp \
    util/queryqueuemanager.cpp \
    connectivity/recordfetcherthread.cpp \
    connection_page/table_info/tabs/tablereferencingfkviewer.cpp \
    connection_page/table_info/tableinfogenerictab.cpp \
    connectivity/fetchresult.cpp \
    grammar_parser/codescanner.cpp \
    grammar_parser/ebnfscanner.cpp \
    grammar_parser/ebnftoken.cpp \
    grammar_parser/ebnfparser.cpp \
    grammar_parser/bnfrule.cpp \
    grammar_parser/bnfruleitem.cpp \
    grammar_parser/dfaitem.cpp \
    grammar_parser/dfastate.cpp \
    grammar_parser/dfatransition.cpp \
    grammar_parser/dfa.cpp \
    connection_page/table_info/tabs/tableindexesviewer.cpp \
    beans/objectddloptions.cpp \
    widgets/tableddloptionswidget.cpp \
    dialogs/tableddloptionsdialog.cpp \
    connectivity/connectionpool.cpp \
    models/tablecheckconstraintsmodel.cpp \
    beans/columninfo.cpp \
    beans/constraintinfo.cpp \
    beans/indexinfo.cpp \
    beans/grantinfo.cpp \
    beans/partitioninfo.cpp \
    connectivity/queryrunner.cpp \
    beans/querylistitem.cpp \
    util/itemcreatorhelper.cpp \
    delegates/indexstorageparamsdelegate.cpp \
    widgets/tablecreatorexternalaccessparameterseditorwidget.cpp \
    beans/tableexternalinfo.cpp \
    models/partitionsmodel.cpp \
    beans/partitioninginfo.cpp \
    beans/tablepartitioninginfo.cpp \
    dialogs/partitioningparamsdialog.cpp \
    delegates/partitioningparamsdelegate.cpp \
    metadata_loaders/table/partitioninfoloader.cpp \
    widgets/tabwidget.cpp \
    app_menu/appmenu.cpp \
    app_menu/appfilemenu.cpp \
    app_menu/appeditmenu.cpp \
    app_menu/appviewmenu.cpp \
    app_menu/apptoolsmenu.cpp \
    app_menu/apphelpmenu.cpp \
    widgets/dbitemlistcombobox.cpp \
    app_menu/appmainmenu.cpp \
    widgets/codeeditorandsearchpanewidget.cpp \
    widgets/searchpane.cpp \
    widgets/closebutton.cpp \
    app_menu/appsessionmenu.cpp \
    connection_page/schema_comparer/schemacomparer.cpp \
    connection_page/schema_comparer/tabs/schemacomparercomparetab.cpp \
    widgets/connectionselectorwidget.cpp \
    util/queryscheduler.cpp \
    widgets/dbtreeview.cpp \
    navtree/trigger/dbtriggerparentitem.cpp \
    navtree/trigger/dbtriggeritem.cpp \
    navtree/index/dbindexcolumnitem.cpp \
    connection_page/schema_comparer/comparers/schemacomparerhelper.cpp \
    metadata_loaders/table/tableinfoloader.cpp \
    beans/tableinfo.cpp \
    beans/tablegeneralinfo.cpp \
    metadata_loaders/table/tablegeneralinfoloader.cpp \
    metadata_loaders/table/tablecolumninfoloader.cpp \
    metadata_loaders/table/tableconstraintsloader.cpp \
    metadata_loaders/table/tableindexesloader.cpp \
    metadata_loaders/table/tableexternalinfoloader.cpp \
    metadata_loaders/table/tablegrantsloader.cpp \
    connection_page/schema_comparer/dbobjecthasher.cpp \
    connection_page/schema_comparer/tabs/schemacompareroptionstab.cpp \
    connection_page/schema_comparer/tabs/schemacomparerresultstab.cpp \
    connection_page/schema_comparer/comparers/tablecomparehelper.cpp \
    code_generators/ddl/tableddlgenerator.cpp \
    code_generators/ddl/tablecolumnsddlgenerator.cpp \
    code_generators/ddl/tableconstraintsddlgenerator.cpp \
    code_generators/ddl/tableindexesddlgenerator.cpp \
    code_generators/ddl/tablegrantsddlgenerator.cpp \
    code_generators/ddl/tablepartitionsddlgenerator.cpp \
    connection_page/schema_comparer/comparers/dbobjectscomparehelper.cpp \
    connection_page/schema_comparer/comparers/dbobjectcomparehelper.cpp \
    connection_page/schema_comparer/comparers/dbobjectcomparerfactory.cpp \
    connection_page/schema_comparer/comparers/sourcecomparehelper.cpp \
    beans/tablediffddloptions.cpp \
    beans/tablecreateddloptions.cpp \
    beans/sourcecodeddloptions.cpp \
    beans/schemacomparisonoptions.cpp \
    beans/sequencediffddloptions.cpp \
    metadata_loaders/sequence/sequenceinfoloader.cpp \
    beans/dbobjectinfo.cpp \
    beans/sequenceinfo.cpp \
    metadata_loaders/metadataloader.cpp \
    beans/sourceinfo.cpp \
    metadata_loaders/code/sourceinfoloader.cpp \
    metadata_loaders/table/tabletriggersloader.cpp \
    beans/triggerinfo.cpp \
    code_generators/ddl/tabletriggersddlgenerator.cpp \
    connection_page/schema_comparer/comparers/sequencecomparehelper.cpp \
    metadata_loaders/metadataloaderfactory.cpp \
    beans/sequencecreateddloptions.cpp \
    beans/newdbobjectddloptions.cpp \
    connectivity/statement.cpp \
    beans/statementdesc.cpp \
    connection_page/data_comparer/datacomparer.cpp \
    connection_page/db_object_comparer/dbobjectcomparer.cpp \
    connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.cpp \
    connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.cpp \
    connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.cpp \
    beans/dbobjectcomparisonoptions.cpp \
    connection_page/data_comparer/tabs/datacompareroptionstab.cpp \
    connection_page/data_comparer/tabs/datacomparercomparetab.cpp \
    connection_page/data_comparer/comparers/datacomparerhelper.cpp \
    beans/datacomparisonoptions.cpp \
    connection_page/data_comparer/tableinfofordatacomparison.cpp \
    connection_page/data_comparer/comparers/datacomparerthread.cpp \
    connectivity/bulkoperationhelper.cpp \
    connection_page/data_comparer/tabs/datacomparerresultstab.cpp \
    beans/datacompareinfo.cpp \
    info_panel/infopanel.cpp \
    beans/dbitemaction.cpp \
    context_menu/contextmenuutil.cpp \
    context_menu/tablecontextmenuutil.cpp \
    context_menu/procedurecontextmenuutil.cpp \
    context_menu/viewcontextmenuutil.cpp \
    connection_page/editorcreatorutil.cpp \
    info_panel/panes/compilermessagespane.cpp \
    connection_page/code_creator/codecreatorwidget.cpp \
    connection_page/code_creator/codecreator.cpp \
    context_menu/functioncontextmenuutil.cpp \
    context_menu/triggercontextmenuutil.cpp \
    navtree/type/dbtypespecitem.cpp \
    navtree/type/dbtypebodyitem.cpp \
    connection_page/code_creator/paircodecreator.cpp \
    context_menu/packagecontextmenuutil.cpp \
    context_menu/typecontextmenuutil.cpp \
    widgets/multieditorwidget.cpp \
    dialogs/gotolinedialog.cpp \
    codeeditor/cursorpositioninfo.cpp \
    connection_page/db_object_creator/dbobjectcreator.cpp \
    connection_page/db_object_creator/dbobjectcreatorpane.cpp \
    connection_page/db_object_creator/dbobjectcreatorbottompane.cpp \
    connection_page/table_creator/tablecreatorpane.cpp \
    context_menu/sequencecontextmenuutil.cpp \
    connection_page/sequence_creator/sequencecreator.cpp \
    connection_page/sequence_creator/sequencecreatorpane.cpp \
    connection_page/db_object_creator/dbobjectcreatorsimplepane.cpp \
    connection_page/dblink_creator/dblinkcreator.cpp \
    connection_page/dblink_creator/dblinkcreatorpane.cpp \
    beans/dblinkinfo.cpp \
    metadata_loaders/dblink/dblinkinfoloader.cpp \
    context_menu/dblinkcontextmenuutil.cpp \
    connection_page/synonym_creator/synonymcreator.cpp \
    connection_page/synonym_creator/synonymcreatorpane.cpp \
    beans/synonyminfo.cpp \
    context_menu/synonymcontextmenuutil.cpp

HEADERS  += mainwindow.h \
    connectdialog.h \
    connectioneditordialog.h \
    connectivity/dbconnection.h \
    mytablewidgetitem.h \
    util/iconutil.h \
    passwordentrydialog.h \
    connectionspane.h \
    connection_page/connectionpage.h \
    navtree/treepane.h \
    connection_page/worksheet/worksheet.h \
    codeeditor/codeeditor.h \
    connection_page/worksheet/worksheetresultpane.h \
    connection_page/worksheet/worksheetquerypane.h \
    navtree/dbtreemodel.h \
    navtree/dbtreeitem.h \
    navtree/table/dbtabletreeitem.h \
    connectivity/connection.h \
    util/strutil.h \
    navtree/table/dbtableparenttreeitem.h \
    connectivity/resultset.h \
    navtree/table/dbtablecolumnparentitem.h \
    navtree/table/dbtablecolumnitem.h \
    util/param.h \
    navtree/table/dbtableconstraintparentitem.h \
    util/queryutil.h \
    connectivity/ociexception.h \
    navtree/table/dbtableconstraintitem.h \
    navtree/table/dbconstraintcolumnitem.h \
    navtree/table/dbtableindexparentitem.h \
    navtree/index/dbindexparentitem.h \
    navtree/index/dbindexitem.h \
    navtree/table/dbtabletriggerparentitem.h \
    navtree/view/dbviewparentitem.h \
    navtree/view/dbviewitem.h \
    navtree/view/dbviewtriggerparentitem.h \
    navtree/package/dbpackageparentitem.h \
    navtree/package/dbpackageitem.h \
    navtree/package/dbpackagespecitem.h \
    navtree/package/dbpackagebodyitem.h \
    navtree/schema/dbschemaitem.h \
    navtree/schema/dballschemasitem.h \
    navtree/procedure/dbprocedureparentitem.h \
    navtree/procedure/dbprocedureitem.h \
    navtree/function/dbfunctionparentitem.h \
    navtree/function/dbfunctionitem.h \
    navtree/sequence/dbsequenceparentitem.h \
    navtree/sequence/dbsequenceitem.h \
    navtree/synonym/dbsynonymparentitem.h \
    navtree/synonym/dbsynonymitem.h \
    navtree/dblink/dbdblinkparentitem.h \
    navtree/dblink/dbdblinkitem.h \
    navtree/type/dbtypeparentitem.h \
    navtree/type/dbtypeitem.h \
    connection_page/dbuimanager.h \
    navtree/nodeaction.h \
    connection_page/table_info/tableinfotoolbar.h \
    connection_page/table_info/tabs/tablecolumnsviewer.h \
    connection_page/table_info/tabs/tabledataviewer.h \
    connection_page/table_info/tabs/tableconstraintsviewer.h \
    connection_page/table_info/tabs/tablegrantsviewer.h \
    connection_page/table_info/tabs/tabletriggersviewer.h \
    connection_page/table_info/tabs/tabledependenciesviewer.h \
    connection_page/table_info/tabs/tabledetailsviewer.h \
    connection_page/table_info/tabs/tablepartitionsviewer.h \
    connection_page/table_info/tabs/tableddlviewer.h \
    widgets/ondemandinfoviewerwidget.h \
    connection_page/table_info/tableinfoviewerwidget.h \
    widgets/datatable.h \
    models/resultsettablemodel.h \
    util/datetimewithtimezone.h \
    widgets/subtabwidget.h \
    widgets/nameeditor.h \
    connection_page/table_creator/tablecreatortab.h \
    connection_page/table_creator/tabs/tablecreatorcolumns.h \
    connection_page/table_creator/tabs/tablecreatorgeneralinfo.h \
    models/genericeditabletablemodel.h \
    delegates/datatypecomboboxdelegate.h \
    delegates/booleandelegate.h \
    connection_page/table_creator/tablecreatortabwithtableview.h \
    connection_page/table_creator/tabs/tablecreatorconstraints.h \
    delegates/schemaselectordelegate.h \
    delegates/comboboxdelegate.h \
    delegates/contrainttypedelegate.h \
    models/tableconstraintsmodel.h \
    delegates/ondeleteactiondelegate.h \
    delegates/indexbasedcomboboxdelegate.h \
    connectivity/dblocalcache.h \
    delegates/tableselectordelegate.h \
    delegates/checkboxlistdelegate.h \
    widgets/checkedcombobox.h \
    util/widgethelper.h \
    enums.h \
    delegates/constraintselectordelegate.h \
    util/dbutil.h \
    dialogs/columnselectordialog.h \
    widgets/lineeditwithbutton.h \
    delegates/columnselectordelegate.h \
    delegates/identifiernamedelegate.h \
    delegates/lineeditwithbuttondelegate.h \
    delegates/plaintexteditordelegate.h \
    dialogs/plaintexteditordialog.h \
    connection_page/table_creator/tabs/tablecreatorcheckconstraints.h \
    connection_page/table_creator/tabs/tablecreatorindexes.h \
    models/tableindexesmodel.h \
    delegates/indexnameeditordelegate.h \
    delegates/numericdelegate.h \
    dialogs/storageparamsdialog.h \
    widgets/storageparamswidget.h \
    widgets/extentsizeeditorwidget.h \
    widgets/tablespacelistcombobox.h \
    beans/storageparams.h \
    connection_page/table_creator/tablecreatortabs.h \
    connection_page/table_creator/tablecreator.h \
    codeeditor/syntaxhighligher.h \
    codeeditor/linenumberarea.h \
    connectivity/queryresult.h \
    connection_page/worksheet/bottom_pane_tabs/worksheetinfotab.h \
    connection_page/worksheet/bottom_pane_tabs/worksheetresultsettab.h \
    connection_page/worksheet/bottom_pane_tabs/worksheetbottompanetab.h \
    widgets/comboboxwithbutton.h \
    dialogs/genericresultsetviewerdialog.h \
    widgets/tableadditionalattributeswidget.h \
    beans/tableadditionalattributes.h \
    dialogs/tableadditionalattributesdialog.h \
    models/tablecolumnsmodel.h \
    widgets/lobparamswidget.h \
    beans/lobparams.h \
    delegates/templatedparamsdelegate.h \
    dialogs/lobparamsdialog.h \
    connection_page/table_creator/tabs/tablecreatorpartitions.h \
    connection_page/table_creator/tabs/tablecreatorgrants.h \
    models/tablegrantsmodel.h \
    widgets/centeredcheckboxwidget.h \
    connection_page/table_creator/tabs/tablecreatorexternalproperties.h \
    widgets/comboboxandlineeditwidget.h \
    widgets/oracleloaderdriverrecordpropertieswidget.h \
    widgets/oracleloaderdriverfieldpropertieswidget.h \
    widgets/tablecreatorexternalpropertiesgeneralinfowidget.h \
    widgets/lineeditandcheckboxwidget.h \
    widgets/tabletoolbar.h \
    widgets/datatableandtoolbarwidget.h \
    delegates/autoappenddelegate.h \
    models/externaltablefieldsmodel.h \
    models/externaltablecolumntransformsmodel.h \
    interfaces/istringlistretriever.h \
    delegates/dynamiccomboboxdelegate.h \
    delegates/abstractcomboboxdelegate.h \
    widgets/oracledatapumpdriverproperties.h \
    widgets/indexorganizedtablepropertieswidget.h \
    dialogs/indexorganizedtablepropertiesdialog.h \
    beans/indexorganizedtableproperties.h \
    util/dialoghelper.h \
    dialogs/descriptiveerrordialog.h \
    navtree/nodepopulatorthread.h \
    navtree/dbtreeitemresult.h \
    navtree/loadingdbtreeitem.h \
    connection_page/connectionpagetab.h \
    connection_page/table_info/tableinfoviewertabs.h \
    connection_page/table_info/tableinfoviewer.h \
    util/asyncconnect.h \
    widgets/tablecreatorpartitionstab.h \
    widgets/lineeditwithautocompleter.h \
    util/queryexectask.h \
    util/queryqueuemanager.h \
    connectivity/recordfetcherthread.h \
    connection_page/table_info/tabs/tablereferencingfkviewer.h \
    connection_page/table_info/tableinfogenerictab.h \
    connectivity/fetchresult.h \
    grammar_parser/codescanner.h \
    grammar_parser/ebnfscanner.h \
    grammar_parser/ebnftoken.h \
    grammar_parser/ebnfparser.h \
    grammar_parser/bnfrule.h \
    grammar_parser/bnfruleitem.h \
    grammar_parser/dfaitem.h \
    grammar_parser/dfastate.h \
    grammar_parser/dfatransition.h \
    grammar_parser/dfa.h \
    connection_page/table_info/tabs/tableindexesviewer.h \
    beans/objectddloptions.h \
    widgets/tableddloptionswidget.h \
    dialogs/tableddloptionsdialog.h \
    connectivity/connectionpool.h \
    models/tablecheckconstraintsmodel.h \
    beans/columninfo.h \
    beans/constraintinfo.h \
    beans/indexinfo.h \
    constants.h \
    beans/grantinfo.h \
    beans/partitioninfo.h \
    connectivity/queryrunner.h \
    beans/querylistitem.h \
    defines.h \
    util/itemcreatorhelper.h \
    delegates/indexstorageparamsdelegate.h \
    widgets/tablecreatorexternalaccessparameterseditorwidget.h \
    beans/tableexternalinfo.h \
    models/partitionsmodel.h \
    beans/partitioninginfo.h \
    beans/tablepartitioninginfo.h \
    dialogs/partitioningparamsdialog.h \
    delegates/partitioningparamsdelegate.h \
    delegates/abstracttemplatedparamsdelegate.h \
    metadata_loaders/table/partitioninfoloader.h \
    widgets/tabwidget.h \
    app_menu/appmenu.h \
    app_menu/appfilemenu.h \
    app_menu/appeditmenu.h \
    app_menu/appviewmenu.h \
    app_menu/apptoolsmenu.h \
    app_menu/apphelpmenu.h \
    interfaces/iqueryscheduler.h \
    widgets/dbitemlistcombobox.h \
    app_menu/appmainmenu.h \
    widgets/codeeditorandsearchpanewidget.h \
    widgets/searchpane.h \
    widgets/closebutton.h \
    app_menu/appsessionmenu.h \
    connection_page/schema_comparer/schemacomparer.h \
    connection_page/schema_comparer/tabs/schemacomparercomparetab.h \
    widgets/connectionselectorwidget.h \
    util/queryscheduler.h \
    widgets/dbtreeview.h \
    navtree/trigger/dbtriggerparentitem.h \
    navtree/trigger/dbtriggeritem.h \
    navtree/index/dbindexcolumnitem.h \
    connection_page/schema_comparer/comparers/schemacomparerhelper.h \
    metadata_loaders/table/tableinfoloader.h \
    beans/tableinfo.h \
    beans/tablegeneralinfo.h \
    metadata_loaders/table/tablegeneralinfoloader.h \
    metadata_loaders/table/tablecolumninfoloader.h \
    metadata_loaders/table/tableconstraintsloader.h \
    metadata_loaders/table/tableindexesloader.h \
    metadata_loaders/table/tableexternalinfoloader.h \
    metadata_loaders/table/tablegrantsloader.h \
    connection_page/schema_comparer/dbobjecthasher.h \
    connection_page/schema_comparer/tabs/schemacompareroptionstab.h \
    connection_page/schema_comparer/tabs/schemacomparerresultstab.h \
    connection_page/schema_comparer/comparers/tablecomparehelper.h \
    code_generators/ddl/tableddlgenerator.h \
    code_generators/ddl/tablecolumnsddlgenerator.h \
    code_generators/ddl/tableconstraintsddlgenerator.h \
    code_generators/ddl/tableindexesddlgenerator.h \
    code_generators/ddl/tablegrantsddlgenerator.h \
    code_generators/ddl/tablepartitionsddlgenerator.h \
    connection_page/schema_comparer/comparers/dbobjectscomparehelper.h \
    connection_page/schema_comparer/comparers/dbobjectcomparehelper.h \
    connection_page/schema_comparer/comparers/dbobjectcomparerfactory.h \
    connection_page/schema_comparer/comparers/sourcecomparehelper.h \
    beans/tablediffddloptions.h \
    beans/tablecreateddloptions.h \
    beans/sourcecodeddloptions.h \
    beans/schemacomparisonoptions.h \
    beans/sequencediffddloptions.h \
    metadata_loaders/sequence/sequenceinfoloader.h \
    beans/dbobjectinfo.h \
    beans/sequenceinfo.h \
    metadata_loaders/metadataloader.h \
    beans/sourceinfo.h \
    metadata_loaders/code/sourceinfoloader.h \
    metadata_loaders/table/tabletriggersloader.h \
    beans/triggerinfo.h \
    code_generators/ddl/tabletriggersddlgenerator.h \
    connection_page/schema_comparer/comparers/sequencecomparehelper.h \
    metadata_loaders/metadataloaderfactory.h \
    beans/sequencecreateddloptions.h \
    beans/newdbobjectddloptions.h \
    connectivity/statement.h \
    beans/statementdesc.h \
    connection_page/data_comparer/datacomparer.h \
    connection_page/db_object_comparer/dbobjectcomparer.h \
    connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.h \
    connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h \
    connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h \
    interfaces/idbobjectcomparehelper.h \
    beans/dbobjectcomparisonoptions.h \
    connection_page/data_comparer/tabs/datacompareroptionstab.h \
    connection_page/data_comparer/tabs/datacomparercomparetab.h \
    interfaces/isearchable.h \
    connection_page/data_comparer/comparers/datacomparerhelper.h \
    beans/datacomparisonoptions.h \
    connection_page/data_comparer/tableinfofordatacomparison.h \
    navtree/idbtreeitemcheckstateprovider.h \
    proxystyle.h \
    connection_page/data_comparer/comparers/datacomparerthread.h \
    connectivity/bulkoperationhelper.h \
    connection_page/data_comparer/tabs/datacomparerresultstab.h \
    beans/datacompareinfo.h \
    info_panel/infopanel.h \
    beans/dbitemaction.h \
    context_menu/contextmenuutil.h \
    context_menu/tablecontextmenuutil.h \
    context_menu/procedurecontextmenuutil.h \
    context_menu/viewcontextmenuutil.h \
    connection_page/editorcreatorutil.h \
    info_panel/panes/compilermessagespane.h \
    connection_page/code_creator/codecreatorwidget.h \
    connection_page/code_creator/codecreator.h \
    context_menu/functioncontextmenuutil.h \
    context_menu/triggercontextmenuutil.h \
    navtree/type/dbtypespecitem.h \
    navtree/type/dbtypebodyitem.h \
    connection_page/code_creator/paircodecreator.h \
    context_menu/packagecontextmenuutil.h \
    context_menu/typecontextmenuutil.h \
    errors.h \
    widgets/multieditorwidget.h \
    dialogs/gotolinedialog.h \
    codeeditor/cursorpositioninfo.h \
    connection_page/db_object_creator/dbobjectcreator.h \
    connection_page/db_object_creator/dbobjectcreatorpane.h \
    connection_page/db_object_creator/dbobjectcreatorbottompane.h \
    connection_page/table_creator/tablecreatorpane.h \
    util/triple.h \
    context_menu/sequencecontextmenuutil.h \
    connection_page/sequence_creator/sequencecreator.h \
    connection_page/sequence_creator/sequencecreatorpane.h \
    connection_page/db_object_creator/dbobjectcreatorsimplepane.h \
    connection_page/dblink_creator/dblinkcreator.h \
    connection_page/dblink_creator/dblinkcreatorpane.h \
    beans/dblinkinfo.h \
    metadata_loaders/dblink/dblinkinfoloader.h \
    context_menu/dblinkcontextmenuutil.h \
    connection_page/synonym_creator/synonymcreator.h \
    connection_page/synonym_creator/synonymcreatorpane.h \
    beans/synonyminfo.h \
    context_menu/synonymcontextmenuutil.h

FORMS    += mainwindow.ui \
    connectdialog.ui \
    connectioneditordialog.ui \
    passwordentrydialog.ui \
    dialogs/columnselectordialog.ui \
    dialogs/plaintexteditordialog.ui

RESOURCES += \
    images.qrc \
    queries.qrc \
    grammars.qrc \
    misc.qrc

#unix:INCLUDEPATH += /home/rovshan/Applications/instantclient_11_2/sdk/include

#unix:LIBS += -L/home/rovshan/Applications/instantclient_11_2 -lociei -lclntsh -locci

unix:INCLUDEPATH += /usr/local/include
unix:LIBS += -L/usr/local/lib -locilib

windows:INCLUDEPATH += "C:/ocilib-3.9.4-windows/ocilib/include"
windows:LIBS += -L"C:/ocilib-3.9.4-windows/ocilib/lib32" -locilibw
