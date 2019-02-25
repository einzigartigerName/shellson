struct list_elem{
    struct list_elem    *next;
    char                *data;
};

typedef struct list{
    struct list_elem    *first;
    struct list_elem    *last;
} list_t;

list_t                  *list_init ();
struct list_elem        *list_insert (list_t *list, char *data);
struct list_elem        *list_append (list_t *list, char *data);
int                     list_remove (list_t *list, struct list_elem *elem);
void                    list_finit (list_t *list);
void                    list_print (list_t *list, void (*print_elem) (char *));
struct list_elem        *list_find (list_t *list, char *data, int (*cmp_elem) (const char *, const char *));