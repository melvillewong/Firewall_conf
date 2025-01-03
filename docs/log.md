2025-01-01

- Fix still reachable memory for input
    > Fixed with globally define input

2025-01-02

- Fix inexecutable test.out, prob something wrong with test.c
    > Fixed with separating main() from the functions to be tested, so that I created main.c for main() and server.c for functions
    > Modified Makefile mainly, linking server.o and main.o
