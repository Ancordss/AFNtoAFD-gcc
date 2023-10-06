// #ifndef XML_PARSER_H
// #define XML_PARSER_H

// #include <stdio.h>

// void init_parser(FILE *input_file);
// int parse_xml();
// char** get_tokens_linearlayout();
// char** get_tokens_estadosattributes();
// char** get_tokens_transcicionales();
// char** get_tokens_finalesattributes();
// char** get_tokens_iniciales();

// #endif // MI_PARSER_H


#ifndef XML_PARSER_H
#define XML_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void init_parser(FILE *input_file);
void cleanup_parser();
int parse_xml();
char** get_tokens_linearlayout();
char** get_tokens_estadosattributes();
char** get_tokens_transcicionales();
char** get_tokens_finalesattributes();
char** get_tokens_iniciales();

#ifdef __cplusplus
}
#endif

#endif // XML_PARSER_H
