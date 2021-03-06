FUNCTION md5 (p_str clob) RETURN VARCHAR2
      IS
         l_hash   VARCHAR2 (50) := ' ';
         l_ix number := 1;
         l_length number;
         c_max_varchar_length constant number := 2000;
      BEGIN
         if p_str is null then 
           return null; 
         end if;
         
         l_length := length(p_str);
         if l_length>c_max_varchar_length then
            while l_ix<l_length
            loop
              l_hash := md5(l_hash || md5(substr(p_str, l_ix, c_max_varchar_length)));     
              
              l_ix := l_ix + c_max_varchar_length;
            end loop;
         else
            l_hash := RAWTOHEX(UTL_RAW.cast_to_raw(DBMS_OBFUSCATION_TOOLKIT.md5 (input_string => p_str)));
         end if;
         
         RETURN l_hash;
      END;