#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define ERROR(msg) { fprintf(stderr, "ERROR: %s - %s\n", __func__, (msg)); exit(EXIT_FAILURE); } while(0)

/*
* @return list_t* - pointer to created list
*/
list_t *list_init (){
    list_t *li = (list_t*) malloc(sizeof(list_t));
    if (li == NULL)
        ERROR("Can not malloc list.");

    li->first = NULL;
    li->last = NULL;
    li->size = 0;
    
    return li;
}


/*
* @param list_t* - list
* @param void* - data to insert
* @return struct list_elem* - inserted list-element
*/
struct list_elem *list_insert (list_t *list, void *data){
    // Create list_elem
    struct list_elem *in = (struct list_elem*) malloc(sizeof(struct list_elem));
    if(in == NULL)
        ERROR("Can not malloc list_elem.");
        
    in->data = data;
    in ->next = list->first;

    // list is empty ->'in' as last element
    if(list->first == NULL){
        list->last = in;
    }

    list->first = in; 
    list->size += 1;

    return in;
}

/*
* @param list_t* - list
* @param void* - data to append
* @return struct list_elem* - inserted list-element
*/
struct list_elem *list_append (list_t *list, void *data){
     // Create list_elem
    struct list_elem *app = (struct list_elem*) malloc(sizeof(struct list_elem));
    if(app == NULL)
        ERROR("Can not malloc list_elem.");
        
    app->data = data;
    app->next = NULL;

    // list is empty ->'app' is first and last element 
    if(list->first == NULL){
        list->first = app;
    }   else{
        list->last->next = app;
    }

    list->last = app;
    list->size += 1;

    return app;
}

/*
* @param list_t* - list
* @param void* - data to find
* @param int (*cmp_elem) - methode to compare the data
* @return struct list_elem* - found list-element (NULL if no matching element was found)
*/
struct list_elem *list_find (list_t *list, void *data, int (*cmp_elem) (const void *, const void *)){
    struct list_elem *current = list->first;

    while(current != NULL){
        if(cmp_elem(current->data, data) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}


/*
* @param list_t
* @param list_elem
* @return int: success -> 0; failure -> -1
*/
int list_remove (list_t *list, struct list_elem *elem){
    if (list->first == NULL || elem == NULL)
        return -1;

    struct list_elem *current = list->first;

    // check first element
    if(current == elem){
        list->first = current->next;
        free(elem);
        list->size -= 1;
        return 0;
    }
    // Check in rest of list
    while(current->next != NULL){
        if(current->next == elem){
            current->next = elem->next;
            free(elem);
            list->size -= 1;
            return 0;
        }
        current = current->next;
    }

    return -1;
}
/*
* @param list_t* - list
*/
void list_rev(list_t *list){
    // zero or One Element
    if (list->first == NULL || list->first == list->last)
        return;

    struct list_elem *current = list->first;
    struct list_elem *prev = NULL;
    struct list_elem *next = NULL;

    while(current != NULL){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->first = prev;
}

/*
* @param list_t* - list
* @param void (*print_elem) - methode to print data in list
*/
void list_print (list_t *list, void (*print_elem) (void *)){
    struct list_elem *current = list->first;
    while(current != NULL){
        print_elem(current->data);
        current = current->next;
    }
}

/*
* @param list_t* - list
*/
void list_finit (list_t *list){
    if(list->first == NULL){
        free(list);
        return;
    }

    struct list_elem *current = NULL;
    while (list->first != list->last) {
        current = list->first;
        list->first = current->next;
        free(current);
    }

    free(list->first);
    free(list);
}