#include<stdio.h>
#include<string.h>
#include "xml.h"
#include "streams.h"
#include<stdlib.h>

res_t test_header(){
    char *c = "<?-- prova 123 --?>";
    char_stream_t s;
    res_t build =  build_from_buffer(&s, c, 19);
    expect(build);
    res_t is_h = is_header(s);
    expect(is_h);
    res_t skipped = skip_header(&s);
    expect(skipped);
    expect(end_of(&s));
    return TRUE;
}

res_t test_get_tag(){
    char *c = "<abcd>";
    char_stream_t s;
    char_stream_t o_stream;
    tag_type_t tt;
    expect(build_from_buffer(&s, c, 6));
    expect(get_tag_inplace(&s, &o_stream, &tt));
    expect(tt == open);
    return TRUE;
}

res_t test_is_open_close_tag(){
    char *c = "<a>";
    char_stream_t s;
    expect(build_from_buffer(&s, c, 3));
    expect(is_open_tag(&s));
    char *k = "</a>";
    expect(build_from_buffer(&s, k, 3));
    expect(is_close_tag(&s));
    return TRUE;
}

res_t test_get_value(){
    char *c = "123.123123";
    char_stream_t s;
    expect(build_from_buffer(&s, c, 15));
    char_stream_t out;
    expect(get_value_inplace(&s, &out));
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
    expect(build_from_buffer(&s, c, 13));
    expect(get_tag_inplace(&s, &tag_s, &tt));
    expect(tt == open);
    expect(get_value_inplace(&s, &value_s));
    expect(get_tag_inplace(&s, &tag_c_s, &tt));
    expect(tt == close);
    nullterminate(&tag_s);
    nullterminate(&value_s);
    nullterminate(&tag_c_s);
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
    build_from_buffer(&s,tmp,5);
    build_from_buffer(&f,tmp,5);
    expect(equal_streams(&f,&s));
    return TRUE;
}

res_t test_parse_xml(){
    xml_tree_t t[2];
    char *buffer = (char*)"<a>123</a>";
    char *a = (char*)"a";
    char_stream_t a_s;
    build_from_buffer(&a_s, a,1);
    char_stream_t s;
    build_from_buffer(&s, buffer, 10);
    char_stream_t v_a_s; //result
    t[0].children_count = 1;
    t[0].tag_name = &a_s;
    t[0].children = &t[1];
    t[1].tag_value = &v_a_s;
    parse_xml_into_tree(t, &s);
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
        printf("Failed equal streams");
    }
    return 0;
}