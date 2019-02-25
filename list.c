#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"


list_t *list_init (){
    list_t *liste = (list_t *)malloc(sizeof(list_t));

    if(liste == NULL){
        return NULL;
    }
    liste->first = NULL;
    liste->last = NULL;
    return liste;
}

struct list_elem *list_insert (list_t *list, char *data){
    if(list->first == NULL){
        list->first = (struct list_elem *)malloc(sizeof(struct list_elem));
        // check malloc worked
        if(list->first == NULL){ perror("ERROR: Malloc not working"); return NULL;}
        list->last = list->first;
        list->first->data = data;
        return list->first;
    } else{
        struct list_elem *elem = (struct list_elem *)malloc(sizeof(struct list_elem));
         if(elem == NULL){ perror("ERROR: Malloc not working"); return NULL;}

        elem->data = data;
        elem->next = list->first;
        list->first = elem;

        return list->last;   
    }
}

struct list_elem *list_append (list_t *list, char *data){
    if(list->first == NULL){
        list->first = (struct list_elem *)malloc(sizeof(struct list_elem));
        
        if(list->first == NULL){ perror("ERROR: Malloc not working"); return NULL;}
        list->last = list->first;
        list->first->data = data;
        return list->last;
    } else{
        struct list_elem *elem = (struct list_elem *)malloc(sizeof(struct list_elem));
         if(elem == NULL){ perror("ERROR: Malloc not working"); return NULL;}

        elem->data = data;
        list->last->next = elem;
        list->last = elem;

        return list->last;   
    }
}

int list_remove (list_t *list, struct list_elem *elem){
    if(list->first == NULL){ return -1; }

    struct list_elem *now = list->first;

    if(now == elem){    // check first element
        list->first = now->next;
        free(elem);
        return 0;
    }
    while(now->next != NULL){   // Check rest of list
        if(now->next == elem){
            now->next = elem->next;
            free(elem);
            return 0;
        }
        now = now->next;
    }
    return -1;
}

void list_finit (list_t *list){
    if(list->first == NULL){ free(list); return;}

    while(list->first != list->last){
        struct list_elem *freenow = list->first;
        list->first = freenow->next;
        free(freenow);
    }
    free(list->first);
    free(list);
}

void list_print (list_t *list, void (*print_elem) (char *)){
    size_t count = 1;
    struct list_elem *now = list->first;
    while(now != NULL){
        printf("%zu:", count);
        print_elem(now->data);
        printf("\n");
        now = now->next;
        count++;
    }
}

struct list_elem *list_find (list_t *list, char *data, int (*cmp_elem) (const char *, const char *)){
    if(list->first == NULL){ return NULL;}

    struct list_elem *now = list->first;
    while(now != NULL){
        if(cmp_elem(now->data, data) == 0){ return now; }
        now = now->next;
    }
    return NULL;
}
