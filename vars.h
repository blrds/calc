
typedef struct vars
{
    char * name;
    double value;
    struct vars * next;
}vars;

typedef struct vars_pointer
{
    vars * first;
    vars * last;
}vars_pointer;


int add_to_end(vars_pointer * head, char * name, double value);
vars * find(vars_pointer * head, char * name);
int delete_by_name(vars_pointer * head, char * name);
int clear(vars_pointer * head);
