#include "vars.h"
#include "formula.h"

typedef struct stance
{
    int id;
    formula *f;
    struct hopes *h;
    struct stance *next;
} stance;

typedef struct hopes
{
    stance *target;
    formula *condition;
    formula *f;
    struct hopes *next;
} hopes;

typedef struct model
{
    vars_pointer vars_p;
    stance *stances;
} model;

stance init_stance(int id, formula *f);
int add_stance(model *m, stance *s);
int clear_stances(model *m);

int add_hope(stance *source, stance *target, formula *f, formula *c);
int clear_hopes(stance *s);

void print_formula(formula *f);
void print_hopes(hopes *h);
void print_stance(stance *s);
model *parse_model(char * path);
void print_model(model *m);
