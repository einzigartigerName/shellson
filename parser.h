#include <stddef.h>
#include "list.h"

#define ERROR(msg) {printf("ERROR: %s - %s\n", __func__, msg); } while(0)
#define EXROR(msg) {printf("ERROR: %s - %s\n", __func__, msg); exit(EXIT_FAILURE); } while(0)

typedef struct alias_t{
    char *alias;
    list_t *args;
} alias_t;

list_t*     parse_input(char *input);
char*       parse_prompt(char *input, char *user, char *host);
alias_t*    parse_alias(char* input);