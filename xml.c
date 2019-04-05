#include "xml.h"

typedef enum token_type_token {
  value,
  tag,
  comment, 
  header
} token_type_t;

res_t skip_header(char_stream_t *s){
  char from[2] = "<?";
  char to[2] = "?>";
  char_stream_t from_str;
  char_stream_t to_str;
  expect(build_from_buffer(&from_str, from, 2)); //TODO: do not make them all over again all the times
  expect(build_from_buffer(&to_str, to, 2));
  return bounded_skip(s, from_str, to_str, TRUE);
}

bool_t is_header(char_stream_t s){
  char start_of_header[2] = "<?";
  char_stream_t header_stream;
  expect(build_from_buffer(&header_stream, start_of_header, 2)); 
  return is_sequence(&s, &header_stream, TRUE);
}

res_t get_tag_inplace(char_stream_t *s, char_stream_t *output){
  char start_of_tag[2] = "<";
  char start_of_close_tag[3] = "</";
  char end_of_tag[2] = ">";
  char_stream_t sot_stream; //decide start of start_close
  char_stream_t eot_stream;
  if(is_close_tag(s)){
    build_from_buffer(&sot_stream, start_of_close_tag, 2);
  }else{
    build_from_buffer(&sot_stream, start_of_tag, 1);
  }
  build_from_buffer(&eot_stream, end_of_tag, 1);
  expect(bounded_get_inplace(s, output, sot_stream, eot_stream));
  return TRUE;
}

res_t get_value_inplace(char_stream_t *s, char_stream_t *out){
  unsigned int size =0;
  backup_t bk;
  out->buffer = (s->buffer + s->position) ; //assume value starts here
  while(!end_of(s) && !is_close_tag(s)){
    size++;
    advance(s);
  }
  out->size = size;
  return TRUE;
}

bool_t is_open_tag(char_stream_t *s){
  char normal_open_tag[2] = "<";
  char close_open_tag[3] = "</";
  char_stream_t not_stream;
  char_stream_t cot_stream;
  build_from_buffer(&not_stream, normal_open_tag, 1);
  build_from_buffer(&cot_stream, close_open_tag, 2);
  expect(!is_sequence(s, &cot_stream, TRUE));
  expect(is_sequence(s, &not_stream, TRUE));
  return TRUE;
}

bool_t is_close_tag(char_stream_t *s){
  char close_open_tag[3] = "</";
  char_stream_t cot_stream;
  build_from_buffer(&cot_stream, close_open_tag, 2);
  expect(is_sequence(s, &cot_stream, TRUE));
  return TRUE; 
}

res_t is_tag_in_tree(xml_tree_t *tree, char_stream_t *s){

}

res_t parse_xml_into_tree(xml_tree_t *tree, char_stream_t *s){
  if(is_open_tag(s)){

  }
}
