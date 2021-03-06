declare
    l_result varchar2(10000);

    g_owner varchar2(50) := :owner;
    g_target_owner varchar2(50) := :target_owner;
    g_object_type varchar2(50) := :object_type;
    g_object_names varchar2(10000) := :object_names;
    g_compare_hash varchar2(10000) := :compare_hash;
        
    opt_tbl_comments number := :opt_tbl_comments;
    opt_tbl_props number := :opt_tbl_props;
    opt_tbl_fb_archive number := :opt_tbl_fb_archive;
    opt_tbl_storage number := :opt_tbl_storage;
    opt_tbl_iot_props number := :opt_tbl_iot_props;
    opt_tbl_lob_props number := :opt_tbl_lob_props;
    opt_tbl_columns number := :opt_tbl_columns;
    opt_tbl_constraints number := :opt_tbl_constraints;
    opt_tbl_indexes number := :opt_tbl_indexes;
    opt_tbl_parts number := :opt_tbl_parts;
    opt_tbl_ext_props number := :opt_tbl_ext_props;
    opt_tbl_triggers number := :opt_tbl_triggers;
    opt_tbl_grants number := :opt_tbl_grants;
    
    opt_src_ignore_whitespace number := :opt_src_ignore_whitespace;
    opt_src_wrap number := :opt_src_wrap;
    
    opt_seq_check_currval number := :opt_seq_check_currval;
    
    TYPE t_tbl_type IS TABLE OF VARCHAR2(100) INDEX BY BINARY_INTEGER;
    l_table t_tbl_type;
    l_hash_table t_tbl_type;
    
    TYPE t_cursor IS REF CURSOR;
    cur_hash t_cursor;
    l_object_hash varchar2(50);
       
    g_compare_hashes number := 0;
    
    l_ix number;
    
    l_object_exists number;
    
    {@replace_with:func_trim_str}
    
    {@replace_with:func_comma_split}
    
    {@replace_with:func_md5}
    
    {@replace_with:func_get_source_with_new_owner}
    
    {@replace_with:func_create_compare_source_hash}
    
    {@replace_with:func_create_compare_table_hash}
    
    {@replace_with:func_create_compare_sequence_hash}
    
begin
     
     l_table := comma_split(g_object_names);
       
     if g_compare_hash is not null then
       g_compare_hashes := 1;
       l_hash_table := comma_split(g_compare_hash);      
     end if;
     
     l_ix := l_table.first;
     loop
        exit when l_ix is null;
        
        select count(0) into l_object_exists from dual
        where exists (select 0 from sys.all_objects where owner=g_owner and 
        object_name=l_table(l_ix) and object_type=DECODE(g_object_type,'PACKAGE SPEC','PACKAGE','TYPE SPEC', 'TYPE', g_object_type));
        
        if l_object_exists > 0 then
        
           if g_object_type = 'TABLE' then
              if g_compare_hashes = 1 then
                l_result := l_result || compare_table_hash(g_owner, l_table(l_ix), l_hash_table(l_ix)) || ',';
              else
                l_result := l_result || create_table_hash(g_owner, l_table(l_ix)) || ',';
              end if;
           elsif (g_object_type in ('VIEW','PACKAGE SPEC','PACKAGE BODY','PROCEDURE','FUNCTION','TYPE SPEC','SYNONYM')) then
              if g_compare_hashes = 1 then
                l_result := l_result || compare_source_hash(g_object_type, g_owner, l_table(l_ix), g_target_owner, l_hash_table(l_ix), opt_src_ignore_whitespace, opt_src_wrap) || ',';
              else
                l_result := l_result || create_source_hash(g_object_type, g_owner, l_table(l_ix), g_target_owner, opt_src_ignore_whitespace, opt_src_wrap) || ',';
              end if;
           elsif g_object_type = 'SEQUENCE' then
              if g_compare_hashes = 1 then
                l_result := l_result || compare_sequence_hash(g_owner, l_table(l_ix), l_hash_table(l_ix)) || ',';
              else
                l_result := l_result || create_sequence_hash(g_owner, l_table(l_ix)) || ',';
              end if;
           else
              l_result := l_result || 'unsupported_object_type,';
           end if;
           
        elsif l_object_exists = 0 and g_compare_hashes = 1 then
            l_result := l_result || 'new,';
        else
            l_result := l_result || 'not_found,';
        end if;
        
       l_ix := l_table.next(l_ix);
     end loop;

     open :rs_out for select decode(l_result, null, null, substr(l_result,1,length(l_result)-1)) from dual;
     --dbms_output.put_line(l_result);
end;
