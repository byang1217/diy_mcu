all: 
	gcc -g -O0 -I. -D__SHELL_TEST prf.c sprintf.c print.c shell.c string.c -o test
