CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c11

all: s21_cat test module_test

rebuild: clean s21_cat

s21_cat: s21_cat.h
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test:
	@cp ../../materials/linters/.clang-format 
	@clang-format -n s21_cat.c 
	@clang-format -n s21_cat.h
	@cppcheck --enable=all --suppress=missingIncludeSystem  s21_cat.c s21_cat.h

module_test:
	@bash main_test.sh

clean:
	rm  s21_cat

