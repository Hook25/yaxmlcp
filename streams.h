#ifndef STREAMS_H
#define STREAMS_H
#define FALSE 0
#define TRUE !FALSE

//expect returns FALSE if expectance is not met
#define LOG_EXPECT FALSE
#if LOG_EXPECT
#include<stdio.h>
#define expect( x ) do{ if (!( x )){ printf("Expectation not met at %d of file %s, func %s\n", __LINE__, __FILE__, __func__); return FALSE; }}while(0)
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

res_t cs_get_char(char_stream_t *_s, char *c);
res_t cs_set_char(char_stream_t *_s, char c);
res_t cs_advance(char_stream_t *_s);
res_t cs_reset(char_stream_t *_s);
res_t cs_build_from_buffer(char_stream_t *_s, char *buffer, unsigned int str_len);
res_t cs_bounded_skip(char_stream_t *s, char_stream_t from, char_stream_t to, bool_t _skip_spaces);
res_t cs_simple_skip(char_stream_t *s, char_stream_t to_skip, bool_t _skip_spaces);
res_t cs_skip_spaces(char_stream_t *_s);
res_t cs_recover_stream(char_stream_t *s, backup_t back);
res_t cs_bounded_get_inplace(char_stream_t *s, char_stream_t *output, char_stream_t start, char_stream_t end);
res_t cs_nullterminate(char_stream_t *s);
res_t cs_char_at(char_stream_t *s, long int index, char *result);
res_t cs_trim_end_spaces(char_stream_t *s);

bool_t cs_equal_streams(char_stream_t *f, char_stream_t *s);
bool_t cs_end_of(char_stream_t *_s);
bool_t cs_valid_position(char_stream_t *_s);
bool_t cs_is_sequence(char_stream_t *s, char_stream_t *sequence, bool_t _skip_spaces);

backup_t cs_backup_stream(char_stream_t *s);

#endif
