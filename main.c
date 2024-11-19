#include "model.h"

int is_running = 0;

int run()
{
    while (is_running)
    {

    }

    return 0;
}

int main(int argc, char* argv[])
{
    model * m=parse_model(argv[1]);
    print_model(m);
    return 0;
}

