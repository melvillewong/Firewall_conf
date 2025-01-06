#include "../include/checker.h"
#include "../include/global.h"
#include "../include/helper.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// check ip format xxx.xxx.xxx.xxx or <IP1>-<IP2>
int check_ip(const char *ip)
{
    regex_t regex;
    int ret;
    const char *pattern =
        "^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)\\.){3}(25[0-5]|2[0-4]"
        "[0-9]|1[0-9]{2}|[1-9][0-9]?|0)(-((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]["
        "0-9]?|0)\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0))?$";

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

    if (ret)
    {
        // printf("Invalid IP\n");
        return 0;
    }
    // else
    // {
    //     printf("Valid IP\n");
    // }

    return 1;
}

// check port format xxxxx or <port1>-<port2>
int check_port(const char *port)
{
    regex_t regex;
    int ret;
    const char *pattern =
        "(^(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{"
        "0,4}|[1-9][0-9]{0,3}|0)-(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-"
        "4][0-9]{3}|[1-5][0-9]{0,4}|[1-9][0-9]{0,3}|0)$|^(6553[0-5]|655[0-2][0-"
        "9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{0,4}|[1-9][0-9]{0,3}|0)$"
        ")";

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

    if (ret)
    {
        // printf("Invalid Port\n");
        return 0;
    }
    // else
    // {
    //     printf("Valid IP\n");
    // }

    return 1;
}

int check_ip_range(char *ip1, char *ip2)
{
    int *fst_parts_arr = (int *)malloc(4 * sizeof(int));
    int *scd_parts_arr = (int *)malloc(4 * sizeof(int));
    int is_right_value = 1;

    // populate ip1 into fst_parts_arr
    char *ip1_copy = strdup(ip1);
    char *part = strtok(ip1_copy, ".");

    for (int i = 0; i < 4; i++)
    {
        fst_parts_arr[i] = atoi(part);
        part = strtok(NULL, ".");
    }

    // populate ip2 into scd_parts_arr if exist
    if (ip2 != NULL)
    {
        char *ip2_copy = strdup(ip2);
        part = strtok(ip2_copy, ".");

        for (int i = 0; i < 4; i++)
        {
            scd_parts_arr[i] = atoi(part);
            part = strtok(NULL, ".");
        }

        for (int i = 0; i < 4; i++)
        {
            if (fst_parts_arr[i] < scd_parts_arr[i])
            {
                break;
            }
            else if (fst_parts_arr[i] == scd_parts_arr[i])
            {
                continue;
            }
            else
            {
                is_right_value = 0;
            }
        }
        free(ip2_copy);
    }
    free(fst_parts_arr);
    free(scd_parts_arr);
    free(ip1_copy);

    return is_right_value;
}

int check_port_range(char *port1, char *port2)
{
    if (port2 != NULL)
    {
        return atoi(port1) <= atoi(port2);
    }
    return 1;
}

int is_valid_ip_port(char *value, Network_t ip_or_port)
{
    char *fst_value = NULL;
    char *scd_value = NULL;

    // check format
    if (ip_or_port == IP)
    {
        if (!check_ip(value)) return 0;
    }
    else if (ip_or_port == PORT)
    {
        if (!check_port(value)) return 0;
    }

    // separate if ranged
    if (strchr(value, '-'))
    {
        split_argv_by_delimiter(value, &fst_value, &scd_value, '-');
    }
    else
    {
        fst_value = strdup(value);
    }

    // check range
    if (ip_or_port == IP)
    {
        if (!check_ip_range(fst_value, scd_value)) return 0;
    }
    else if (ip_or_port == PORT)
    {
        if (!check_port_range(fst_value, scd_value)) return 0;
    }

    free(fst_value);
    if (scd_value != NULL) free(scd_value);

    return 1;
}

int is_ip_in_rule(char *rule_ips, const char *connect_ip)
{
    char *rule_ip1 = NULL;
    char *rule_ip2 = NULL;
    int is_rule2_ip = 0;
    int is_in_rule = 1;

    int *fst_parts_arr = (int *)malloc(4 * sizeof(int));
    int *scd_parts_arr = (int *)malloc(4 * sizeof(int));
    int *connect_parts_arr = (int *)malloc(4 * sizeof(int));

    // check if single or ranged, and assign
    if (strchr(rule_ips, '-'))
    {
        split_argv_by_delimiter(rule_ips, &rule_ip1, &rule_ip2, '-');
        is_rule2_ip = 1;
    }
    else
    {
        rule_ip1 = strdup(rule_ips);
    }

    // populate rule_ip1 into fst_parts_arr
    char *part = strtok(rule_ip1, ".");
    for (int i = 0; i < 4 && part != NULL; i++)
    {
        fst_parts_arr[i] = atoi(part);
        part = strtok(NULL, ".");
    }

    // populate rule_ip2 into scd_parts_arr if exist
    if (is_rule2_ip && rule_ip1 != NULL && rule_ip2 != NULL)
    {
        part = strtok(rule_ip2, ".");
        for (int i = 0; i < 4 && part != NULL; i++)
        {
            scd_parts_arr[i] = atoi(part);
            part = strtok(NULL, ".");
        }
    }

    char *connect_ip_copy = strdup(connect_ip);
    if (connect_ip_copy != NULL)
    {
        part = strtok(connect_ip_copy, ".");
        for (int i = 0; i < 4 && part != NULL; i++)
        {
            connect_parts_arr[i] = atoi(part);
            part = strtok(NULL, ".");
        }
    }
    free(connect_ip_copy);

    for (int i = 0; i < 4; i++)
    {
        if (is_rule2_ip)
        {
            if (!(fst_parts_arr[i] <= connect_parts_arr[i] &&
                  connect_parts_arr[i] <= scd_parts_arr[i]))
            {
                is_in_rule = 0;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (fst_parts_arr[i] != connect_parts_arr[i])
            {
                is_in_rule = 0;
            }
        }
    }

    free(rule_ip1);
    if (is_rule2_ip) free(rule_ip2);
    free(fst_parts_arr);
    free(scd_parts_arr);
    free(connect_parts_arr);

    return is_in_rule;
}

int is_port_in_rule(char *rule_ports, const char *connect_port)
{
    char *rule_port1 = NULL;
    char *rule_port2 = NULL;

    if (strchr(rule_ports, '-'))
    {
        split_argv_by_delimiter(rule_ports, &rule_port1, &rule_port2, '-');
    }
    else
    {
        rule_port1 = strdup(rule_ports);
    }

    if (rule_port2 != NULL)
    {
        if (!(atoi(rule_port1) <= atoi(connect_port) &&
              atoi(connect_port) <= atoi(rule_port2)))
        {
            free(rule_port1);
            free(rule_port2);
            return 0;
        }
    }
    else
    {
        if (!(atoi(rule_port1) == atoi(connect_port)))
        {
            free(rule_port1);
            return 0;
        }
    }

    free(rule_port1);
    if (rule_port2 != NULL) free(rule_port2);

    return 1;
}
