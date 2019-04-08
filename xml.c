#include "xml.h"

res_t xs_skip_header(char_stream_t *s){
  char from[2] = "<?";
  char to[2] = "?>";
  char_stream_t from_str;
  char_stream_t to_str;
  expect(cs_build_from_buffer(&from_str, from, 2)); //TODO: do not make them all over again all the times
  expect(cs_build_from_buffer(&to_str, to, 2));
  return cs_bounded_skip(s, from_str, to_str, TRUE);
}

bool_t xs_is_header(char_stream_t s){
  char start_of_header[2] = "<?";
  char_stream_t header_stream;
  expect(cs_build_from_buffer(&header_stream, start_of_header, 2)); 
  return cs_is_sequence(&s, &header_stream, TRUE);
}

res_t xs_get_tag_inplace(char_stream_t *s, char_stream_t *output, tag_type_t *tag_type){
  char start_of_tag[2] = "<";
  char start_of_close_tag[3] = "</";
  char end_of_tag[2] = ">";
  char_stream_t sot_stream; //decide start of start_close
  char_stream_t eot_stream;
  char c;
  if(xs_is_close_tag(s)){
    cs_build_from_buffer(&sot_stream, start_of_close_tag, 2);
    *tag_type = close; 
  }else{
    cs_build_from_buffer(&sot_stream, start_of_tag, 1);
    *tag_type = open;
  }
  cs_build_from_buffer(&eot_stream, end_of_tag, 1);
  expect(cs_bounded_get_inplace(s, output, sot_stream, eot_stream));
  expect(cs_trim_end_spaces(s));
  expect(cs_char_at(s,-1,&c));
  if(c == '/'){
    *tag_type = self_conclusive;
  }
  return TRUE;
}

res_t xs_get_value_inplace(char_stream_t *s, char_stream_t *out){
  unsigned int size =0;
  backup_t bk;
  out->buffer = (s->buffer + s->position) ; //assume value starts here
  while(!cs_end_of(s) && !xs_is_close_tag(s)){
    size++;
    cs_advance(s);
  }
  out->size = size;
  return TRUE;
}

bool_t xs_is_open_tag(char_stream_t *s){
  char normal_open_tag[2] = "<";
  char close_open_tag[3] = "</";
  char_stream_t not_stream;
  char_stream_t cot_stream;
  cs_build_from_buffer(&not_stream, normal_open_tag, 1);
  cs_build_from_buffer(&cot_stream, close_open_tag, 2);
  expect(!cs_is_sequence(s, &cot_stream, TRUE));
  expect(cs_is_sequence(s, &not_stream, TRUE));
  return TRUE;
}

bool_t xs_is_close_tag(char_stream_t *s){
  char close_open_tag[3] = "</";
  char_stream_t cot_stream;
  cs_build_from_buffer(&cot_stream, close_open_tag, 2);
  expect(cs_is_sequence(s, &cot_stream, TRUE));
  return TRUE; 
}

bool_t xt_is_value_node(xml_tree_t *t){
  return !!t->tag_value;
}

bool_t xt_is_valid_node(xml_tree_t *t){
  expect(t);
  expect(t->tag_name || t->tag_value);
  return TRUE;
}

res_t xt_work_with_tag(xml_tree_t *t, char_stream_t *s){
  char_stream_t tmp;
  tag_type_t tag_type;
  bool_t has_to_reset = FALSE; //if tags are ordered, this serves no purpose
  bool_t one_ok = FALSE;
  bool_t ordered;
  expect(xs_get_tag_inplace(s, &tmp, &tag_type));
  expect(tag_type == open);
  expect(cs_equal_streams(t->tag_name, &tmp));
  //we found the correct tag
  do{
    for(int i = 0; i<t->children_count; i++){
      ordered = xt_parse_xml_into_tree((t->children + i), s);
      if(!ordered){
        has_to_reset = TRUE;
      }else{
        one_ok = TRUE;
      }
    }
  }while(has_to_reset && one_ok);
  expect(xs_get_tag_inplace(s,&tmp, &tag_type));
  expect(tag_type == close);
  expect(cs_equal_streams(t->tag_name, &tmp));
  return TRUE;
}

// |->| is where we are
res_t xt_parse_xml_into_tree(xml_tree_t *t, char_stream_t *s){
  expect(xt_is_valid_node(t));
  if(xt_is_value_node(t)){ //<tag>|->|value</tag>
    if(xs_is_open_tag(s)){ //enum <tag/>
      char c;
      tag_type_t tag_type = none;
      xs_get_tag_inplace(s, t->tag_value,&tag_type);
      expect(tag_type == self_conclusive); //if it is not self conclusive we are in the wrong place
      expect(cs_trim_end_spaces(t->tag_value)); //the / MUST be last char b4 >
      expect(cs_char_at(t->tag_value, -1, &c));
      expect(c == '/');
      t->tag_value->size-=1; //remove the / and we gucci
    }else{
      expect(xs_get_value_inplace(s, t->tag_value));
    }
  }else{ //|->|<tag>...</tag>
    expect(xt_work_with_tag(t, s));
  }
  return TRUE;
}
