#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "parser.h"

char* append_char(char *str, char c){
    char *result = NULL;
    asprintf(&result, "%s%c", str, c);
    free(str);
    return result;
}

char* append_str(char *str, char *append){
    char *result = NULL;
    asprintf(&result, "%s%s", str, append);
    free(str);
    return result;
}

char* reset(){
    char *pointer = malloc(1);
    memset(pointer, 0, 1);
    return pointer;
}

list_t* parse_input(char *input){
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
                if(is_backslash)
                    tmp = append_char(tmp, '\\');
                tmp = append_char(tmp, c);
            break;
        }
    }

    free(tmp);
    if(is_single_quote){ printf("ERROR: missing matching \' to %d.\n", is_single_quote); lfinit(args); return NULL; }
    if(is_double_quote){ printf("ERROR: missing matching \" to %d.\n", is_double_quote); lfinit(args); return NULL; }

    return args;
}

char* parse_prompt(char *input, char *user, char *host){
    list_t *lexed = parse_input(input);
    if(lexed == NULL || lexed->size != 3){
        lfinit(lexed);
        return ">>";
    }

    if(strcmp(lexed->first->data, "PROMPT") != 0){
        lfinit(lexed);
        return ">>";
    }
    
    if(strcmp(lget(lexed, 1)->data, "=") != 0){
        lfinit(lexed);
        return ">>";
    }

    // process promt-var
    int is_percent = 0;     // Flag
    char *tmp = reset();

    char *prompt_var = lexed->last->data;
    for(int i = 0; i < (int) strlen(prompt_var); i++){
        char c = *(prompt_var + i);
        switch(c){
            // percent
            case '%':
                if(is_percent){
                    tmp = append_char(tmp, '%');
                    is_percent = 0;
                } else{
                    is_percent = i + 1;
                }
            break;
            // user
            case 'u':
                if(is_percent){
                    tmp = append_str(tmp, user);
                    is_percent = 0;
                } else{
                    tmp = append_char(tmp, c);
                }
            break;
            // host
            case 'h':
                if(is_percent){
                    tmp = append_str(tmp, host);
                    is_percent = 0;
                } else{
                    tmp = append_char(tmp, c);
                }
            break;
            // default
            default:
                is_percent = 0;
                tmp = append_char(tmp, c);
            break;
        }
    }



    if(is_percent){
        free(tmp);
        lfinit(lexed);
        return ">>";
    }

    lfinit(lexed);
    return tmp;
}

// ZSH/Bash compatible
alias_t* parse_alias(char *input){ 
    return NULL;
}