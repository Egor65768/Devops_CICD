all: clean s21_cat

s21_cat: s21_cat.o
	gcc -Wextra -Werror -Wall s21_cat.o -o s21_cat

s21_cat.o : s21_cat.c
	gcc -c -Wextra -Werror -Wall s21_cat.c -o s21_cat.o 

clean : 
	rm -rf s21_cat 
	rm -rf s21_cat.o
style: 
	clang-format -style=google -n *.c *.h

rebuild : clean all