CFLAGS = -Wall -Werror -g
LDFLAGS = -lcriterion

all: server.out test.out

FUNC_SRC = $(wildcard src/*.c)

MAIN_SRC = server.c $(FUNC_SRC)
MAIN_OBJ = $(MAIN_SRC:.c=.o)

TEST_SRC = $(wildcard tests/*.c) $(FUNC_SRC)
TEST_OBJ = $(TEST_SRC:.c=.o)

server.out: $(MAIN_OBJ)
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
	rm $(MAIN_OBJ) $(TEST_OBJ) server.out test.out
