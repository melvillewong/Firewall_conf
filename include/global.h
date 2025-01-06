#ifndef GLOBAL_H
#define GLOBAL_H

#define BUFFER_SIZE 256

typedef enum
{
    IP,
    PORT,
} Network_t;

typedef enum
{
    ACCEPTED,
    REJECTED,
    ILLEGAL,
} Connection_t;

typedef enum
{
    DELETED,
    NOT_FOUND,
    INVALID,
} Deleteion_t;

extern char *input;

#endif
