#include <sys/socket.h>
#include <sys/types.h>
#include<netdb.h>

int sd;
sd = socket(AF_INET, SOCK_STREAM,0 );
struct addrinfo *hints, *results;

hints = (struct addrinfo*) calloc(1,sizeof(struct(struct addrinfo));
hints -> ai_faimly = AF_INET;
hints-> ai_socktupe = SOCK_STREAM //TCP socket
hints -> ai_flags =AI_PASSIVE // only needed on server
getaddrinfo(NULL, “80”, hints, &results); //Server sets node to NULL

connect(sd, results->ai_addr, results->ai_addrlen);
	  
free(hints);
freeaddrinfo(results);
