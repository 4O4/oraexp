select column_name, table_owner, table_name, null as data_type 
from sys.all_ind_columns where index_owner=:owner and index_name=:index_name order by column_position
