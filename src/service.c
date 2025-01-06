#include "../include/service.h"
#include "../include/checker.h"
#include "../include/cleaner.h"
#include "../include/global.h"
#include "../include/helper.h"
#include "../include/storage.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input = NULL;

void list_requests_cmd(char *result)
{
    Request *request_head_copy = request_head;

    while (request_head_copy != NULL)
    {
        snprintf(result + strlen(result), BUFFER_SIZE - strlen(result), "%s\n",
                 request_head_copy->request);
        request_head_copy = request_head_copy->next;
    }

    size_t len = strlen(result);
    if (len > 0 && result[len - 1] == '\n')
    {
        result[len - 1] = '\0';
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
void list_rule_cmd(char *result)
{
    if (rule_head == NULL) return;

    Rule *rule_head_copy = rule_head;

    while (rule_head_copy != NULL)
    {
        if (rule_head_copy->ips != NULL && rule_head_copy->ports != NULL)
        {
            snprintf(result + strlen(result), BUFFER_SIZE - strlen(result),
                     "Rule: %s %s\n", rule_head_copy->ips,
                     rule_head_copy->ports);
        }

        Query *query = rule_head_copy->query;
        while (query != NULL)
        {
            if (strlen(result) < BUFFER_SIZE - 1)
            {
                snprintf(result + strlen(result), BUFFER_SIZE - strlen(result),
                         "Query: %s %s\n", query->ip, query->port);
            }
            query = query->next;
        }
        rule_head_copy = rule_head_copy->next;
    }

    size_t len = strlen(result);
    if (len > 0 && result[len - 1] == '\n')
    {
        result[len - 1] = '\0';
    }
}

char *process_cmd(char *request)
{
    char *cmd = NULL;
    char *cmd_argv = NULL;
    char *result = (char *)malloc(BUFFER_SIZE);
    result[0] = '\0';

    request[strcspn(request, "\n")] = '\0';

    if (strlen(request) == 0)
    {
        snprintf(result, BUFFER_SIZE, "Illegal request");
        return result;
    }

    store_request(&request_head, &request_tail, request);

    if (strchr(request, ' ') == NULL)
    {
        if (strcmp(request, "R") == 0)
        {
            list_requests_cmd(result);
        }
        else if (strcmp(request, "L") == 0)
        {
            list_rule_cmd(result);
        }
        else
        {
            snprintf(result, BUFFER_SIZE, "Illegal request");
        }
        return result;
    }

    split_argv_by_delimiter(request, &cmd, &cmd_argv, ' ');

    if (cmd == NULL || cmd_argv == NULL)
    {
        free(cmd);
        free(cmd_argv);
        snprintf(result, BUFFER_SIZE, "Invalid rule");
        return result;
    }

    // printf("cmd_argv: %s\n", cmd_argv);
    if (strcmp(cmd, "A") == 0)
    {
        if (add_rule_cmd(cmd_argv))
        {
            snprintf(result, BUFFER_SIZE, "Rule added");
        }
        else
        {
            snprintf(result, BUFFER_SIZE, "Invalid rule");
        }
    }
    else if (strcmp(cmd, "C") == 0)
    {
        Connection_t outcome = connect_rule_cmd(cmd_argv);
        if (outcome == ACCEPTED)
        {
            snprintf(result, BUFFER_SIZE, "Connection accepted");
        }
        else if (outcome == REJECTED)
        {
            snprintf(result, BUFFER_SIZE, "Connection rejected");
        }
        else if (outcome == ILLEGAL)
        {
            snprintf(result, BUFFER_SIZE,
                     "Illegal IP address or port specified");
        }
    }
    else if (strcmp(cmd, "D") == 0)
    {
        Deleteion_t outcome = del_rule_cmd(cmd_argv);
        if (outcome == DELETED)
        {
            snprintf(result, BUFFER_SIZE, "Rule deleted");
        }
        else if (outcome == NOT_FOUND)
        {
            snprintf(result, BUFFER_SIZE, "Rule not found");
        }
        else if (outcome == INVALID)
        {
            snprintf(result, BUFFER_SIZE, "Rule invalid");
        }
    }
    else
    {
        snprintf(result, BUFFER_SIZE, "Illegal request");
    }

    free(cmd);
    free(cmd_argv);
    return result;
}

void interactive_mode()
{
    char *result = NULL;
    size_t n = 0;

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
            free(result);
            result = NULL;
        }
    }

    // free input when normal exit (^D)
    free_memory();
}
