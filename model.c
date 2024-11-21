
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "cJSON.h"

stance init_stance(int id, formula *f) {
    stance s;
    s.id = id;
    s.f = f;
    s.h = NULL;
    s.next = NULL;
    return s;
}

int add_stance(model *m, stance *s) {
    if (!m || !s) {
        return -1; 
    }

    if (!m->stances) {
        m->stances = s; 
    } else {
        stance *current = m->stances;
        while (current->next) {
            current = current->next; 
        }
        current->next = s; 
    }

    return 0; 
}

int clear_stances(model *m) {
    if (!m) {
        return -1; 
    }

    stance *current = m->stances;
    while (current) {
        stance *next = current->next;

        
        if (current->f) {
            clear_formula(current->f);
            free(current->f);
        }

        
        clear_hopes(current);

        free(current); 
        current = next; 
    }

    m->stances = NULL; 
    return 0; 
}

int add_hope(stance *source, stance *target, formula *f, formula *c) {
    if (!source || !target || !f || !c) {
        return -1; 
    }

    hopes *new_hope = (hopes *)malloc(sizeof(hopes));
    if (!new_hope) {
        return -1; 
    }

    new_hope->target = target;
    new_hope->condition = c;
    new_hope->f = f;
    new_hope->next = NULL;

    if (!source->h) {
        source->h = new_hope; 
    } else {
        hopes *current = source->h;
        while (current->next) {
            current = current->next; 
        }
        current->next = new_hope; 
    }

    return 0; 
}

int clear_hopes(stance *s) {
    if (!s) {
        return -1; 
    }

    hopes *current = s->h;
    while (current) {
        hopes *next = current->next;

        
        if (current->condition) {
            clear_formula(current->condition);
            free(current->condition);
        }
        if (current->f) {
            clear_formula(current->f);
            free(current->f);
        }

        free(current); 
        current = next; 
    }

    s->h = NULL; 
    return 0; 
}

int is_number(const char *str) {
    if (!str || *str == '\0') return 0; 

    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0'; 
}

operation *create_operation(cJSON *json_op) {
    operation *op = (operation *)malloc(sizeof(operation));
    if (!op) {
        fprintf(stderr, "Memory allocation error for operation.\n");
        return NULL;
    }

    op->a = NULL;
    op->b = NULL;
    op->result = NULL;
    op->func = NULL;
    op->is_bool = 0;
    op->is_a_const = 0;
    op->is_b_const = 0;
    op->a_const = 0.0;
    op->b_const = 0.0;
    op->next = NULL;

    char *operand1 = cJSON_GetObjectItem(json_op, "operand1")?cJSON_GetObjectItem(json_op, "operand1")->valuestring:strdup("");
    char *operand2 = cJSON_GetObjectItem(json_op, "operand2")?cJSON_GetObjectItem(json_op, "operand2")->valuestring:strdup("");
    char *result = cJSON_GetObjectItem(json_op, "result")?cJSON_GetObjectItem(json_op, "result")->valuestring:strdup("");
    char *action = cJSON_GetObjectItem(json_op, "action")?cJSON_GetObjectItem(json_op, "action")->valuestring:strdup("");

    if (is_number(operand1)) {
        op->is_a_const = 1;
        op->a_const = atof(operand1);
    } else {
        op->is_a_const = 0;
        op->a = strdup(operand1);
    }

    if (operand2)
        if (is_number(operand2))
        {
            op->is_b_const = 1;
            op->b_const = atof(operand2);
        }
        else
        {
            if (strcmp(operand2, "")==0)
                op->is_b_const = -1;
            else
            {
                op->is_b_const = 0;
                op->b = strdup(operand2);
            }
        }

    op->result = strdup(result);

    op->func = strdup(action);

    if (strcmp(action, "equals") == 0 || 
        strcmp(action, "greater_than") == 0 || 
        strcmp(action, "less_than") == 0 || 
        strcmp(action, "greater_equal") == 0 || 
        strcmp(action, "less_equal") == 0 || 
        strcmp(action, "logical_or") == 0 || 
        strcmp(action, "logical_and") == 0 || 
        strcmp(action, "logical_not") == 0) {
        op->is_bool = 1;
    } else {
        op->is_bool = 0;
    }

    return op;
}


formula *create_formula(cJSON *json_formulas) {
    formula *f = (formula *)malloc(sizeof(formula));
    f->first = NULL;
    operation *last_op = NULL;

    cJSON *json_op = NULL;
    cJSON_ArrayForEach(json_op, json_formulas) {
        operation *op = create_operation(json_op);
        if (!f->first) {
            f->first = op;
        } else {
            last_op->next = op;
        }
        last_op = op;
    }

    return f;
}


hopes *create_hopes(cJSON *json_transitions, stance *all_stances, vars_pointer * vars_p) {
    hopes *head = NULL;
    hopes *last = NULL;
    cJSON *json_transition = NULL;
    cJSON_ArrayForEach(json_transition, json_transitions) {
        hopes *h = (hopes *)malloc(sizeof(hopes));
        char *to_name = cJSON_GetObjectItem(json_transition, "to")->valuestring;
        stance *target = all_stances;
        while (target && target->id!=atoi(to_name)) {
            target = target->next;
        }
        h->target = target;
        
        cJSON *conditions = cJSON_GetObjectItem(json_transition, "conditions");
        h->condition = create_formula(conditions);
        cJSON *actions = cJSON_GetObjectItem(json_transition, "actions");
        h->f = create_formula(actions);

        h->next = NULL;
        if (!head) {
            head = h;
        } else {
            last->next = h;
        }
        last = h;
        add_variables(vars_p, h->f);
    }
    return head;
}

void add_variables(vars_pointer *vars_p, formula * f) {
    if (!vars_p)
        vars_p = (vars_pointer *)malloc(sizeof(vars_pointer));

    operation * item = f->first;
    
    while (item)
    {
        printd_operations(item);
        if (!item->is_a_const)
            if (!find(vars_p, item->a))
            {
                add_to_end(vars_p, item->a, 0.0);
            }
        if (item->is_b_const==0)
            if (!find(vars_p, item->b))
            {
                add_to_end(vars_p, item->b, 0.0);
            }
        if (!find(vars_p, item->result))
            {
                add_to_end(vars_p, item->result, 0.0);
            }
        item=item->next;
    }
}

model *parse_model(char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(file_size + 1);
    fread(content, 1, file_size, file);
    fclose(file);

    content[file_size] = '\0';

    cJSON *json = cJSON_Parse(content);
    free(content);

    if (!json) {
        fprintf(stderr, "Error: Invalid JSON format\n");
        return NULL;
    }
    model *m = (model *)malloc(sizeof(model));
    m->vars_p->first = NULL;
    m->stances = NULL;


    cJSON *stances = cJSON_GetObjectItem(json, "stances");
    cJSON *json_stance = NULL;
    cJSON_ArrayForEach(json_stance, stances) {
        stance *s = (stance *)malloc(sizeof(stance));
        s->id = atoi(cJSON_GetObjectItem(json_stance, "name")->valuestring);

        cJSON *formulas = cJSON_GetObjectItem(json_stance, "formulas");
        s->f = create_formula(formulas);
        cJSON *json_op = NULL;
        add_variables(m->vars_p, s->f);
        s->h = NULL;
        s->next = NULL;
        add_stance(m, s);
    }

    stance *last_stance = m->stances;
    cJSON_ArrayForEach(json_stance, stances) {
        cJSON *transitions = cJSON_GetObjectItem(json_stance, "transitions");
        last_stance->h = create_hopes(transitions, m->stances, m->vars_p);

        last_stance = last_stance->next;
    }

    cJSON_Delete(json);
    return m;
}

void print_formula(formula *f) {
    if (!f || !f->first) {
        printf("        (empty formula)\n");
        return;
    }

    operation *current_op = f->first;
    while (current_op) {
        printf("        %s = %s(",
            current_op->result, 
            current_op->func);
        if (current_op->is_a_const)
            printf("%d",current_op->a_const);
        else
            printf("%s", current_op->a);
        
        if (current_op->is_b_const)
            printf(", %d", current_op->b_const);
        else
            if(0 != strlen(current_op->b))
                printf(", %s", current_op->b);
        printf(")\n");
        current_op = current_op->next;
    }
}

void print_hopes(hopes *h) {
    if (!h) {
        printf("    hopes:\n        (no transitions)\n");
        return;
    }

    printf("    hopes:\n");
    while (h) {
        printf("        to: %d\n", h->target ? h->target->id : -1);
        
        printf("        conditions:\n");
        print_formula(h->condition);

        printf("        actions:\n");
        print_formula(h->f);

        h = h->next;
    }
}

void print_stance(stance *s)
{
    if (!s)
    {
        printf("stances:\n    (no stances)\n");
        return;
    }

    printf("    id = %d\n", s->id);
    printf("    formula:\n");
    print_formula(s->f);
    print_hopes(s->h);
    s = s->next;
}

void print_model(model *m)
{
    if (!m)
    {
        printf("Model is empty.\n");
        return;
    }
    stance *s = m->stances;
    int i=0;
    while (s)
    {
        i++;
        print_stance(s);
        s = s->next;
        if(i>5)break;
    }
}
