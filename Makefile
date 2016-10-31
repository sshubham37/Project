

project: billing.o
	cc billing.o -lncurses -o project

billing.o: billing.c billing.h
		cc -c billing.c -lncurses
