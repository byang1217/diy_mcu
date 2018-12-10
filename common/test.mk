all: 
	gcc -g -O0 -I. -D__SHELL_TEST print.c shell.c string.c -o test
