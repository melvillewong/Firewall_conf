#ifndef CLEANER_H
#define CLEANER_H

#include "../include/storage.h"

void free_queries(Rule *rule);
int free_rule(char *ips, char *ports);
void free_lists();
void cleanup(int sig);

#endif
