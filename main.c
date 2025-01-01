#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERLENGTH 256

char *input = NULL;

char *process_cmd(char *request);
void interactive_mode();
void cleanup(int sig);

int main(int argc, char **argv)
{
    if (argc != 2) return -1;

    if (strncmp(argv[1], "-i", strlen(argv[1])) == 0)
    {
        interactive_mode();
    }

    return 0;
}

char *process_cmd(char *request)
{
    // char *cmd = NULL;
    // char *cmd_argv = NULL;
    // char *result = (char *)malloc(BUFFERLENGTH);

    printf("strlen: %ld", strlen(request));
    request[strcspn(request, "\n")] = '\0';

    if (strlen(request) == 0)
    {
        return "Illegal request";
    }

    return "processed request";
}

void interactive_mode()
{
    size_t n = 0;
    const char *result = NULL;

    // cleanup allocated memory when exiting (^C)
    signal(SIGINT, cleanup);

    while (1)
    {
        printf(">> ");
        if (getline(&input, &n, stdin) == -1)
        {
            if (feof(stdin))
            {
                // EOF (^D) detected
                printf("\nEOF detected. Exitiing program...\n");
            }
            else if (ferror(stdin))
            {
                // Error while reading
                perror("Error reading input\n");
            }
            break;
        }

        result = process_cmd(input);
        if (result != NULL && result[0] != '\0')
        {
            printf("%s\n", result);
        }
    }

    // free input when normal exit (^D)
    if (input)
    {
        free((void *)input);
        input = NULL;
    }
}

void cleanup(int sig)
{
    printf("\nExiting program...\n");
    if (input)
    {
        free(input);
        input = NULL;
    }
    exit(0);
}
