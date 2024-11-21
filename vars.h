#ifndef VARS_H
#define VARS_H

#ifdef DEBUG_VARS
#define vars_to_str(var) ({                          \
    static char buf[DEBUG_BUFFER];                   \
    if ((var) == NULL)                               \
        snprintf(buf, DEBUG_BUFFER, "NULL");         \
    else                                             \
        snprintf(buf, DEBUG_BUFFER,                  \
                 "Variable: %s = %.2f",              \
                 (var)->name ? (var)->name : "NULL", \
                 (var)->value);                      \
    buf;                                             \
})
#define printd_var(var)                                              \
    {                                                                \
        printf("<%s>\n%s\n", __func__, vars_to_str(var)); \
    }

#define printd_vars_list(var)                               \
    {                                                       \
        vars *v = var;                                      \
        printf("<%s>Vars list begin\n", __func__);          \
        do                                                  \
        {                                                   \
            printf("%s\n", vars_to_str(var));               \
            if (v)                                          \
            {                                               \
                v = v->next;                                \
                printf("v->next=%s\n", v ? v->name : NULL); \
            }                                               \
        } while (v);                                        \
        printf("Vars list end\n");                          \
    }
#else
#define vars_to_str(var)
#define printd_var(var)
#define printd_vars_list(var)
#endif // DEBUG_VARS

typedef struct vars
{
    char * name;
    double value;
    struct vars * next;
}vars;

typedef struct vars_pointer
{
    vars * first;
}vars_pointer;

int add_to_end(vars_pointer * head, char * name, double value);
vars * find(vars_pointer * head, char * name);
vars * last(vars_pointer * head);
int delete_by_name(vars_pointer * head, char * name);
int clear(vars_pointer * head);

#endif //VARS_H
