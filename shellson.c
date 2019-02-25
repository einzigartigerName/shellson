#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

#define BUFFER_SIZE 1024

void print_elem(char *input);

int main(){
    char *buffer = (char *) malloc(BUFFER_SIZE);    // Inputbuffer for read from stdin
    if(buffer == NULL){ perror("ERROR: Cannot alloc memory for BUFFER"); exit(EXIT_FAILURE); }

    list_t *args;

    // Main Shell-Loop
    while(true){
        fprintf(stdout, ">> ");
        // Read Input into Buffer
        fgets(buffer, BUFFER_SIZE, stdin);
        fflush(stdin);

        args = parse(buffer);

        list_print(args, print_elem);
    }
 }

 void print_elem(char *input){
     fprintf(stdout, "%s", input);
 }