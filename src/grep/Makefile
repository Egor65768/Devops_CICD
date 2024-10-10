all : clean s21_grep

s21_grep: s21_grep.o
	gcc -Wextra -Werror -Wall s21_grep.o -o s21_grep

s21_grep.o : s21_grep.c
	gcc -c -Wextra -Werror -Wall s21_grep.c -o s21_grep.o 

clean : 
	rm -rf s21_grep 
	rm -rf s21_grep.o
style: 
	clang-format -style=google -n *.c *.h

format: 
	clang-format -style=google -i *.c *.h

rebuild : clean all