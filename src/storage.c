#include "../include/storage.h"
#include "../include/checker.h"
#include "../include/cleaner.h"
#include <stdlib.h>
#include <string.h>

Request *request_head = NULL;
Request *request_tail = NULL;
Rule *rule_head = NULL;

void store_request(Request **head, Request **tail, char *request)
{
    Request *request_new = (Request *)malloc(sizeof(Request));

    request_new->request = strdup(request);

    if (*head == NULL)
    {
        *head = request_new;
        *tail = request_new;
        request_new->next = NULL;
        request_new->prev = NULL;
    }
    else
    {
        (*tail)->next = request_new;
        request_new->prev = *tail;
        request_new->next = NULL;
        *tail = request_new;
    }
}

void store_rule(char *ips, char *ports)
{
    Rule *rule_new = (Rule *)malloc(sizeof(Rule));
    if (rule_new)
    {
        rule_new->ips = strdup(ips);
        rule_new->ports = strdup(ports);
        rule_new->next = rule_head;
        rule_new->query = NULL;
        rule_head = rule_new;
    }
}

int store_query(char *ip, char *port)
{
    Rule *rule_head_copy = rule_head;
    while (rule_head_copy != NULL)
    {
        char *rule_ips = rule_head_copy->ips;
        char *rule_ports = rule_head_copy->ports;
        Query *query_head = rule_head_copy->query;
        Query *query_new = (Query *)malloc(sizeof(Query));

        if (rule_ips != NULL && rule_ports != NULL)
        {
            if (is_ip_in_rule(rule_ips, ip) &&
                is_port_in_rule(rule_ports, port))
            {
                query_new->ip = (char *)malloc(strlen(ip) + 1);
                strcpy(query_new->ip, ip);

                query_new->port = (char *)malloc(strlen(port) + 1);
                strcpy(query_new->port, port);

                query_new->next = query_head;
                rule_head_copy->query = query_new;
                return 1;
            }
        }
        rule_head_copy = rule_head_copy->next;
    }
    return 0;
}

int del_rule(char *ips, char *ports)
{
    Rule *rule_head_copy = rule_head;
    Rule *prev = NULL;
    int is_match_ips = 0;
    int is_match_ports = 0;

    while (rule_head_copy != NULL)
    {
        is_match_ips = !strcmp(rule_head_copy->ips, ips);
        is_match_ports = !strcmp(rule_head_copy->ports, ports);

        if (is_match_ips && is_match_ports)
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

            return 1;
        }
        prev = rule_head_copy;
        rule_head_copy = rule_head_copy->next;
    }

    return 0;
}
