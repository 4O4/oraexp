select * from sys.all_plsql_object_settings
where owner=:owner and name=:object_name and type=:object_type