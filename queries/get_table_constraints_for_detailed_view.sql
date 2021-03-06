select constraint_name, 
decode(constraint_type, 'P', 'Primary key', 'R', 'Foreign key', 'U', 'Unique key', 'C', 'Check', constraint_type) as type, 
(
select col_names from (
SELECT constraint_name,
       LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
       KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
FROM   (SELECT constraint_name,
               column_name,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) AS curr,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) -1 AS prev
        FROM   sys.all_cons_columns WHERE owner=:owner and table_name=:object_name)
GROUP BY constraint_name
CONNECT BY prev = PRIOR curr AND constraint_name = PRIOR constraint_name
START WITH curr = 1) t where constraint_name=ac.constraint_name
) as columns,
search_condition, r_owner, 
(select table_name from sys.all_constraints where owner=ac.r_owner and constraint_name=ac.r_constraint_name) as r_table_name,
r_constraint_name, 
delete_rule, status, deferrable, deferred, validated, generated, bad, rely, last_change,
index_owner, index_name, invalid, view_related,

decode(constraint_type, 'P', 'constraint_pk', 'R', 'constraint_fk', 'U', 'constraint_uq', 'C', 'constraint_ch', constraint_type) as type_icon

from sys.all_constraints ac
where owner=:owner and table_name=:object_name
order by decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 3), constraint_name

