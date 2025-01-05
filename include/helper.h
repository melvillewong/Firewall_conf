#ifndef HELPER_H
#define HELPER_H

void split_argv_by_delimiter(char *input, char **fst_part, char **scd_part,
                             char delimiter);
void handle_sig(int sig);

#endif
