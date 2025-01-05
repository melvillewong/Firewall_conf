#include "./include/service.h"
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2) return -1;

    if (strncmp(argv[1], "-i", strlen(argv[1])) == 0)
    {
        interactive_mode();
    }

    return 0;
}
