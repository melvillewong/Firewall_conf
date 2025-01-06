#include "../include/helper.h"
#include "../include/cleaner.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split_argv_by_delimiter(char *input, char **fst_part, char **scd_part,
                             char delimiter)
{
    char *delimiter_pos = strchr(input, delimiter);
    if (delimiter_pos == NULL)
    {
        *fst_part = NULL;
        *scd_part = NULL;
        return;
    }

    int fst_length = delimiter_pos - input;
    int scd_length = strlen(input) - fst_length - 1;

    *fst_part = (char *)malloc(fst_length + 1);
    *scd_part = (char *)malloc(scd_length + 1);

    strncpy(*fst_part, input, fst_length);
    strncpy(*scd_part, delimiter_pos + 1, scd_length);

    (*fst_part)[fst_length] = '\0';
    (*scd_part)[scd_length] = '\0';
}

void handle_sig(int sig)
{
    switch (sig)
    {
        case SIGINT:
            printf("\n");
            free_memory();
            exit(0);
        case SIGHUP:
            printf("Terminal closed. Cleaning up...\n");
            free_memory();
            exit(0);
        case SIGTERM:
            printf("Cleaning up...\n");
            free_memory();
            exit(0);
        case SIGSEGV:
            printf("Caught segmentation fault. Cleaning up...\n");
            free_memory();
            exit(1);
    }
}
