forking: client fserver

fserver: forking_server.o networking.o
	gcc -o server forking_server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

client.o: client.c networking.h
	gcc -c client.c

forking_server.o: forking_server.c networking.h
	gcc -c forking_server.c

networking.o: networking.c networking.h
	gcc -c networking.c

chub: chub.o chub.o
	gcc -o chub chub.o networking.o

chub.o: chub.c networking.h
	gcc -c chub.c

clean:
	rm *.o
	rm *~
