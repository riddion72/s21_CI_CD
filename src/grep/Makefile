FLAGS = -Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep

test:
	make rebuild
	bash test_how_it_grep.sh

check:
	clang-format -style=Google -i *.c
	clang-format -style=Google -n *.c

rebuild:
	make clean
	make all

clean:
	rm -f s21_grep
