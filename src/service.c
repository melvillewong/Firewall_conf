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
        printf("%s\n", request_head_copy->request);
        // if (request_head_copy->next != NULL)
        // {
        //     printf("\n");
        // }
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
        free(ips);
        free(ports);
        return 0;
    }

    store_rule(ips, ports);

    free(ips);
    free(ports);
    return 1;
}

Connection_t connect_rule_cmd(char *ip_port)
{
    char *ip = NULL;
    char *port = NULL;
    Connection_t result;

    split_argv_by_delimiter(ip_port, &ip, &port, ' ');

    int is_single = !(strchr(ip, '-') || strchr(port, '-'));

    if (!(is_valid_ip_port(ip, IP) && is_valid_ip_port(port, PORT) &&
          is_single))
    {
        result = ILLEGAL;
    }
    else
    {
        if (store_query(ip, port))
        {
            result = ACCEPTED;
        }
        else
        {
            result = REJECTED;
        }
    }

    free(ip);
    free(port);
    return result;
}

Deleteion_t del_rule_cmd(char *rule)
{
    char *ips = NULL;
    char *ports = NULL;
    Deleteion_t result;

    split_argv_by_delimiter(rule, &ips, &ports, ' ');

    if (!(is_valid_ip_port(ips, IP) && is_valid_ip_port(ports, PORT)))
    {
        result = INVALID;
    }

    if (del_rule(ips, ports))
    {
        result = DELETED;
    }
    else
    {
        result = NOT_FOUND;
    }

    free(ips);
    free(ports);

    return result;
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
            printf("Query: %s %s\n", query->ip, query->port);
            // if (query->next != NULL)
            // {
            //     printf("\n");
            // }
            query = query->next;
        }
        rule_head_copy = rule_head_copy->next;
    }
}

char *process_cmd(char *request)
{
    char *cmd = NULL;
    char *cmd_argv = NULL;
    char *result = NULL;

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
            return NULL;
        }
        else if (strcmp(request, "L") == 0)
        {
            list_rule_cmd();
            return NULL;
        }
        else
        {
            return "Illegal request";
        }
    }

    split_argv_by_delimiter(request, &cmd, &cmd_argv, ' ');

    if (cmd == NULL || cmd_argv == NULL)
    {
        free(cmd);
        free(cmd_argv);
        return "Invalid rule";
    }

    // printf("cmd_argv: %s\n", cmd_argv);
    if (strcmp(cmd, "A") == 0)
    {
        if (add_rule_cmd(cmd_argv))
        {
            result = "Rule added";
        }
        else
        {
            result = "Invalid rule";
        }
    }
    else if (strcmp(cmd, "C") == 0)
    {
        Connection_t outcome = connect_rule_cmd(cmd_argv);
        if (outcome == ACCEPTED)
        {
            result = "Connection accepted";
        }
        else if (outcome == REJECTED)
        {
            result = "Connection rejected";
        }
        else if (outcome == ILLEGAL)
        {
            result = "Illegal IP address or port specified";
        }
    }
    else if (strcmp(cmd, "D") == 0)
    {
        Deleteion_t outcome = del_rule_cmd(cmd_argv);
        if (outcome == DELETED)
        {
            result = "Rule deleted";
        }
        else if (outcome == NOT_FOUND)
        {
            result = "Rule not found";
        }
        else if (outcome == INVALID)
        {
            result = "Rule invalid";
        }
    }
    else
    {
        result = "Illegal request";
    }

    free(cmd);
    free(cmd_argv);
    return result;
}

void interactive_mode()
{
    size_t n = 0;

    // cleanup allocated memory for distinct exiting signal
    signal(SIGINT, handle_sig);
    signal(SIGHUP, handle_sig);
    signal(SIGTERM, handle_sig);
    signal(SIGSEGV, handle_sig);

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
    free_memory();
}
