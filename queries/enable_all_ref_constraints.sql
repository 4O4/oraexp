BEGIN
  FOR c IN
  (SELECT c.owner, c.table_name, c.constraint_name
   FROM sys.all_constraints c, sys.all_tables t
   WHERE c.table_name = t.table_name and t.owner='{owner}' 
   AND c.status = 'DISABLED' and c.constraint_type='R')
  LOOP
    dbms_utility.exec_ddl_statement('alter table "' || c.owner || '"."' || c.table_name || '" enable constraint ' || c.constraint_name);
  END LOOP;
END;
