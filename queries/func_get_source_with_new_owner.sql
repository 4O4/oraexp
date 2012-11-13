    {@keep_if:>=10}
        {@replace_with:func_wrap_code}
    {}

    {@replace_with:func_clob_replace}
    {@replace_with:func_replace_source_owner}
    {@replace_with:func_trim_str}

    function get_source_with_new_owner(p_object_type varchar2, p_owner varchar2, p_object_name varchar2, p_new_owner varchar2, 
                                       p_remove_whitespace number := 0, p_wrap number := 0, p_sql_terminator number := 1) return clob is
      l_code clob;
      l_will_wrap number := 0;
    begin
         {@keep_if:>=10}
         if p_wrap = 1 and p_object_type in ('PACKAGE BODY', 'FUNCTION', 'PROCEDURE') then
            l_will_wrap := 1;
         end if;
         {}
    
         l_code := trim_str(DBMS_METADATA.GET_DDL (replace(p_object_type, ' ', '_'), p_object_name, p_owner));
                  
         replace_source_owner(p_owner, p_new_owner, p_object_name, p_object_type, l_code);
         
         if l_will_wrap = 0 and p_remove_whitespace = 1 then
            l_code := clob_replace(clob_replace(clob_replace(l_code, ' '), chr(10)), chr(13));
         end if;
         
         {@keep_if:>=10}
         if l_will_wrap = 1 then
            l_code := wrap_code(l_code);
         end if;
         {}
      
         return l_code;
    end;