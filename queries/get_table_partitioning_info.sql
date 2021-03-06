select partitioning_type, 
(
select col_names from (
SELECT name,
       LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
       KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
FROM   (SELECT name,
               column_name,
               ROW_NUMBER() OVER (PARTITION BY name ORDER BY column_position) AS curr,
               ROW_NUMBER() OVER (PARTITION BY name ORDER BY column_position) -1 AS prev
        FROM   sys.all_part_key_columns WHERE owner=:owner and name=:object_name and object_type='TABLE')
GROUP BY name
CONNECT BY prev = PRIOR curr AND name = PRIOR name
START WITH curr = 1) t where name=t.table_name
) as partition_key_columns,
subpartitioning_type, 
(
select col_names from (
SELECT name,
       LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
       KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
FROM   (SELECT name,
               column_name,
               ROW_NUMBER() OVER (PARTITION BY name ORDER BY column_position) AS curr,
               ROW_NUMBER() OVER (PARTITION BY name ORDER BY column_position) -1 AS prev
        FROM   sys.all_subpart_key_columns WHERE owner=:owner and name=:object_name and object_type='TABLE')
GROUP BY name
CONNECT BY prev = PRIOR curr AND name = PRIOR name
START WITH curr = 1) t where name=t.table_name
) as subpartition_key_columns, (select count(0) from sys.all_tab_partitions where owner=t.owner and table_name=t.table_name) as partition_count
{@keep_if:>=11} , interval {}
from sys.all_part_tables t where owner=:owner and table_name=:object_name
