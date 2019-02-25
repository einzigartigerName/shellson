struct list_elem {
    struct list_elem    *next;	    // Next Element in List
    char	            *data;	    // Data
};

typedef struct list {
    size_t              size;       // Size of List
	struct list_elem    *first;     // First Element of List
	struct list_elem    *last;  	// Last Element of List
} list_t;

/* function prototypes */
list_t                  *list_init ();
struct list_elem        *list_insert (list_t *list, char *data);
struct list_elem        *list_append (list_t *list, char *data);
int                     list_remove (list_t *list, struct list_elem *elem);
void                    list_finit (list_t *list);
void                    list_print (list_t *list, void (*print_elem) (char *));
struct list_elem        *list_find (list_t *list, char *data, int (*cmp_elem) (const char *, const char *));
char*                   list_remove_first(list_t *li);
