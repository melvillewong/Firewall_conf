2025-01-01

- Fix still reachable memory for input
    > Fixed with globally define input

2025-01-02

- Fix inexecutable test.out, prob something wrong with test.c
    > Fixed with separating main() from the functions to be tested, so that I created main.c for main() and server.c for functions
    > Modified Makefile mainly, linking server.o and main.o

2025-01-04

- TODO: `is_ip_in_rule` and `is_port_in_rule` in storage.c
    > Done

2025-01-05

- TODO: Check out how to debug
- Fix invalid IP and port (Entrypoint: `is_valid_ip_port`)
    > Fixed

> Completed Server Interactive mode with no memory leak

2025-01-06

- TODO: Test out any memory leak for running server with connection mode
