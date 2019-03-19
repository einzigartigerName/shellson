#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

// Checks if pointer is null
void check_malloc(void *pointer){
    if(pointer == NULL){ 
        perror("ERROR: Cannot malloc.\n");
        exit(EXIT_FAILURE);
    }
}

// Appends char c to string str
char* append_char(char* str, char c, bool termiante, size_t *str_len){
	// char to String for adding
	char c_str[2];
	c_str[0] = c;
	c_str[1] = '\0';
	strcat(str, c_str);

	if(!termiante){
        *str_len = *str_len + 1;
        str = (char *) realloc(str, *str_len);
		check_malloc(str);
    }
	return str;
}

// Adds string to list
void add_to_list(list_t *li, char *data, size_t *len){
    append_char(data, '\0', true, len);
    list_append(li, data);
}

// Resetting the given string-pointer
char* reset(char *pointer){
    pointer = malloc(1);
    check_malloc(pointer);
	*pointer = '\0';
	return pointer;
}

// Parse the string and returns list with arguments
list_t *parse(char *input){
    // Output list with all args
    list_t *args = list_init();
    check_malloc(args);

    // flags for parsing
    int is_double_quote = 0;
    int is_single_quote = 0;
    int is_backslash = 0;

    size_t len = 1;
    char *tmp_arg = malloc(len);
    check_malloc(tmp_arg);
	*tmp_arg = '\0';

    int input_len = strlen(input);
    for(int i = 0; i < input_len; i++){
        char c = *(input + i);

        switch(c){
            case ' ':   // Space
                if(is_double_quote || is_single_quote || is_backslash){
                    tmp_arg = append_char(tmp_arg, c, false, &len);
                    is_backslash = 0;
                }   else{
                    add_to_list(args, tmp_arg, &len);
                    tmp_arg = reset(tmp_arg);
                    len = 1;
                }
                break;

            case '"':   // Double Quote
                if(is_backslash || is_single_quote){
                    tmp_arg = append_char(tmp_arg, c, false, &len);
                } else if(is_double_quote){
                    add_to_list(args, tmp_arg, &len);
                    tmp_arg = reset(tmp_arg);
                    len = 1;
                    is_double_quote = 0;
                } else{
                    is_double_quote = i + 1;
                }
                break;
            
            case '\'':  // Single Quote
                if(is_backslash || is_double_quote){
                    tmp_arg = append_char(tmp_arg, c, false, &len);
                } else if(is_single_quote){
                    add_to_list(args, tmp_arg, &len);
                    tmp_arg = reset(tmp_arg);
                    len = 1;
                    is_single_quote = 0;
                } else{
                    is_single_quote = i + 1;
                }
                break;
            
            case '\n':
                goto end_of_loop;

            default:
                tmp_arg = append_char(tmp_arg, c, false, &len);
                break;
        }
    }
    end_of_loop:;
	// Add last word
	add_to_list(args, tmp_arg, &len);

	// Check for unclosed Quotes
	if(is_single_quote){ fprintf(stderr, "ERROR: No matching single quote to quote at %d!\n", is_single_quote); exit(EXIT_FAILURE); }
    if(is_double_quote){ fprintf(stderr, "ERROR: No matching double quote to quote at %d!\n", is_double_quote); exit(EXIT_FAILURE); }

	free(tmp_arg);

    return args;
}