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
    expect(build_from_buffer(&s, c, 7));
    expect(get_tag_inplace(&s, &o_stream));
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
    expect(build_from_buffer(&s, c, 13));
    expect(get_tag_inplace(&s, &tag_s));
    expect(get_value_inplace(&s, &value_s));
    expect(get_tag_inplace(&s, &tag_c_s));
    nullterminate(&tag_s);
    nullterminate(&value_s);
    nullterminate(&tag_c_s);
    expect(strcmp("a", tag_s.buffer) == 0);
    expect(strcmp("prova", value_s.buffer) == 0);
    expect(strcmp("a", tag_c_s.buffer) == 0);
    free(c);
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
    return 0;
}