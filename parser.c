#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

char* append_char(char *str, char c){
    char *result = NULL;
    asprintf(&result, "%s%c", str, c);
    free(str);
    return result;
}

char* reset(){
    char *pointer = malloc(1);
    memset(pointer, 0, 1);
    return pointer;
}

list_t* parse(char *input){
    list_t *args = linit();

    int is_single_quote = 0;
    int is_double_quote = 0;
    int is_backslash = 0;

    char *tmp = reset();

    int input_len = strlen(input);
    // Check for empty input
    if(input_len == 1){
        return NULL;
    }
    for(int i = 0; i < input_len; i++){
        char c = *(input + i);
        switch(c){
            // single quote
            case '\'':
                if(is_backslash){
                    tmp = append_char(tmp, c);
                    is_backslash = 0;
                } else if(is_double_quote){
                    tmp = append_char(tmp, c);
                } else if(is_single_quote){
                    lappend(args, tmp);
                    tmp = reset(tmp);
                    is_single_quote = 0;
                } else{
                    tmp = reset();
                    is_single_quote = i + 1;
                }
            break;

            // double quote
            case '"':
                if(is_backslash){
                    tmp = append_char(tmp, c);
                    is_backslash = 0;
                } else if(is_single_quote){
                    tmp = append_char(tmp, c);
                } else if(is_double_quote){
                    lappend(args, tmp);
                    tmp = reset();
                    is_double_quote = 0;
                } else{
                    tmp = reset();
                    is_double_quote = i + 1;
                }
            break;
            // backslash
            case '\\':
                if(is_backslash){
                    tmp = append_char(tmp, c);
                    is_backslash = 0;
                } else{
                    is_backslash = i + 1;
                }
            break;
            // end of line - treat like space
            case '\n':
            //space
            case ' ':
                if(is_single_quote || is_double_quote){
                    tmp = append_char(tmp, c);
                } else if(is_backslash){
                    tmp = append_char(tmp, c);
                    is_backslash = 0;
                } else{
                    if(strcmp(tmp, "") != 0){
                        lappend(args, tmp);
                        tmp = reset();
                    }
                }
            break;

            // default
            default: 
                tmp = append_char(tmp, c);
            break;
        }
    }

    free(tmp);
    if(is_single_quote){ printf("ERROR: missing matching \' to %d.\n", is_single_quote); lfinit(args); return NULL; }
    if(is_double_quote){ printf("ERROR: missing matching \" to %d.\n", is_double_quote); lfinit(args); return NULL; }

    return args;
}