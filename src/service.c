#include "../include/service.h"
#include "../include/checker.h"
#include "../include/cleaner.h"
#include "../include/global.h"
#include "../include/helper.h"
#include "../include/storage.h"
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input = NULL;
char *result = NULL;

void list_requests_cmd()
{
    Request *request_head_copy = request_head;

    while (request_head_copy != NULL)
    {
        printf("%s", request_head_copy->request);
        if (request_head_copy->next != NULL)
        {
            printf("\n");
        }
        request_head_copy = request_head_copy->next;
    }
}

int add_rule_cmd(char *rule)
{
    char *ips = NULL;
    char *ports = NULL;

    split_argv_by_delimiter(rule, &ips, &ports, ' ');

    if (ips == NULL || ports == NULL)
    {
        ips = NULL;
        ports = NULL;
        return 0;
    }

    if (!(is_valid_ip_port(ips, IP) && is_valid_ip_port(ports, PORT)))
    {
        // printf("ips: %d, ports: %d\n", is_valid_ip_port(ips, IP),
        //        is_valid_ip_port(ports, PORT));
        free(ips);
        free(ports);
        return 0;
    }

    // printf("ips: %s, ports: %s", ips, ports);
    store_rule(ips, ports);
    return 1;
}

int connect_rule_cmd(char *ip_port)
{
    char *ip = NULL;
    char *port = NULL;

    split_argv_by_delimiter(ip_port, &ip, &port, ' ');

    int is_single = !(strchr(ip, '-') || strchr(port, '-'));

    if (!(is_valid_ip_port(ip, IP) && is_valid_ip_port(port, PORT) &&
          is_single))
    {
        return 0;
    }
    else
    {
        if (store_query(ip, port))
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    free(ip);
    free(port);
}

int del_rule_cmd(char *rule)
{
    char *ips = NULL;
    char *ports = NULL;

    split_argv_by_delimiter(rule, &ips, &ports, ' ');

    if (!(is_valid_ip_port(ips, IP) && is_valid_ip_port(ports, PORT)))
    {
        free(ips);
        free(ports);
        return 0; // Rule invalid
    }

    if (del_rule(ips, ports))
    {
        return 1; // Rule deleted
    }

    free(ips);
    free(ports);

    return 2; // Rule not found
}
void list_rule_cmd()
{
    if (rule_head == NULL) return;

    Rule *rule_head_copy = rule_head;

    while (rule_head_copy != NULL)
    {
        if (rule_head_copy->ips != NULL && rule_head_copy->ports != NULL)
        {
            printf("Rule: %s %s\n", rule_head_copy->ips, rule_head_copy->ports);
        }

        Query *query = rule_head_copy->query;
        while (query != NULL)
        {
            printf("Query: %s %s", query->ip, query->port);
            if (query->next != NULL)
            {
                printf("\n");
            }
            query = query->next;
        }
        rule_head_copy = rule_head_copy->next;
    }
}

char *process_cmd(char *request)
{
    char *cmd = NULL;
    char *cmd_argv = NULL;
    char *result = (char *)malloc(BUFFERLENGTH);

    request[strcspn(request, "\n")] = '\0';

    if (strlen(request) == 0)
    {
        return "Illegal request";
    }

    store_request(&request_head, &request_tail, request);

    if (strchr(request, ' ') == NULL)
    {
        if (strcmp(request, "R") == 0)
        {
            list_requests_cmd();
        }
        else if (strcmp(request, "L") == 0)
        {
            list_rule_cmd();
        }
        else
        {
            snprintf(result, BUFFERLENGTH, "Illegal request");
        }
        return result;
    }

    split_argv_by_delimiter(request, &cmd, &cmd_argv, ' ');

    if (cmd == NULL || cmd_argv == NULL)
    {
        free(cmd);
        free(cmd_argv);
        snprintf(result, BUFFERLENGTH, "Invalid rule");
        return result;
    }

    printf("cmd_argv: %s\n", cmd_argv);
    if (strcmp(cmd, "A") == 0)
    {
        if (add_rule_cmd(cmd_argv))
        {
            return "Rule added";
        }
        else
        {
            return "Invalid rule";
        }
    }
    else if (strcmp(cmd, "C") == 0)
    {
        if (connect_rule_cmd(cmd_argv) == 1)
        {
            return "Connection accepted";
        }
        else if (connect_rule_cmd(cmd_argv) == 2)
        {
            return "Connection rejected";
        }
        else
        {
            return "Illegal IP address or port specified";
        }
    }
    else if (strcmp(cmd, "D") == 0)
    {
        if (del_rule_cmd(cmd_argv) == 1)
        {
            return "Rule deleted";
        }
        else if (del_rule_cmd(cmd_argv) == 2)
        {
            return "Rele not found";
        }
        else
        {
            return "Rele invalid";
        }
    }
    else
    {
        snprintf(result, BUFFERLENGTH, "Illegal request");
    }

    free(cmd);
    free(cmd_argv);

    return result;
}

void interactive_mode()
{
    size_t n = 0;

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
    else if (result)
    {
        free((void *)result);
        result = NULL;
    }
}
