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
#include <pthread.h>
#include "/home/pub/include/clients.c"
#include "/home/pub/include/servers.c"
#define HASHSIZE 20
#define LOWERPORT 10000
#define MAXSERVS 200
#define LOCALHOST "150.26.237.19"
#define DEFAULTHOST "150.26.237.19"

#include "ctrl.maple.h"

int func();

int main(int argc, char **argv){
	struct hash *u_vars_hash;	
	int i;
	if((u_vars_hash = malloc((size_t)sizeof(struct hash)*HASHSIZE)) == NULL){
		printf("failed : malloc(hash) -- exit,\n");
		exit(1);
	}
	for(i=0;i<HASHSIZE;i++){
		u_vars_hash[i].type = "0";
		u_vars_hash[i].var = '\0';
		u_vars_hash[i].var_name = '\0';
	}
	printf("%s\t",u_vars_hash[2].type);
	printf("%s\t",u_vars_hash[2].var);
	printf("%s\t\n",u_vars_hash[2].var_name);
	func(2,u_vars_hash);
	printf("%s\t",u_vars_hash[2].type);
	printf("%s\t",u_vars_hash[2].var);
	printf("%s\t\n",u_vars_hash[2].var_name);

	//loop();
	return(0);
}

int func(int *uvhash_size, struct hash *uvhash){
	char i[] = "777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777";
	uvhash[2].type = "8";	
	uvhash[2].var = "777999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
	uvhash[2].var_name = i;
	return(0);
}


#include "ctrl.maple-funcs.c"
