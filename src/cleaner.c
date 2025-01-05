#include "../include/cleaner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_queries(Rule *rule)
{
    Query *query_copy = rule->query;

    while (query_copy != NULL)
    {
        Query *query_temp = query_copy;
        query_copy = query_copy->next;

        free(query_temp->ip);
        query_temp->ip = NULL;
        free(query_temp->port);
        query_temp->port = NULL;
        free(query_temp);
    }
}

int free_rule(char *ips, char *ports)
{
    Rule *rule_head_copy = rule_head;
    Rule *prev = NULL;

    while (rule_head_copy != NULL)
    {
        int is_match_ips = strcmp(rule_head_copy->ips, ips);
        int is_match_ports = strcmp(rule_head_copy->ports, ports);

        if (is_match_ips == 0 && is_match_ports == 0)
        {
            if (prev == NULL)
            {
                rule_head = rule_head_copy->next;
            }
            else
            {
                prev->next = rule_head_copy->next;
            }

            free_queries(rule_head_copy);
            rule_head_copy->query = NULL;

            free(rule_head_copy->ips);
            free(rule_head_copy->ports);
            free(rule_head_copy);
        }
        prev = rule_head_copy;
        rule_head_copy = rule_head_copy->next;
        return 1;
    }
    return 0;
}

void free_memory()
{
    Request *request_head_copy = request_head;
    Request *request_temp = NULL;
    Rule *rule_head_copy = rule_head;
    Rule *rule_temp = NULL;

    // free all requests
    while (request_head_copy != NULL)
    {
        request_temp = request_head_copy;
        request_head_copy = request_head_copy->next;

        free(request_temp->request);
        free(request_temp);
    }
    request_head = NULL;

    // free all rules with its queries
    while (rule_head_copy != NULL)
    {
        rule_temp = rule_head_copy;
        rule_head_copy = rule_head_copy->next;

        free(rule_temp->ips);
        free(rule_temp->ports);
        free_queries(rule_temp);
        free(rule_temp);
    }
    rule_head = NULL;
}
