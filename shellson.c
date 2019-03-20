#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

#define BUFFER_SIZE 1024

void print_elem(void *input);

int main(){
    char *buffer = (char *) malloc(BUFFER_SIZE);    // Inputbuffer for read from stdin
    if(buffer == NULL) EXROR("Cannot malloc memory for buffer.");

    list_t *args;

    // Main Shell-Loop
    while(true){
        fprintf(stdout, ">> ");
        // Read Input into Buffer
        fgets(buffer, BUFFER_SIZE, stdin);
        fflush(stdin);

        args = parse(buffer);

        if(args != NULL){
            lprint(args, print_elem);
            lfinit(args);
        }
    }
 }

 void print_elem(void *input){
     fprintf(stdout, "%s\n", (char *) input);
 }