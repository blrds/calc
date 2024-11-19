
typedef struct operation
{
    char * a, * b, *result, *func;
    int is_bool;
    struct operation * next;
}operation;

typedef struct formula
{
    operation * first;
}formula;

operation operation_init(char * a, char * b, char * result, char * func);
int add_new_formula(formula * f, operation * o);
int clear_formula(formula * f);