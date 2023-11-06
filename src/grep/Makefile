CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

test:
	@clang-format -n s21_grep.c 
	@cppcheck --enable=all --suppress=missingIncludeSystem  s21_grep.c

clean:
	rm  s21_grep

