select object_name as type_name, status from sys.all_objects
where object_type='TYPE BODY' and object_name=:object_name and owner=:owner order by object_name