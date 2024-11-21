
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formula.h"

operation operation_init(char *a, char *b, char * result, char *func, double a_const, double b_const, int is_a, int is_b) {
    operation o;
    if (is_a)
    {
        o.is_a_const = 0;
        o.a = strdup(a);
    }
    else
    {
        o.is_a_const = 1;
        o.a = strdup("");
        o.a_const = a_const;
    }
    if (is_b)
    {
        o.is_b_const = 0;
        o.b = strdup(b);
    }
    else
    {
        o.is_b_const = 1;
        o.b = strdup("");
        o.b_const = b_const;
    }
    o.func = strdup(func);
    o.result = strdup(result);
    o.is_bool = 0;
    o.next = NULL;
    return o;
}

int add_new_formula(formula *f, operation *o) {
    if (!f || !o) {
        return -1; 
    }

    if (!f->first) {
        f->first = o; 
    } else {
        operation *current = f->first;
        while (current->next) {
            current = current->next; 
        }
        current->next = o; 
    }

    return 0; 
}

int clear_formula(formula *f) {
    if (!f) {
        return -1; 
    }

    operation *current = f->first;
    while (current) {
        operation *next = current->next;
        free(current->a);     
        free(current->b);     
        free(current->func);  
        free(current);        
        current = next;       
    }

    f->first = NULL; 
    return 0; 
}

double add(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a + b;
}

double subtract(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a - b;
}

double multiply(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a * b;
}

double divide(double a, double b) {
    printd_formula("%lf %lf", a, b);
    if (b == 0) {
        // Предотвращение деления на 0
        return 0.0;
    }
    return a / b;
}

int mod(int a, int b)
{
    printd_formula("%d %d", a, b);
    if (b == 0) {
        // Предотвращение деления на 0
        return 0;
    }
    return a % b;
}

// Логические операции
int greater_than(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a > b;
}

int less_than(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a < b;
}

int equals(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a == b;
}

int greater_equal(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a >= b;
}

int less_equal(double a, double b) {
    printd_formula("%lf %lf", a, b);
    return a <= b;
}

int logical_or(int a, int b) {
    printd_formula("%d %d", a, b);
    return (a || b);
}

int logical_and(int a, int b) {
    printd_formula("%d %d", a, b);
    return (a && b);
}

int logical_not(int a) {
    printd_formula("%d", a);
    return !a;
}
