#include <stddef.h>
#include "list.h"

#define ERROR(msg) {printf("ERROR: %s - %s\n", __func__, msg); } while(0)
#define EXROR(msg) {printf("ERROR: %s - %s\n", __func__, msg); exit(EXIT_FAILURE); } while(0)

list_t* parse(char *input);