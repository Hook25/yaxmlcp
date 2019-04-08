#include "streams.h"
#include <ctype.h>

bool_t cs_valid_position(char_stream_t *_s){
  return _s->buffer && _s->position >= 0 && _s->position < _s->size;
}

res_t cs_get_char(char_stream_t *_s, char *c){
  expect(cs_valid_position(_s));
  *c = _s->buffer[_s->position];
  return TRUE;  
}

res_t cs_set_char(char_stream_t *_s, char c){
  expect(cs_valid_position(_s));
  _s->buffer[_s->position] = c;
  return TRUE;
}

res_t cs_advance(char_stream_t *_s){
  _s->position++;
  return cs_valid_position(_s);
}

res_t cs_reset(char_stream_t *_s){
  _s->position = 0;
  return TRUE;
}

bool_t cs_end_of(char_stream_t *_s){
  return _s->position == _s->size;
}

res_t cs_build_from_buffer(char_stream_t *_s, char *buffer, unsigned int str_len){
 expect(_s && buffer);
 _s->buffer = buffer;
 _s->size = str_len;
 cs_reset(_s);
 return TRUE;
}

res_t cs_bounded_skip(char_stream_t *s, char_stream_t from, char_stream_t to, bool_t _skip_spaces){
  char stream_char;
  char check_char;
  bool_t start_of_to = FALSE;
  expect(cs_simple_skip(s, from, _skip_spaces));
  expect(cs_get_char(&to, &check_char));
  while(!start_of_to){
    expect(cs_get_char(s, &stream_char));
    if(cs_is_sequence(s, &to, _skip_spaces)){
      start_of_to = TRUE;
    }else{
      cs_advance(s);
    }
  }
  expect(cs_simple_skip(s, to, _skip_spaces));
  return TRUE;
}

res_t cs_simple_skip(char_stream_t *s, char_stream_t to_skip, bool_t _skip_spaces){
  char stream_char;
  char check_char;
  while(!cs_end_of(&to_skip)){
    if(_skip_spaces){
      cs_skip_spaces(s);
    }
    expect(cs_get_char(s, &stream_char));
    expect(cs_get_char(&to_skip, &check_char));
    expect((stream_char == check_char));
    cs_advance(s);
    cs_advance(&to_skip);    
  }
  return TRUE;
}

bool_t cs_is_sequence(char_stream_t *s, char_stream_t *sequence, bool_t _skip_spaces){
  char stream_c;
  char sequence_c;
  res_t rgc = TRUE; //result get char
  backup_t s_b = cs_backup_stream(s);
  backup_t sequence_b = cs_backup_stream(sequence);
  while(!cs_end_of(sequence) && rgc){
    if(_skip_spaces){
      cs_skip_spaces(s);
    }
    rgc &= cs_get_char(sequence, &sequence_c); 
    rgc &= cs_get_char(s, &stream_c);
    rgc &= (stream_c == sequence_c);
    cs_advance(s);
    cs_advance(sequence);
  }
  cs_recover_stream(s, s_b);
  cs_recover_stream(sequence, sequence_b);
  return rgc;
}

backup_t cs_backup_stream(char_stream_t *s){
  return s->position;
}

res_t cs_recover_stream(char_stream_t *s, backup_t bk){
  s->position = bk;
  expect(cs_valid_position(s));
  return TRUE;
}

res_t cs_skip_spaces(char_stream_t *_s){
  char c;
  expect(cs_get_char(_s, &c));
  while(isspace(c)){
    cs_advance(_s);
    expect(cs_get_char(_s, &c));
  }
  return TRUE;
}

res_t cs_bounded_get(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end){
  char stream_char;
  expect(cs_is_sequence(s, &start, TRUE));
  expect(cs_simple_skip(s, start, TRUE));
  while(!cs_is_sequence(s, &end, TRUE)){
    expect(cs_get_char(s, &stream_char));
    expect(cs_set_char(output, stream_char));
    cs_advance(s);
    cs_advance(output);
  }
  stream_char = 0;
  expect(cs_set_char(output, stream_char));
  expect(cs_reset(output));
  return TRUE;
}

res_t cs_bounded_get_inplace(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end){
  unsigned int size = 0;
  expect(cs_is_sequence(s, &start, TRUE));
  expect(cs_simple_skip(s, start, TRUE));
  output->buffer = (s->buffer + s->position);
  while(!cs_is_sequence(s, &end, TRUE)){
    size++;
    cs_advance(s);
  }
  cs_simple_skip(s, end, TRUE);
  output->size = size;
  expect(cs_reset(output));
  return TRUE; 
}

res_t cs_nullterminate(char_stream_t *s){
  s->buffer[s->size] = 0;
  return TRUE;
}
res_t cs_char_at(char_stream_t *s, long int index, char *result){
  backup_t bk = cs_backup_stream(s);
  if(index >= 0){
    s->position = index;
  }else{
    index += s->size;
    s->position = index;
  }
  res_t get_c_res = cs_get_char(s, result);
  cs_recover_stream(s,bk);
  return get_c_res;
}

res_t cs_trim_end_spaces(char_stream_t *s){
  char c;
  expect(cs_char_at(s, -1, &c));
  while(isspace(c)){
    s->size-=1;
    expect(cs_char_at(s, -1, &c));
  }
  return TRUE;
}

bool_t cs_equal_streams(char_stream_t *f, char_stream_t *s){
  char c_f, c_s;
  expect(f);
  expect(s);
  expect(f->size == s->size);
  unsigned long i = 0;
  while(i < f->size){
    expect(cs_char_at(f,i,&c_f));
    expect(cs_char_at(s,i,&c_s));
    expect(c_f == c_s);
    i++;
  }
  return TRUE;
}