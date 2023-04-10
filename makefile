advcalc:advcalc.o functions.o
	gcc advcalc.o functions.o -o advcalc2ir

advcalc.o:advcalc.c functions.h
	gcc -c advcalc.c -o advcalc.o

functions.o:functions.c functions.h
	gcc -c functions.c -o functions.o

clean:
	rm -f *.o advcalc2ir *.ll
