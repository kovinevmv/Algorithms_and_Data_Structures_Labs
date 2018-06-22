Kursach: struct.h main.o function.o createlist.o
	gcc main.o -o Kursach_Main.out createlist.o function.o
	rm *.o
main.o: main.c struct.h
	gcc -c main.c
createlist.o: createlist.c createlist.h struct.h function.h
	gcc -c createlist.c
function.o: function.c function.h struct.h
	gcc -c function.c
