#include <stddef.h>

struct list_elem {
    struct list_elem    *next;	    // Next Element in List
    void	            *data;	    // Data
};

typedef struct list {
    size_t              size;       // Size of List
	struct list_elem    *first;     // First Element of List
	struct list_elem    *last;  	// Last Element of List
} list_t;

/* function prototypes */
list_t                  *linit ();
struct list_elem        *linsert (list_t *list, void *data);
struct list_elem        *lappend (list_t *list, void *data);
int                     lrm (list_t *list, struct list_elem *elem);
void                    lrmf (list_t *list);
void                    lrml (list_t *list);
struct list_elem        *lfind (list_t *list, void *data, int (*cmp_elem) (const void *, const void *));
void                    lprint (list_t *list, void (*print_elem) (void *));
void                    lrev(list_t *list);
void                    lfinit (list_t *list);
