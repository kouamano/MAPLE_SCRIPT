#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>
#include "/home/pub/include/clients.c"
#include "/home/pub/include/servers.c"

int main(int argc, char **argv){
	char i;
	char server[2048];
	int port;
	char state[8192];
	char c;
	if(argc == 3){
		sscanf(argv[1],"%s",&server);
		sscanf(argv[2],"%d",&port);
		i = 0;
		while(scanf("%c",&c) != EOF){
			if(c != '\n'){
				state[i] = c;
				i++;
			}
		}
		state[i] = '\0';
	}else if(argc == 4){
		sscanf(argv[1],"%s",&server);
		sscanf(argv[2],"%d",&port);
		sscanf(argv[3],"%s",&state);
	}else{
		printf("set host, port, and statement\n");
		exit(1);
	}
	maple_client(server,port,state);
	exit(0);
}
