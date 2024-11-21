#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <locale.h>
#include "model.h"

#ifdef DEBUG_MAIN
#define printd_main(...)          \
    {                             \
        printf("<%s>", __func__); \
        printf(__VA_ARGS__);      \
        printf("\n");             \
    }
#else
#define printd_main(...)
#endif
#ifdef DEBUG_MENU
#define printd_menu(...)          \
    {                             \
        printf("<%s>", __func__); \
        printf(__VA_ARGS__);      \
        printf("\n");             \
    }
#else
#define printd_menu(...)
#endif

int streq(char *a, char *b)
{
    return 0 == strcmp(a, b);
}

volatile int is_running = 0;

int foo()
{
    return 0;
}

void clear_local_vars(vars_pointer *local_vars)
{
    printd_vars_list(local_vars->first);
    clear(local_vars);
    free(local_vars);
}

void print_vars(vars_pointer *vp)
{
    vars *v = vp->first;
    while (v)
    {
        printf("%s=%lf ", v->name, v->value);
        v = v->next;
    }
    printf("\n");
}

int evaluate_formula(formula *f, vars_pointer *global_vars, int is_constant)
{
    vars_pointer *local_vars = is_constant ? NULL : (vars_pointer *)malloc(sizeof(vars_pointer));
    operation *op = f->first;
    while (op)
    {
        printd_operations(op);
        double a = 0.0;
        if (op->is_a_const == 1)
            a = op->a_const;
        else if (op->is_a_const == 0)
        {
            if (is_constant)
                a = find(global_vars, op->a)->value;
            else
            {
                vars *temp = find(local_vars, op->a);
                a = (temp ? temp : find(global_vars, op->a))->value;
            }
        }
        double b = 0.0;
        if (op->is_b_const == 1)
            b = op->b_const;
        else if (op->is_b_const == 0)
            if (is_constant)
                b = find(global_vars, op->b)->value;
            else
            {
                vars *temp = find(local_vars, op->b);
                b = (temp ? temp : find(global_vars, op->b))->value;
            }
        double result = 0.0;

        if (strcmp(op->func, "add") == 0)
            result = add(a, b);
        else if (strcmp(op->func, "subtract") == 0)
            result = subtract(a, b);
        else if (strcmp(op->func, "multiply") == 0)
            result = multiply(a, b);
        else if (strcmp(op->func, "divide") == 0)
            result = divide(a, b);
        else if (strcmp(op->func, "mod") == 0)
            result = mod(a, b);
        else if (strcmp(op->func, "greater_than") == 0)
            result = greater_than(a, b);
        else if (strcmp(op->func, "less_than") == 0)
            result = less_than(a, b);
        else if (strcmp(op->func, "equals") == 0)
            result = equals(a, b);
        else if (strcmp(op->func, "greater_equal") == 0)
            result = greater_equal(a, b);
        else if (strcmp(op->func, "less_equal") == 0)
            result = less_equal(a, b);
        else if (strcmp(op->func, "logical_or") == 0)
            result = logical_or((int)a, (int)b);
        else if (strcmp(op->func, "logical_and") == 0)
            result = logical_and((int)a, (int)b);
        else if (strcmp(op->func, "logical_not") == 0)
            result = logical_not((int)a);

        vars *res_var = find(is_constant ? global_vars : local_vars, op->result);
        if (res_var)
            res_var->value = result;
        else
            add_to_end(is_constant ? global_vars : local_vars, op->result, result);

        op = op->next;
    }
    double res = is_constant ? 0 : last(local_vars)->value;
    if (!is_constant)
        clear_local_vars(local_vars);
    return res == 1;
}

void *run(void *arg)
{
    model *m = (model *)arg;
    stance *current_state = NULL;
    if (!current_state && m->stances)
    {
        current_state = m->stances;
        if (current_state->id == 0)
        {
            evaluate_formula(current_state->f, m->vars_p, 1);
            hopes *h = current_state->h;
            if (h)
            {
                evaluate_formula(h->f, m->vars_p, 1);
                current_state = h->target;
            }
        }
        else
        {
            current_state = current_state->next;
        }
    }
    while (is_running)
    {
#ifdef MAX_TICKS
        if (find(m->vars_p, "time")->value > MAX_TICKS)
            break;
#endif
        printd_main("Tact start");
#ifdef DEBUG_VARS_TIME
        print_vars(m->vars_p);
#endif // DEBUG_VARS_TIME
        printd_main("eval_formula s=%d", current_state->id);
        evaluate_formula(current_state->f, m->vars_p, 1);
        hopes *h = current_state->h;
        int j = 0;
        while (h)
        {
            printd_main("checking condition to s=%d", h->target->id);
            int res = evaluate_formula(h->condition, m->vars_p, 0);

            if (res == 1)
            {
                printd_main("eval_formula of h targeted s=%d", h->target->id);
                evaluate_formula(h->f, m->vars_p, 1);
                current_state = h->target;
                foo();
                break;
            }

            h = h->next;
        }

        printd_main("Tact end");
        printd_main("---------------");
        sleep(1);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    setlocale(LC_NUMERIC, "ru_RU");
    pthread_t thread_id;
    model *m = parse_model(argv[1]);
    print_model(m);
    while (1)
    {
        int command;
        printd_menu("Enter command (1 to start, 0 to stop): ");
        scanf("%d", &command);

        if (command == 1)
        {
            if (!is_running)
            {
                is_running = 1;
                if (pthread_create(&thread_id, NULL, run, (void *)m) != 0)
                {
                    fprintf(stderr, "Error creating thread.\n");
                    return 1;
                }
            }
            else
            {
                printd_menu("Thread is already running.");
            }
        }
        else if (command == 0)
        {
            if (is_running)
            {
                is_running = 0;
                pthread_join(thread_id, NULL);
                printd_menu("Program stopped.");
                break;
            }
            else
            {
                printd_menu("Thread is not running.");
            }
        }
        else
        {
            printd_menu("Unknown command. Please enter 1 or 0.");
        }
    }

    return 0;
}