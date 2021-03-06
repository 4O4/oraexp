select s.partition_name, s.subpartition_name, s.high_value, 
--p.partition_position, 
s.subpartition_position as partition_position, s.tablespace_name 
from sys.all_tab_subpartitions s inner join sys.all_tab_partitions p 
on (s.table_owner=p.table_owner and s.table_name=p.table_name and s.partition_name=p.partition_name)
where s.table_owner=:owner and s.table_name=:object_name
order by p.partition_position, s.subpartition_position
