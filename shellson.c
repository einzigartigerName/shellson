#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"

#define BUFFER_SIZE 1024
#define CONFIG_FILE ".shellsonrc"
#define DEFAULT_PROMPT "[%s@%s]$"

// Alias related
void alias_print(void *data);
int alias_search(const void *a, const void *b);
int alias_cmp(const void *a,const void *b);
void *alias_cpy(void *data);

// util
void *cpystr(void *data);
void print(void *input);

// general loop
void list_to_argv(list_t *list, char *array[]);
int check_explicite(char *data);
void execute(char *args[], char *envp[]);

int main(int argc, char *argv[], char *envp[]){
    char *buffer = (char *) malloc(BUFFER_SIZE);    // Inputbuffer for read from stdin
    if(buffer == NULL) EXROR("Cannot malloc memory for buffer.");

    /* 
    *   Setup: Read and process Config-file
    */
    char *host = (char *) malloc(BUFFER_SIZE);
    if(buffer == NULL) EXROR("Cannot malloc memory for buffer.");

    // login username
    char *user = getlogin();
    gethostname(host, BUFFER_SIZE);

    // config file
    char *config_file_path = NULL;
    asprintf(&config_file_path, "/home/%s/%s", user, CONFIG_FILE);

    // home folder
    char *home = NULL;
    asprintf(&home, "/home/%s", user);

    // fallback prompt
    char *prompt = NULL;
    asprintf(&prompt, DEFAULT_PROMPT, user, host);

    // list with alias
    list_t *alias = linit();

    // if config-file exits -> read else create
    FILE *fd_config = fopen(config_file_path, "rb+");
    if (fd_config == NULL) {
        // Create config and write std-prompt
        fd_config = fopen(config_file_path, "wb");
        fprintf(fd_config, "PROMPT = \"%%u@%%h$\"\n");
        fclose(fd_config);
    } else {
        // Read line by line and parse
        while (fgets(buffer, BUFFER_SIZE, fd_config) != NULL){
            // parse PROMPT-Var
            if(strncmp(buffer, "PROMPT", 6) == 0){
                prompt = parse_prompt(buffer, user, host);

            }
            // parse alias
            else if(strncmp(buffer, "alias", 5) == 0){

                alias_t *tmp = parse_alias(buffer);
                if(tmp == NULL)
                    continue;

                struct list_elem *rm = lfind(alias, tmp->alias, alias_cmp);
                if(rm != NULL){
                    lrm(alias, rm);
                }

                lappend(alias, tmp);
            }
            // import alias from file
            else if(strncmp(buffer, "source", 6) == 0){
                list_t *lexed = parse_input(buffer);
                if(lexed == NULL || lexed->size != 2)
                    continue;

                // open file and parse alias
                FILE *fd_source = fopen(lexed->last->data, "r");
                if(fd_source == NULL)
                    ERROR("Source-File cannot be opened");

                while (fgets(buffer, BUFFER_SIZE, fd_source) != NULL){

                    if(strncmp(buffer, "alias", 5) == 0){
                        alias_t *tmp = parse_alias(buffer);
                        if(tmp == NULL)
                            continue;

                        struct list_elem *rm = lfind(alias, tmp->alias, alias_cmp);
                        if(rm != NULL){
                            lrm(alias, rm);
                        }

                        lappend(alias, tmp);
                    }
                }
            }
        }
    }


    /*
    *   Main Shell-Loop
    */
    while(true){
        fprintf(stdout, "%s ", prompt);
        // Read Input into Buffer
        fgets(buffer, BUFFER_SIZE, stdin);
        fflush(stdin);

        list_t *largs = parse_input(buffer);
        // Check for no arguments
        if(largs == NULL){
            continue;
        }
        
        /*
        * Check for Keywords
        */
        char *first = largs->first->data;
        // exit shell - exit
        if(strcmp(first, "exit") == 0){
            exit(EXIT_SUCCESS);
        }

        // change directory - cd
        else if(strcmp(first, "cd") == 0){
            if(largs->size > 2)
                ERROR("Too many arguments for \'cd\'.");

            if(largs->size == 1){
                chdir(home);
                continue;
            }

            chdir(largs->last->data);
            continue;
        }

        /*
        *   Check if alias was called
        */
        struct list_elem *aliascall = lfind(alias, first, alias_search);
        if(aliascall != NULL){
            // TODO: execute Alias
            // create copy of alias argv
            continue;
        }

        // transform list to argv for execve
        char *args[largs->size + 1];
        list_to_argv(largs, args);

        // try executing
        execute(args, envp);

        lfinit(largs);
    }

    /*
    *   Exit the Shell
    */
    free(buffer);
    free(user);
    free(host);
    free(home);
    free(prompt);
}

/*
* Handle alias_t
*/
// print
void alias_print(void *data){
    printf("%s\n", (char *) ((alias_t *) data)->alias);
}

// compare two alias_t via alias
int alias_cmp(const void *a, const void *b){
    return strcmp((char *) ((alias_t *) a)->alias, (char *) ((alias_t *) b)->alias);
}

// compare alias_t->alias with char* 
int alias_search(const void *a, const void *b){
    return strcmp((char *) ((alias_t *) a)->alias, (char *) b);
}

/*
*   Util
*/
void *cpystr(void *data){
    char *out = NULL;
    asprintf(&out, "%s", (char *) data);
    return out;
}

void print(void *input){
    printf("%s\n", (char *) input);
}

// build argv from array to execute later
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

// check if argument is explicite call
int check_explicite(char *data){
    for(int i = 0; i < (int) strlen(data); i++){
        char c = *(data + i);
        if(c == '/')
           return 1;
    }
    return 0;
}

// try executing
void execute(char *args[], char *envp[]){
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
}
