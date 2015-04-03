#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>
#include "/home/pub/include/clients.c"

#define BUF_LEN 1024
#define COMM_LEN 4096
#define VLEN 8192

int main(int argc, char **argv){
	char hostname[BUF_LEN];
	int port = 0;
	char com[COMM_LEN];
	//char v[VLEN];
	if(argc == 4){
		sscanf(argv[1],"%s",hostname);
		sscanf(argv[2],"%d",&port);
		sscanf(argv[3],"%s",com);
		printf("%s",maple_client(hostname,port,com,0));
	}else{
		printf("number of args: 4.\n");
		exit(1);
	}
	return(0);
}
