all:advcalc.o functions.o
	gcc advcalc.o functions.o -o advcalc2ir

advcalc.o:advcalc.c functions.h
	gcc -c advcalc.c -o advcalc.o

functions.o:functions.c functions.h
	gcc -c functions.c -o functions.o

file.adv:
	echo 'test = 5\ntest + 1' > file.adv

file:file.s
	clang -o file file.s

file.s:file.ll
	llc -o file.s file.ll

file.ll:all file.adv
	./advcalc2ir file.adv

test:file.ll
	lli file.ll

testerror:all file.adv
	echo 'this is error' > file.adv
	./advcalc2ir file.adv

clean:
	rm -f *.o advcalc2ir *.ll *.s file file.* test.*
