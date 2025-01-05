#ifndef STORAGE_H
#define STORAGE_H

typedef struct Request
{
    char *request;
    struct Request *next;
    struct Request *prev;
} Request;

typedef struct Query
{
    char *ip;
    char *port;
    struct Query *next;
} Query;

typedef struct Rule
{
    char *ips;
    char *ports;
    Query *query;
    struct Rule *next;
} Rule;

extern Request *request_head;
extern Request *request_tail;
extern Rule *rule_head;

void store_request(Request **head, Request **tail, char *request);
void store_rule(char *ip, char *port);
int store_query(char *ip, char *port);
int del_rule(char *ip, char *port);

#endif
