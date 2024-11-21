#ifndef FORMULA_H
#define FORMULA_H

#ifdef DEBUG_FORMULA
#define printd_formula(...){\
    printf("<%s>", __func__);\
    printf(__VA_ARGS__);\
    printf("\n");\
}
#else
#define printd_formula(...)
#endif //DEBUG_FORMULA

#ifdef DEBUG_OPERATIONS
#define operation_to_str(op) ({ \
    static char buf[DEBUG_BUFFER]; \
    if ((op) == NULL) snprintf(buf, DEBUG_BUFFER, "NULL"); \
    else snprintf(buf, DEBUG_BUFFER, \
        "Operation: %s = %s(%s, %s) (is_bool: %d, is_a_const: %d, a_const: %.2f, is_b_const: %d, b_const: %.2f)", \
        (op)->result ? (op)->result : "NULL", \
        (op)->func ? (op)->func : "NULL", \
        (op)->is_a_const ? "(const)" : ((op)->a ? (op)->a : "NULL"), \
        (op)->is_b_const==1 ? "(const)" : ((op)->is_b_const==0 ? (op)->b : "NULL"), \
        (op)->is_bool, \
        (op)->is_a_const, \
        (op)->a_const, \
        (op)->is_b_const, \
        (op)->b_const \
    ); \
    buf; \
})
#define printd_operations(op){\
    printf("<%s>\n%s\n", __func__, operation_to_str(op));\
}
#else
#define operation_to_str(op)
#define printd_operations(op)
#endif //DEBUG_OPERATIONS

typedef struct operation
{
    char * a, * b, *result, *func;
    int is_bool, is_a_const, is_b_const;
    double a_const, b_const;
    struct operation * next;
}operation;

typedef struct formula
{
    operation * first;
}formula;

operation operation_init(char *a, char *b, char * result, char *func, double a_const, double b_const, int is_a, int is_b);
int add_new_formula(formula * f, operation * o);
int clear_formula(formula * f);

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
int mod(int a, int b);

int greater_than(double a, double b);
int less_than(double a, double b);
int equals(double a, double b);
int greater_equal(double a, double b);
int less_equal(double a, double b);
int logical_or(int a, int b);
int logical_and(int a, int b);
int logical_not(int a);

#endif // FORMULA_H