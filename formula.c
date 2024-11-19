
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formula.h"

operation operation_init(char *a, char *b, char * result, char *func) {
    operation o;
    o.a = strdup(a); 
    o.b = strdup(b);
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
