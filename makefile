advcalc:advcalc.o functions.o
	gcc advcalc.o functions.o -o advcalc

advcalc.o:advcalc.c
	gcc -c advcalc.c -o advcalc.o

functions.o:functions.c
	gcc -c functions.c -o functions.o

clean:
	rm -f *.o advcalc
