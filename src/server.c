#include "../include/server.h"
#include <regex.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input = NULL;

int check_ip(const char *ip)
{
    regex_t regex;
    int ret;
    const char *pattern = "^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)"
                          "\\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)"
                          "\\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)"
                          "\\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)$";

    // compile regex
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret)
    {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    // execute regex matching
    ret = regexec(&regex, ip, 0, NULL, 0);
    regfree(&regex);

    if (!ret)
    {
        printf("Valid IP\n");
    }
    else
    {
        printf("Invalid IP\n");
        return 0;
    }

    return 1;
}

int check_port(const char *port)
{
    regex_t regex;
    int ret;
    const char *pattern = "(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]"
                          "{3}|[1-9][0-9]{0,3}|0)";

    // compile regex
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret)
    {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    // execute regex matching
    ret = regexec(&regex, port, 0, NULL, 0);
    regfree(&regex);

    if (!ret)
    {
        printf("Valid Port\n");
    }
    else
    {
        printf("Invalid Port\n");
        return 0;
    }

    return 1;
}

char *process_cmd(char *request)
{
    // char *cmd = NULL;
    // char *cmd_argv = NULL;
    // char *result = (char *)malloc(BUFFERLENGTH);

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
