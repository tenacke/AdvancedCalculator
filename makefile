all:advcalc.o functions.o
	gcc advcalc.o functions.o -o advcalc2ir

advcalc.o:advcalc.c functions.h
	gcc -c advcalc.c -o advcalc.o

functions.o:functions.c functions.h
	gcc -c functions.c -o functions.o

file:file.s
	clang -o file file.s

file.s:file.ll
	llc -o file.s file.ll

clean:
	rm -f *.o advcalc2ir *.ll
