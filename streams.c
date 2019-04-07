#include "streams.h"
#include <ctype.h>

bool_t valid_position(char_stream_t *_s){
  return _s->buffer && _s->position >= 0 && _s->position < _s->size;
}

res_t get_char(char_stream_t *_s, char *c){
  expect(valid_position(_s));
  *c = _s->buffer[_s->position];
  return TRUE;  
}

res_t set_char(char_stream_t *_s, char c){
  expect(valid_position(_s));
  _s->buffer[_s->position] = c;
  return TRUE;
}

res_t advance(char_stream_t *_s){
  _s->position++;
  return valid_position(_s);
}

res_t reset(char_stream_t *_s){
  _s->position = 0;
  return TRUE;
}

bool_t end_of(char_stream_t *_s){
  return _s->position == _s->size;
}

res_t build_from_buffer(char_stream_t *_s, char *buffer, unsigned int buffer_size){
 expect(_s && buffer);
 _s->buffer = buffer;
 _s->size = buffer_size;
 reset(_s);
 return TRUE;
}

res_t bounded_skip(char_stream_t *s, char_stream_t from, char_stream_t to, bool_t _skip_spaces){
  char stream_char;
  char check_char;
  bool_t start_of_to = FALSE;
  expect(simple_skip(s, from, _skip_spaces));
  expect(get_char(&to, &check_char));
  while(!start_of_to){
    expect(get_char(s, &stream_char));
    if(is_sequence(s, &to, _skip_spaces)){
      start_of_to = TRUE;
    }else{
      advance(s);
    }
  }
  expect(simple_skip(s, to, _skip_spaces));
  return TRUE;
}

res_t simple_skip(char_stream_t *s, char_stream_t to_skip, bool_t _skip_spaces){
  char stream_char;
  char check_char;
  while(!end_of(&to_skip)){
    if(_skip_spaces){
      skip_spaces(s);
    }
    expect(get_char(s, &stream_char));
    expect(get_char(&to_skip, &check_char));
    expect((stream_char == check_char));
    advance(s);
    advance(&to_skip);    
  }
  return TRUE;
}

bool_t is_sequence(char_stream_t *s, char_stream_t *sequence, bool_t _skip_spaces){
  char stream_c;
  char sequence_c;
  res_t rgc = TRUE; //result get char
  backup_t s_b = backup_stream(s);
  backup_t sequence_b = backup_stream(sequence);
  while(!end_of(sequence) && rgc){
    if(_skip_spaces){
      skip_spaces(s);
    }
    rgc &= get_char(sequence, &sequence_c); 
    rgc &= get_char(s, &stream_c);
    rgc &= (stream_c == sequence_c);
    advance(s);
    advance(sequence);
  }
  recover_stream(s, s_b);
  recover_stream(sequence, sequence_b);
  return rgc;
}

backup_t backup_stream(char_stream_t *s){
  return s->position;
}

res_t recover_stream(char_stream_t *s, backup_t bk){
  s->position = bk;
  expect(valid_position(s));
  return TRUE;
}

res_t skip_spaces(char_stream_t *_s){
  char c;
  expect(get_char(_s, &c));
  while(isspace(c)){
    advance(_s);
    expect(get_char(_s, &c));
  }
  return TRUE;
}

res_t bounded_get(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end){
  char stream_char;
  expect(is_sequence(s, &start, TRUE));
  expect(simple_skip(s, start, TRUE));
  while(!is_sequence(s, &end, TRUE)){
    expect(get_char(s, &stream_char));
    expect(set_char(output, stream_char));
    advance(s);
    advance(output);
  }
  stream_char = 0;
  expect(set_char(output, stream_char));
  expect(reset(output));
  return TRUE;
}

res_t bounded_get_inplace(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end){
  unsigned int size = 0;
  expect(is_sequence(s, &start, TRUE));
  expect(simple_skip(s, start, TRUE));
  output->buffer = (s->buffer + s->position);
  while(!is_sequence(s, &end, TRUE)){
    size++;
    advance(s);
  }
  simple_skip(s, end, TRUE);
  output->size = size;
  expect(reset(output));
  return TRUE; 
}

res_t nullterminate(char_stream_t *s){
  s->buffer[s->size] = 0;
  return TRUE;
}
res_t char_at(char_stream_t *s, long int index, char *result){
  backup_t bk = backup_stream(s);
  if(index >= 0){
    s->position = index;
  }else{
    index += s->size;
    s->position = index;
  }
  res_t get_c_res = get_char(s, result);
  recover_stream(s,bk);
  return get_c_res;
}

res_t trim_end_spaces(char_stream_t *s){
  char c;
  expect(char_at(s, -1, &c));
  while(isspace(c)){
    s->size-=1;
    expect(char_at(s, -1, &c));
  }
  return TRUE;
}

bool_t equal_streams(char_stream_t *f, char_stream_t *s){
  char c_f, c_s;
  expect(f);
  expect(s);
  expect(f->size == s->size);
  unsigned long i = 0;
  while(i < f->size){
    expect(char_at(f,i,&c_f));
    expect(char_at(s,i,&c_s));
    expect(c_f == c_s);
    i++;
  }
  return TRUE;
}