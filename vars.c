
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vars.h"

int add_to_end(vars_pointer *head, char *name, double value) {

    if (!head || !name) {
        return -1; 
    }

    vars *new_var = (vars *)malloc(sizeof(vars));
    if (!new_var) {
        return -1; 
    }

    new_var->name = strdup(name); 
    new_var->value = value;
    new_var->next = NULL;

    if (!head->first)
    {
        head->first = new_var;
    }
    else
    {
        vars *temp = head->first;
        temp = head->first;
            while (temp->next)
                temp = temp->next;
        if (0 != strcmp(temp->name, new_var->name))
        {
            temp->next = new_var;
        }
    }
    printd_var(new_var);
    return 0;
}

vars *last(vars_pointer *head)
{
    if (!head)
    {
        return NULL;
    }
    vars *a = head->first;
    while (a->next)
        a = a->next;
    return a;
}

vars *find(vars_pointer *head, char *name) {
    if (!head || !name) {
        return NULL; 
    }

    vars *current = head->first;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current; 
        }
        current = current->next;
    }

    return NULL; 
}


int delete_by_name(vars_pointer *head, char *name) {
    if (!head || !name) {
        return -1; 
    }

    vars *current = head->first;
    vars *prev = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            
            if (prev) {
                prev->next = current->next; 
            } else {
                head->first = current->next; 
            }

            free(current->name); 
            free(current);       
            return 0;            
        }

        prev = current;
        current = current->next;
    }

    return -1; 
}


int clear(vars_pointer *head) {
    if (!head) {
        return -1; 
    }

    vars *current = head->first;
    while (current) {
        vars *next = current->next;

        free(current->name);
        free(current);
        current->next=NULL;

        current = next;
    }

    printd_vars_list(head->first);
    head->first = NULL;

    return 0; 
}
