#ifndef STREAMS_H
#define STREAMS_H
#define FALSE 0
#define TRUE !FALSE

//expect returns FALSE if expectance is not met
#define LOG_EXPECT FALSE
#if LOG_EXPECT
#include<stdio.h>
#define expect( x ) do{ if (!( x )){ printf("Expectation not met at %d of file %s\n", __LINE__, __FILE__); return FALSE; }}while(0)
#else
#define expect( x ) do{ if(!( x )){ return FALSE; } } while(0)
#endif
typedef char res_t;
typedef res_t bool_t;
typedef unsigned int backup_t;

typedef struct char_stream_token {
  char *buffer;
  unsigned int size;
  unsigned int position;
} char_stream_t;

bool_t valid_position(char_stream_t *_s);
res_t get_char(char_stream_t *_s, char *c);
res_t set_char(char_stream_t *_s, char c);
res_t advance(char_stream_t *_s);
res_t reset(char_stream_t *_s);
bool_t end_of(char_stream_t *_s);
res_t build_from_buffer(char_stream_t *_s, char *buffer, unsigned int buffer_size);
res_t bounded_skip(char_stream_t *s, char_stream_t from, char_stream_t to, bool_t _skip_spaces);
res_t simple_skip(char_stream_t *s, char_stream_t to_skip, bool_t _skip_spaces);
bool_t is_sequence(char_stream_t *s, char_stream_t *sequence, bool_t _skip_spaces);
res_t skip_spaces(char_stream_t *_s);
backup_t backup_stream(char_stream_t *s);
res_t recover_stream(char_stream_t *s, backup_t back);
res_t bounded_get_inplace(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end);
res_t nullterminate(char_stream_t *s);

#endif
