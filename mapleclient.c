#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUF_LEN 1024
#define COMM_LEN 4096

int main(int argc, char **argv){
	int s;
	struct hostent *servhost;
	struct sockaddr_in server;
	char hostname[BUF_LEN] = "localhost";
	int port = 0;
	char com[COMM_LEN];
	int com_len;
	int con_stat;
	char buf[BUF_LEN];
	int read_size;
	char c;
	int i = 0;
	if(argc == 3){
		sscanf(argv[1],"%s",hostname);
		sscanf(argv[2],"%d",&port);
		while(scanf("%c",&c) != EOF){
			if(c != '\n'){
				com[i] = c;
				i++;
			}
		}
		com[i] = '\0';
		//printf(":%s:",com);
		com_len = strlen(com);
		servhost = gethostbyname(hostname);
		bzero((char *)&server,sizeof(server));
		server.sin_family = AF_INET;
		bcopy(servhost->h_addr,(char *)&server.sin_addr,servhost->h_length);
		server.sin_port = htons(port);
		if((s = socket(AF_INET,SOCK_STREAM,0)) < 0){
			printf("failed : generating socket.\n");
			exit(1);
		}
		if((con_stat = connect(s,(struct sockaddr *)&server,sizeof(server))) == -1){
			printf("failed : connect().\n");
			exit(1);
		}
		write(s,com,com_len);
		while(1){
			read_size = read(s,buf,BUF_LEN);
			if(read_size > 0){
				write(1,buf,read_size);
			}else{
				break;
			}
		}
		close(s);
	}else if(argc == 4){
		sscanf(argv[1],"%s",hostname);
		sscanf(argv[2],"%d",&port);
		sscanf(argv[3],"%s",com);
		//printf(":%s:",com);
		com_len = strlen(com);
		servhost = gethostbyname(hostname);
		bzero((char *)&server,sizeof(server));
		server.sin_family = AF_INET;
		bcopy(servhost->h_addr,(char *)&server.sin_addr,servhost->h_length);
		server.sin_port = htons(port);
		if((s = socket(AF_INET,SOCK_STREAM,0)) < 0){
			printf("failed : generating socket.\n");
			exit(1);
		}
		if((con_stat = connect(s,(struct sockaddr *)&server,sizeof(server))) == -1){
			printf("failed : connect().\n");
			exit(1);
		}
		write(s,com,com_len);
		while(1){
			read_size = read(s,buf,BUF_LEN);
			if(read_size > 0){
				write(1,buf,read_size);
			}else{
				break;
			}
		}
		close(s);
	}else{
		printf("number of args: 4.\n");
		exit(1);
	}
	return(0);
}
