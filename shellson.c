#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"

#define BUFFER_SIZE 1024

void print_elem(void *input);
void list_to_argv(list_t *list, char *array[]);
int check_explicite(char *data);

int main(int argc, char *argv[], char *envp[]){
    char *buffer = (char *) malloc(BUFFER_SIZE);    // Inputbuffer for read from stdin
    if(buffer == NULL) EXROR("Cannot malloc memory for buffer.");

    list_t *args_list;

    // Main Shell-Loop
    while(true){
        fprintf(stdout, ">> ");
        // Read Input into Buffer
        fgets(buffer, BUFFER_SIZE, stdin);
        fflush(stdin);

        args_list = parse(buffer);
        // Check for no arguments
        if(args_list == NULL){
            continue;
        }
        
        // Check for Shell-Keywords
        char *first = args_list->first->data;
        // exit shell - exit
        if(strcmp(first, "exit") == 0){
            exit(EXIT_SUCCESS);
        }
        // change directory - cd
        else if(strcmp(first, "cd") == 0){
            if(args_list->size > 2)
                ERROR("Too many arguments for \'cd\'.");
            chdir(args_list->last->data);
            continue;
        }

        // transform list to argv for execve
        char *args[args_list->size + 1];
        list_to_argv(args_list, args);

        // fork process to execute commands
        pid_t pid = fork();
        int exstatus;
        switch(pid){
            // ERROR in fork
            case -1:
                ERROR("Process cannot be forked!");
            break;

            // Child - execute programm
            case 0:
                // Execute Explicit called programm
                if(check_explicite(args[0])){
                        execve(args[0], args, envp);
                }
                // Check in $PATH for Programm
                else{
                    // $PATH
                    char *path = getenv("PATH");
            
                    const char delim[2] = ":";
                    char *pathvar;

                    // Walk through $PATH and search for programm + exec
                    pathvar = strtok(path, delim);
                    while(pathvar != NULL){
                        char *filename = NULL;
                        asprintf(&filename, "%s/%s", pathvar, args[0]);
                        // execute and check if failed
                        if(execve(filename, args, envp) != -1)
                            break; // Programm executed - Exit loop

                        // Programm failed - new pathvar
                        pathvar = strtok(NULL, delim);
                    }
                    if(pathvar == NULL){
                        printf("Programm \'%s\' not found!\n", args[0]);
                        exit(EXIT_FAILURE);
                    }
                }
            break;

            // Parent - wait for child
            default:
                wait(&exstatus);
            break;
        }
        lfinit(args_list);
    }
}

void print_elem(void *input){ fprintf(stdout, "%s\n", (char *) input); }

void list_to_argv(list_t *list, char *array[]){

    struct list_elem *current = list->first;
    size_t i = 0;
    while(current != NULL){
        array[i] = current->data;
        current = current->next;
        i++;
    }
    array[list->size] = (char *) 0;
}

int check_explicite(char *data){
    for(int i = 0; i < (int) strlen(data); i++){
        char c = *(data + i);
        if(c == '/')
           return 1;
    }
    return 0;
}