all : shell.o
	gcc -o shell shell.o
run : ./shell
	./shell
shell.o : shell.c
	gcc -c shell.c
clean : 
	rm *.o ./shell ./a.out
fix : shell.c
	gcc -g shell.c
	gdb ./a.out
