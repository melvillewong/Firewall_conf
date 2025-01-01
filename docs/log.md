2025-01-01

- Fix still reachable memory for input

```C
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BUFFERLENGTH 256

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
    request[strcspn(request, "\n")] = '\0';

    if (strlen(request) == 0)
    {
        return "Illegal request";
    }

    return "processed request";
}

void interactive_mode()
{
    static char *input = NULL;  // Static variable to persist between calls
    static size_t n = 0;        // Static variable for buffer size

    signal(SIGINT, cleanup);  // Register cleanup function for SIGINT (Ctrl+C)

    const char *result = NULL;

    while (1)
    {
        printf(">> ");
        if (getline(&input, &n, stdin) == -1)
        {
            perror("Error reading input\n");
            break;
        }

        result = process_cmd(input);
        if (result != NULL && result[0] != '\0')
        {
            printf("%s\n", result);
        }
    }
}

// Cleanup function for handling SIGINT (Ctrl+C)
void cleanup(int sig)
{
    // Cleanup code when SIGINT (Ctrl+C) is received
    printf("\nExiting program...\n");
    static char *input = NULL;
    if (input) {
        free(input);  // Free memory allocated for input
    }
    exit(0);  // Exit gracefully after freeing memory
}
```
