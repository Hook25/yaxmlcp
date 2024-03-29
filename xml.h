#ifndef XML_H
#define XML_H
#include "streams.h"

typedef struct xml_tree_tag{
    char_stream_t *tag_value;
    char_stream_t *tag_name;
    unsigned int children_count;
    struct xml_tree_tag *children; 
} xml_tree_t;

typedef enum tag_type_tag{
    none,
    open,
    close,
    self_conclusive
}tag_type_t;

res_t xs_is_header(char_stream_t s);
res_t xs_skip_header(char_stream_t *s);
res_t xs_get_tag_inplace(char_stream_t *s, char_stream_t *output, tag_type_t *tag_type);
res_t xs_get_value_inpace(char_stream_t *s, char_stream_t *output);
bool_t xs_is_open_tag(char_stream_t *s);
bool_t xs_is_close_tag(char_stream_t *s);
res_t xs_get_value_inplace(char_stream_t *s, char_stream_t *out);

res_t xt_parse_xml_into_tree(xml_tree_t *tree, char_stream_t *s);
res_t xt_is_tag_in_tree(xml_tree_t *tree, char_stream_t *s);
bool_t xt_is_tag_node(xml_tree_t *t);

#endif
