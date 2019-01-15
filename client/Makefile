
#makefile client

proiect: proiect.o client.o grafica.o
	gcc -g -o proiect proiect.o client.o grafica.o -l sigil -w

proiect.o: proiect.c grafica.h client.h
	gcc -g -o proiect.o -c proiect.c

grafica.o: grafica.c grafica.h client.h
	gcc -g -o grafica.o -c grafica.c

client.o: client.c client.h
	gcc -g -o client.o -c client.c
