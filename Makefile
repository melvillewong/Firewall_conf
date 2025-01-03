CFLAGS = -Wall -Werror -g
LDFLAGS = -lcriterion

all: main.out test.out

MAIN_SRC = src/main.c src/server.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

TEST_SRC = tests/test.c src/server.c
TEST_OBJ = $(TEST_SRC:.c=.o)

main.out: $(MAIN_OBJ)
	gcc -o $@ $^

test.out: $(TEST_OBJ)
	gcc -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

tests/%.o: tests/%.c
	gcc $(CFLAGS) -c $< -o $@

static-check:
	cppcheck --enable=all --suppress=missingIncludeSystem .

clean:
	rm src/*.o test/*.o main.out test.out
