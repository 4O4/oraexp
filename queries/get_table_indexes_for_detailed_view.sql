select owner, index_name, null as columns, index_type,
status, uniqueness, compression, tablespace_name, prefix_length, ini_trans,
max_trans, initial_extent, next_extent, min_extents, max_extents, pct_increase,
pct_threshold, pct_free, logging, blevel, leaf_blocks, distinct_keys, degree, instances, 
partitioned, temporary, generated, secondary, buffer_pool, user_stats, funcidx_status, join_index, last_analyzed, 
'user' as owner_icon_column,
'index' as ind_icon_column,
'column' as col_icon_column
from sys.all_indexes ai
where table_owner=:owner and table_name=:object_name and table_type='TABLE'
order by index_name
