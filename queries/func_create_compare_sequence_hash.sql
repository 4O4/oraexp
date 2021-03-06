    function create_sequence_hash(p_owner varchar2, p_sequence_name varchar2) return varchar2 is
       l_hash varchar2(50);
       l_tmp_md5 varchar2(50);
       l_tmp_str varchar2(5000);
    begin
       for cur_seq in (select * from sys.all_sequences where sequence_owner=p_owner and sequence_name=p_sequence_name)
       loop
          l_tmp_str := cur_seq.min_value||cur_seq.max_value||cur_seq.increment_by||cur_seq.cycle_flag||cur_seq.order_flag||cur_seq.cache_size;

          if opt_seq_check_currval = 1 then
            l_tmp_str := l_tmp_str || cur_seq.last_number;
          end if;
                            
          l_tmp_md5 := md5(l_tmp_str);
       end loop;
       
       l_hash := l_tmp_md5;
       return l_hash;
    end;
    
    function compare_sequence_hash(p_owner varchar2, p_sequence_name varchar2, p_hash varchar2) return varchar2 is
    begin      
      if p_hash = create_sequence_hash(p_owner, p_sequence_name) then
        return 'same';
      else
        return 'different';
      end if;
    end;
