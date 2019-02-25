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

// Append given char to string
char *append_char(char *str,char c){
    char *result = NULL;
    asprintf(&result, "%s%c", str, c);
    free(str);

    return result;
}

char *reset_pointer(char *pointer){
    //free(pointer);

    pointer = NULL;
    asprintf(&pointer, "");

    return pointer;
}

list_t* parse(char *input){
    // list of args to be returned
    list_t *li = list_init();
    check_malloc(li);

    // Flags for spezial Chars
    int is_backshlash = 0;
    int is_single_quote = 0;
    int is_double_quote = 0;

    // tmp var to store ongoing arg
    char *tmp_arg = malloc(1);
    check_malloc(tmp_arg);
    *tmp_arg = '\0';

    // Main Parser-Loop
    for(unsigned long i = 0; i < strlen(input); i++){
        char c = *(input + i);

        switch(c){
            case ' ':   // Whitespace
                if(is_backshlash || is_single_quote || is_double_quote){
                    tmp_arg = append_char(tmp_arg, c);
                    is_backshlash = 0;
                }   else{
                    list_append(li, tmp_arg);
                    tmp_arg = reset_pointer(tmp_arg);
                }
            break;

            case '"':   // Double Quote
                if(is_double_quote){
                    is_double_quote = 0;
                    list_append(li, tmp_arg);
                    tmp_arg = reset_pointer(tmp_arg);
                }   else if(is_backshlash){
                    tmp_arg = append_char(tmp_arg, c);
                    is_backshlash = 0;
                }   else if(is_single_quote){
                    tmp_arg = append_char(tmp_arg, c);
                }   else{
                    is_double_quote = i;
                }
            break;

            case '\'':  // Single Quote
                if(is_single_quote){
                    is_single_quote = 0;
                    list_append(li, tmp_arg);
                    tmp_arg = reset_pointer(tmp_arg);
                }   else if(is_backshlash){
                    tmp_arg = append_char(tmp_arg, c);
                    is_backshlash = 0;
                }   else if(is_double_quote){
                    tmp_arg = append_char(tmp_arg, c);
                }   else{
                    is_single_quote = i;
                }
            break;

            case '\\':
                if(is_backshlash){
                    tmp_arg = append_char(tmp_arg, c);
                    is_backshlash = 0;
                }   else{
                    is_backshlash = i;
                }
            break;

            default:
                tmp_arg = append_char(tmp_arg, c);
            break;
        }
    }
    // Add last word
	list_append(li, tmp_arg);

	// Check for unclosed Quotes
	if(is_single_quote){ fprintf(stderr, "ERROR: No matching single quote to quote at %d!\n", is_single_quote); exit(EXIT_FAILURE); }
    if(is_single_quote){ fprintf(stderr, "ERROR: No matching double quote to quote at %d!\n", is_double_quote); exit(EXIT_FAILURE); }

	free(tmp_arg);
    return li;
}