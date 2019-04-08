# yaxmlcp

This project aim is to provide a robust xml parser for embedded devices.
  
### The main objectives are :
* Avoid malloc and free, to ensure memory aviability
* Minimize the memory impact, making most of the operations in place
* Be robust to invalid xml

### Example usage
```C
#include<stdio.h>
#include<string.h>
#include "xml.h"
#include "streams.h"
#include<stdlib.h>

int main(){
    xml_tree_t root_tag = {0};
    xml_tree_t output_tree = {0};
    char_stream_t root_tag_stream;
    char_stream_t output;
    char_stream_t s;
    char buffer[11];
    strcpy(buffer, "<a>123</a>");
    char *a = (char*)"a";
    assert(cs_build_from_buffer(&root_tag_stream, a, 1));
    assert(cs_build_from_buffer(&s, buffer, 10));
    root_tag.tag_name = &root_tag_stream;
    root_tag.children_count = 1;
    output_tree.tag_value = &output;
    root_tag.children = &output_tree;
    assert(xt_parse_xml_into_tree(&root_tag, &s));
    cs_nullterminate(&output);
    printf(output.buffer);
    return TRUE;
}
```
