#include<stdio.h>
#include<string.h>
#include "xml.h"
#include "streams.h"
#include<stdlib.h>

res_t test_header(){
    char *c = "<?-- prova 123 --?>";
    char_stream_t s;
    res_t build =  cs_build_from_buffer(&s, c, 19);
    expect(build);
    res_t is_h = xs_is_header(s);
    expect(is_h);
    res_t skipped = xs_skip_header(&s);
    expect(skipped);
    expect(cs_end_of(&s));
    return TRUE;
}

res_t test_get_tag(){
    char *c = "<abcd>";
    char_stream_t s;
    char_stream_t o_stream;
    tag_type_t tt;
    expect(cs_build_from_buffer(&s, c, 6));
    expect(xs_get_tag_inplace(&s, &o_stream, &tt));
    expect(tt == open);
    return TRUE;
}

res_t test_is_open_close_tag(){
    char *c = "<a>";
    char_stream_t s;
    expect(cs_build_from_buffer(&s, c, 3));
    expect(xs_is_open_tag(&s));
    char *k = "</a>";
    expect(cs_build_from_buffer(&s, k, 3));
    expect(xs_is_close_tag(&s));
    return TRUE;
}

res_t test_get_value(){
    char *c = "123.123123";
    char_stream_t s;
    expect(cs_build_from_buffer(&s, c, 15));
    char_stream_t out;
    expect(xs_get_value_inplace(&s, &out));
}

void print_char_stream_t(char_stream_t *c){
    printf("size-> %d position-> %d\n", c->size, c->position);
}

res_t test_parse_open_val_close(){
    char *c = strdup("<a>prova</a>");
    char_stream_t s;
    char_stream_t tag_s;
    char_stream_t value_s;
    char_stream_t tag_c_s;
    tag_type_t tt;
    expect(cs_build_from_buffer(&s, c, 13));
    expect(xs_get_tag_inplace(&s, &tag_s, &tt));
    expect(tt == open);
    expect(xs_get_value_inplace(&s, &value_s));
    expect(xs_get_tag_inplace(&s, &tag_c_s, &tt));
    expect(tt == close);
    cs_nullterminate(&tag_s);
    cs_nullterminate(&value_s);
    cs_nullterminate(&tag_c_s);
    expect(strcmp("a", tag_s.buffer) == 0);
    expect(strcmp("prova", value_s.buffer) == 0);
    expect(strcmp("a", tag_c_s.buffer) == 0);
    free(c);
    return TRUE;
}

res_t test_equal_streams(){
    char *tmp = "Lorem";
    char_stream_t s;
    char_stream_t f;
    cs_build_from_buffer(&s,tmp,5);
    cs_build_from_buffer(&f,tmp,5);
    expect(cs_equal_streams(&f,&s));
    return TRUE;
}

res_t test_parse_xml(){
    xml_tree_t root_tag = {0};
    xml_tree_t output_tree = {0};
    char_stream_t root_tag_stream;
    char_stream_t output;
    char_stream_t s;
    char buffer[11];
    strcpy(buffer, "<a>123</a>");
    char *expected_result = "123";
    char *a = (char*)"a";
    expect(cs_build_from_buffer(&root_tag_stream, a, 1));
    expect(cs_build_from_buffer(&s, buffer, 11));
    root_tag.tag_name = &root_tag_stream;
    root_tag.children_count = 1;
    output_tree.tag_value = &output;
    root_tag.children = &output_tree;
    expect(xt_parse_xml_into_tree(&root_tag, &s));
    cs_nullterminate(&output);
    expect(strcmp(output.buffer, expected_result) == 0);
    return TRUE;
}

int main(){
    res_t header_test_result = test_header();
    if(!header_test_result){
        printf("Failed skip header test\n");
    }
    res_t get_tag_result = test_get_tag();
    if(!get_tag_result){
        printf("Failed get tag test\n");
    } 
    res_t is_open_tag_result = test_is_open_close_tag();
    if(!is_open_tag_result){
        printf("Failed is open-close tag\n");
    }
    res_t get_value_result = test_get_value();
    if(!get_value_result){
        printf("Failed to get value inplace\n");
    }
    res_t o_v_c_result = test_parse_open_val_close();
    if(!o_v_c_result){
        printf("Failed open value close\n");
    }
    res_t equal_streams_result = test_equal_streams();
    if(!equal_streams_result){
        printf("Failed equal streams\n");
    }
    res_t parse_xml_result = test_parse_xml();
    if(!parse_xml_result){
        printf("Failed to parse xml\n");
    }
    return 0;
}