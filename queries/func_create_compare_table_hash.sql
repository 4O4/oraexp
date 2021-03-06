    function create_table_hash(p_owner varchar2, p_table_name varchar2) return varchar2 is
       l_hash varchar2(50);
       l_tmp_md5 varchar2(50);
       l_tmp_str varchar2(5000);
       l_comments varchar2(4000);
       
       l_ix_columns clob; 
       l_col_expression long;  
       
       l_is_partitioned number := 0;
       l_is_iot number := 0;
       l_is_external number := 0;
    begin
       --create hash for general info
       for cur_general_info in (select sys.all_tables.*, 
                                case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'YES'
                                else 'NO' end as is_external 
                                {@keep_if:>=11.2}
                                 , NVL(fb.status, 'DISABLED') as flashback_status
                                {}
                                {@keep_if:==11.1}
                                 , DECODE(fb.flashback_archive_name, null, 'DISABLED', 'ENABLED') as flashback_status
                                {}
                                {@keep_if:>=11}
                                , fb.flashback_archive_name, fb.archive_table_name 
                                {}
                                from sys.all_tables 
                                {@keep_if:>=11} 
                                    left join sys.user_flashback_archive_tables fb 
                                    on sys.all_tables.owner=fb.owner_name and sys.all_tables.table_name=fb.table_name 
                                {}
                                where sys.all_tables.owner=p_owner and sys.all_tables.table_name=p_table_name)
       loop
          l_tmp_str := cur_general_info.duration||cur_general_info.iot_type||cur_general_info.is_external;
          
          if opt_tbl_parts = 1 then
            l_tmp_str := l_tmp_str||cur_general_info.partitioned;
          end if;
          
          if opt_tbl_props = 1 then 
            l_tmp_str := l_tmp_str||cur_general_info.degree||cur_general_info.logging||
                         cur_general_info.cache||cur_general_info.monitoring||cur_general_info.compression||
                         cur_general_info.dependencies||cur_general_info.row_movement;
          end if;
          
          if opt_tbl_storage = 1 then
            l_tmp_str := l_tmp_str||cur_general_info.pct_free||cur_general_info.pct_used||
                         cur_general_info.ini_trans||cur_general_info.max_trans||cur_general_info.pct_increase||
                         cur_general_info.max_extents||cur_general_info.freelists||cur_general_info.freelist_groups||
                         cur_general_info.buffer_pool;
          end if;
                                                               
          {@keep_if:>=11}
          if opt_tbl_fb_archive = 1 then
             l_tmp_str := l_tmp_str||cur_general_info.flashback_status||cur_general_info.flashback_archive_name;
          end if;
          {}
          
          if cur_general_info.iot_type = 'IOT' then
            l_is_iot := 1;
          end if;
          
          if trim_str(cur_general_info.partitioned) = 'YES' then
            l_is_partitioned := 1;
          end if;
          
          if cur_general_info.is_external = 'YES' then
            l_is_external := 1;
          end if;
                       
          l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
       end loop;
       
       --create hash for IOT properties
       if opt_tbl_iot_props = 1 and l_is_iot = 1 then 
         for cur_iot in (select nvl(ai.prefix_length, -1) as prefix_length, 
                          ai.pct_threshold, 
                          (select column_name from sys.all_tab_columns cols where cols.owner=ai.owner and cols.table_name=ai.table_name 
                           and cols.column_id=ai.include_column) as include_col_name,
                          (select table_name from sys.all_tables atab1 where 
                           atab1.owner=ai.owner and atab1.iot_name=ai.table_name and atab1.iot_type='IOT_OVERFLOW') as overflow_table_name,
                           (select logging from sys.all_tables atab2 where 
                           atab2.owner=ai.owner and atab2.iot_name=ai.table_name and atab2.iot_type='IOT_OVERFLOW') as overflow_logging,
                            case when exists (select 0 from sys.all_tables atab3 where atab3.owner=ai.owner and 
                                       atab3.iot_name=ai.table_name and atab3.iot_type='IOT_MAPPING') then 1 
                             else 0 end as has_mapping_table
                          from sys.all_indexes ai 
                          where (ai.owner, ai.index_name) in 
                          (select DECODE(index_owner, NULL, owner, index_owner), index_name from sys.all_constraints where owner=p_owner and table_name=p_table_name
                          and constraint_type='P')
                          and index_type='IOT - TOP')
         loop
           l_tmp_str := cur_iot.prefix_length||cur_iot.pct_threshold||cur_iot.include_col_name||cur_iot.overflow_logging||cur_iot.has_mapping_table; 
           l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
         end loop;
       end if;
    
       --create hash for comments
       if opt_tbl_comments = 1 then 
           begin
             select comments into l_comments from sys.all_tab_comments where 
             owner=p_owner and table_name=p_table_name and table_type='TABLE';
             
             l_tmp_md5 := md5(l_tmp_md5 || md5(l_comments));
           exception when no_data_found then
             null; --just ignore
           end;
       end if;
       
       --for columns
       l_tmp_str := '';
       if opt_tbl_columns = 1 then
           for cur_tab_columns in (select cols.column_name, data_type || 
                                    case when char_used='C' and data_type not like 'N%' then ' ('||char_length||' CHAR)'
                                         when char_used='C' and data_type like 'N%' then ' ('||char_length||')'
                                         when char_used='B' and data_type not like 'N%' then ' ('||char_length||' BYTE)'
                                         when char_used='B' and data_type like 'N%' then ' ('||char_length||')'
                                         else ''
                                    end ||
                                    decode(data_type, 'NUMBER', 
                                         case when data_precision is not null and data_scale is null then ' ('||to_char(data_precision)||')'
                                              when data_precision is null and data_scale is not null then ' (*, '||to_char(data_scale)||')'
                                              when data_precision is not null and data_scale is not null then ' ('||to_char(data_precision)||', '||to_char(data_scale)||')'
                                         else '' end,
                                         'RAW', '('||data_length||')',
                                         '')
                                    as data_type,
                                    decode(nullable, 'Y', 'Yes', 'No') as nullable,
                                    data_default,
                                    virtual_column as virtual,
                                    sys.all_col_comments.comments
                                    from sys.all_tab_cols cols left join sys.all_col_comments on 
                                        (cols.owner=sys.all_col_comments.owner and 
                                        cols.table_name=sys.all_col_comments.table_name and
                                        cols.column_name=sys.all_col_comments.column_name)
                                    where cols.owner=p_owner and cols.table_name=p_table_name and hidden_column='NO'
                                    order by column_id)
           loop
              l_tmp_str := cur_tab_columns.column_name || cur_tab_columns.data_type || cur_tab_columns.nullable || 
                           md5(trim_str(cur_tab_columns.data_default)) || cur_tab_columns.virtual;
                           
              if opt_tbl_comments = 1 then 
     			 l_tmp_str := l_tmp_str||md5(cur_tab_columns.comments);
			  end if;
                                        
              if opt_tbl_lob_props = 1 and cur_tab_columns.data_type in ('CLOB','BLOB','NCLOB') then
                    for cur_lob_props in (select pctversion, decode(sys.all_lobs.retention,null,0,1) as retention, freepools, 
                                            cache, logging, s.freelists, buffer_pool {@keep_if:>=11} , s.max_size*s.bytes/s.blocks as max_size  {}
                                            from sys.all_lobs left join sys.user_segments s on (sys.all_lobs.segment_name=s.segment_name and s.segment_type='LOBSEGMENT')
                                            where sys.all_lobs.owner=p_owner and sys.all_lobs.table_name=p_table_name
                                            and sys.all_lobs.column_name=cur_tab_columns.column_name)
                    loop
                        l_tmp_str := l_tmp_str||cur_lob_props.pctversion||cur_lob_props.retention||nvl(cur_lob_props.freepools,1)||cur_lob_props.cache||
                                     cur_lob_props.logging;
                                     
                        if opt_tbl_storage = 1 then
                            l_tmp_str := l_tmp_str||cur_lob_props.freelists||cur_lob_props.buffer_pool {@keep_if:>=11} || cur_lob_props.max_size {} ;
                        end if;
                    end loop;
              end if;
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
              
           end loop;
      end if;
       
       
       --for constraints
      if opt_tbl_constraints = 1 then
          for cur_cons in (select constraint_name, 
                            decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 'C', 3, 4) as cons_type, 
                            (
                            select col_names from (
                            SELECT constraint_name,
                                   LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
                                   KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
                            FROM   (SELECT constraint_name,
                                           column_name,
                                           ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) AS curr,
                                           ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) -1 AS prev
                                    FROM   sys.all_cons_columns WHERE owner=p_owner and table_name=p_table_name)
                            GROUP BY constraint_name
                            CONNECT BY prev = PRIOR curr AND constraint_name = PRIOR constraint_name
                            START WITH curr = 1) t where constraint_name=ac.constraint_name
                            ) as column_names,
                            r_owner, 
                            (select table_name from sys.all_constraints where owner=ac.r_owner and constraint_name=ac.r_constraint_name) as r_table_name,
                            delete_rule, deferrable, deferred, generated
                            from sys.all_constraints ac
                            where owner=p_owner and table_name=p_table_name and constraint_type in ('P','R','U') 
                            order by decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 3), constraint_name)
           loop
              l_tmp_str := cur_cons.cons_type || cur_cons.column_names || cur_cons.r_table_name ||
                           cur_cons.delete_rule || cur_cons.deferrable || cur_cons.deferred;
                           
              if cur_cons.r_owner != p_owner then
                 l_tmp_str := l_tmp_str || cur_cons.r_owner;
              end if;
              
              if cur_cons.generated != 'GENERATED NAME' then
                 l_tmp_str := l_tmp_str || cur_cons.constraint_name;
              end if;
                           
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
           end loop;
           
           --for checks
            for cur_checks in (select constraint_name, 
                              decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 'C', 3, 4) as cons_type, 
                              (
                              select col_names from (
                              SELECT constraint_name,
                                     LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
                                     KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
                              FROM   (SELECT constraint_name,
                                             column_name,
                                             ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) AS curr,
                                             ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) -1 AS prev
                                      FROM   sys.all_cons_columns WHERE owner=p_owner and table_name=p_table_name)
                              GROUP BY constraint_name
                              CONNECT BY prev = PRIOR curr AND constraint_name = PRIOR constraint_name
                              START WITH curr = 1) t where constraint_name=ac.constraint_name
                              ) as column_names, search_condition, deferrable, deferred, generated
                              from sys.all_constraints ac
                              where owner=p_owner and table_name=p_table_name and constraint_type = 'C' 
                              order by constraint_name)
            loop
              if not (trim_str(cur_checks.search_condition) = '"'||cur_checks.column_names||'" IS NOT NULL' and cur_checks.generated = 'GENERATED NAME') then
                  l_tmp_str := cur_checks.cons_type || cur_checks.column_names || md5(cur_checks.search_condition) || cur_checks.deferrable || cur_checks.deferred;
                               
                  if cur_checks.generated != 'GENERATED NAME' then
                     l_tmp_str := l_tmp_str || cur_checks.constraint_name;
                  end if;
                               
                  l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
              end if;
            end loop;
        end if;
        
        --for indexes
        if opt_tbl_indexes = 1 then
            for cur_indexes in (select ai.*, pi.locality
                                from sys.all_indexes ai left join sys.all_part_indexes pi
                                on (ai.owner=pi.owner and ai.index_name=pi.index_name and ai.table_name=pi.table_name)
                                where ai.table_owner=p_owner and ai.table_name=p_table_name and ai.table_type='TABLE'
                                and ai.generated='N'
                                and not exists (select 0 from sys.all_constraints ac where owner=ai.owner and table_name=ai.table_name and index_name=ai.index_name)
                                order by ai.index_name)
            loop
              l_ix_columns := null;
              l_tmp_str := cur_indexes.index_type || cur_indexes.uniqueness || cur_indexes.compression || cur_indexes.prefix_length;
              if cur_indexes.generated != 'GENERATED NAME' then
                 l_tmp_str := l_tmp_str || cur_indexes.index_name;
              end if;
              
              if opt_tbl_storage = 1 then
                 l_tmp_str := l_tmp_str || cur_indexes.max_trans;
              end if;
                           
              for cc in (select column_name, column_position
                   from sys.all_ind_columns aic where 
                   index_owner=cur_indexes.owner and index_name=cur_indexes.index_name and 
                   table_owner=cur_indexes.table_owner and table_name=cur_indexes.table_name
                   order by column_position)
              loop
                  begin
                   select column_expression into l_col_expression from sys.all_ind_expressions
                   where index_owner=cur_indexes.owner and index_name=cur_indexes.index_name and 
                   table_owner=cur_indexes.table_owner and table_name=cur_indexes.table_name and
                   column_position=cc.column_position;
                  exception when no_data_found then
                   l_col_expression := null;
                  end;
          
                  l_ix_columns := l_ix_columns || nvl(trim_str(l_col_expression), cc.column_name) || ',';
              end loop;
        
              l_ix_columns := substr(l_ix_columns, 1, length(l_ix_columns)-1);
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str) || md5(l_ix_columns));
            end loop;
        end if;
              
              
        --for partitions
        if opt_tbl_parts = 1 and l_is_partitioned = 1 then 
            for cur_parts in (select s.partition_name, s.subpartition_name, p.high_value as p_high, s.high_value as subp_high, 
                              p.partition_position, s.subpartition_position, s.tablespace_name 
                              from sys.all_tab_subpartitions s inner join sys.all_tab_partitions p 
                              on (s.table_owner=p.table_owner and s.table_name=p.table_name and s.partition_name=p.partition_name)
                              where s.table_owner=p_owner and s.table_name=p_table_name
                              order by p.partition_position, s.subpartition_position)
            loop
              l_tmp_str := md5(cur_parts.p_high) || md5(cur_parts.subp_high);
              
              if cur_parts.partition_name not like 'SYS\_%' escape '\' then
                l_tmp_str := l_tmp_str || cur_parts.partition_name;
              end if;
              
              if cur_parts.subpartition_name not like 'SYS\_%' escape '\' then
                l_tmp_str := l_tmp_str || cur_parts.subpartition_name;
              end if;
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
            
            --for subpartition template
            for cur_part_tpl in (select subpartition_name, subpartition_position, high_bound as high_value 
                                  from sys.all_subpartition_templates
                                  where user_name=p_owner and table_name=p_table_name
                                  order by subpartition_position)
            loop
              l_tmp_str := md5(cur_part_tpl.high_value);
              
              if cur_part_tpl.subpartition_name not like 'SYS\_%' escape '\' then
                l_tmp_str := l_tmp_str || cur_part_tpl.subpartition_name;
              end if;
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
        end if;
        
        --for external table info
        if opt_tbl_ext_props = 1 and l_is_external = 1 then
            for cur_ext in (select et.*, at.degree from sys.all_external_tables et
                            inner join sys.all_tables at on et.owner=at.owner and et.table_name=at.table_name
                            where et.owner=p_owner and et.table_name=p_table_name)
            loop
              l_tmp_str := cur_ext.type_name||cur_ext.default_directory_owner||cur_ext.default_directory_name||
                           cur_ext.reject_limit||trim_str(cur_ext.degree)||md5(cur_ext.access_parameters);             
                           
              --for Oracle 10g or later
              --l_tmp_str := l_tmp_str || cur_ext.property;
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
            
            
            for cur_ext_loc in (select '"'||l.directory_owner||'"."'||l.directory_name||'"' as directory_name,
                                l.location from sys.all_external_locations l where l.owner=p_owner and l.table_name=p_table_name
                                order by l.directory_owner, l.directory_name, l.location)
            loop
              l_tmp_str := cur_ext_loc.directory_name||cur_ext_loc.location;
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
        end if;
        
        --for triggers
        if opt_tbl_triggers = 1 then
            for cur_triggers in (select * from sys.all_triggers where
                                table_owner=p_owner and table_name=p_table_name and base_object_type='TABLE' order by owner, trigger_name)
            loop
              l_tmp_str := cur_triggers.trigger_name||cur_triggers.trigger_type||cur_triggers.triggering_event||cur_triggers.column_name||
                           cur_triggers.referencing_names||cur_triggers.when_clause||cur_triggers.status||
                           cur_triggers.action_type||md5(trim_str(cur_triggers.trigger_body));
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
        end if;
        
        --for grants
        if opt_tbl_grants = 1 then
            for cur_grants in (select grantee, privilege, grantable from sys.all_tab_privs where table_schema=p_owner and table_name=p_table_name
                               order by grantee, privilege, grantable)
            loop
              l_tmp_str := cur_grants.grantee || cur_grants.privilege || cur_grants.grantable;
              
              l_tmp_md5 := md5(l_tmp_md5 || md5(l_tmp_str));
            end loop;
        end if;
       
       l_hash := l_tmp_md5;
       return l_hash;
    end;
    
    function compare_table_hash(p_owner varchar2, p_table_name varchar2, p_hash varchar2) return varchar2 is
    begin      
      if p_hash = create_table_hash(p_owner, p_table_name) then
        return 'same';
      else
        return 'different';
      end if;
    end;
