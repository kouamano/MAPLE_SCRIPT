#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>
#include <pthread.h>
//#include "/home/pub/include/clients.c"
#include "clients.c"
//#include "/home/pub/include/servers.c"
#include "servers.c"
#define HASHSIZE 200
#define LOWERPORT 10000
#define MAXSERVS 200
#define MAXSTATELEN 8192
#define MAXMRETLEN 8192
#define LOCALHOST "150.26.237.19"
#define DEFAULTHOST "150.26.237.19"

struct hash {
	char *type;
	char *name;
	char *var;
};

int is_alnum_char();
int to_maple_statement();
int statement_interpret();

int main(int argc, char **argv){
	int i = 0;
	int escape = 0;
	signed char c;
	int hashsize = 0;
	char *statement;
	struct hash *user_vars;
	if((statement = calloc((size_t)sizeof(char),MAXSTATELEN)) == NULL){
		printf("failed : malloc(statement) -- exit.\n");
		exit(1);
	}
	if((user_vars = calloc((size_t)sizeof(struct hash),HASHSIZE)) == NULL){
		printf("failed : malloc(user_vars) -- exit.\n");
		exit(1);
	}
	/* (*main loop */
	printf(" << ");
	while(scanf("%c",&c) != EOF){
		if((escape == 1)&&(c == '\\')){
			statement[i] = c;
			i++;
		}else if((escape == 0)&&(c == '\\')){
			statement[i] = c;
			i++;
			escape = 1;
		}else if((escape == 1)&&(c == '\n')){
			i--;
			escape = 0;
		}else if((escape == 0)&&(c == '\n')){
			statement[i] = '\0';
			i = 0;
			printf("S::%s::\n",statement);
			statement_interpret(statement,&hashsize,user_vars);
			printf(" << ");
		}else if(c != '\n'){
			escape = 0;
			statement[i] = c;
			i++;
		}
	}
	/* *) */
	return(0);
}

int to_maple_statement(char *_statement, int _vars_size, struct hash *_vars, char *m_statement){
	/* m_statement must be already allocated. */
	int i = 0;
	int j = 0;
	int n = 0;
	int x = 0;
	int escape = 0;
	int sub = 0;
	char *sub_name;
	char *sub_var;
	int _statement_len = 0;
	_statement_len = strlen(_statement);
	sub_name = calloc((size_t)sizeof(char),256);
	//printf("----- /to_maple_statement() -----\n");
	//printf("_vars_size:%d\n",_vars_size);
	for(i=0;i<_statement_len;i++){
		if(_statement[i] == '\\'){
			//printf("ESC\n");
			escape++;
			if(escape == 2){
				m_statement[n] = _statement[i];
				n++;
				escape = 0;
			}
		}else if((_statement[i] == '#')&&(escape == 0)){
			if(sub == 0){
				sub = 1;
				sub_name[j] = _statement[i];
				j++;
				//printf("sub_name1:%s\n",sub_name);
			}else if(sub == 1){
				m_statement[n] = '\0';
				sub_name[j] = '\0';
				//printf("sub_name2:%s\n",sub_name);
				sub_var = sub_name;
				for(x=0;x<_vars_size;x++){
					if(strcmp(_vars[x].name,sub_name) == 0){
						sub_var = _vars[x].var;
					}
				}
				//printf("sub_var2:%s\n",sub_var);
				strcat(m_statement,sub_var);
				//printf("m_statement2:%s\n",m_statement);
				n = strlen(m_statement);
				j = 0;
				sub_name[j] = _statement[i];
				j++;
			}
		}else if((((int)_statement[i] >= 97)&&((int)_statement[i] <= 122))||(((int)_statement[i] >= 65)&&((int)_statement[i] <= 90))||(((int)_statement[i] >= 48)&&((int)_statement[i] <= 57))){
			if(sub == 1){
				sub_name[j] = _statement[i];
				j++;
			}else if(sub == 0){
				m_statement[n] = _statement[i];
				n++;
			}
			escape = 0;
		}else{
			sub = 0;
			m_statement[n] = '\0';
			sub_name[j] = '\0';
			//printf("sub_name3:%s\n",sub_name);
			sub_var = sub_name;
			for(x=0;x<_vars_size;x++){
				if(strcmp(_vars[x].name,sub_name) == 0){
					sub_var = _vars[x].var;
				}
			}
			//printf("sub_var3:%s\n",sub_var);
			strcat(m_statement,sub_var);
			//printf("m_statement3:%s\n",m_statement);
			n = strlen(m_statement);
			j = 0;
			m_statement[n] = _statement[i];
			n++;
			escape = 0;
		}
	}
	m_statement[n] = '\0';
	sub_name[j] = '\0';
	//printf("sub_name4:%s\n",sub_name);
	sub_var = sub_name;
	//printf("sub_var4:%s\n",sub_var);
	for(x=0;x<_vars_size;x++){
		if(strcmp(_vars[x].name,sub_name) == 0){
			sub_var = _vars[x].var;
		}
	}
	strcat(m_statement,sub_var);
	//printf("----- to_maple_statement()/ -----\n");
	return(0);
}

int statement_interpret(char *statement, int *vars_size, struct hash *vars){
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int statement_len;
	int inq = 0;
	int subs = 0;
	int dsubs = 0;
	char *s_port;
	int i_port;
	char *var_name;
	char *maple_statement;
	char *new_maple_statement;
	int sizeup = 0;
	int rep = 0;
	int direct_print = 0;
	char *maple_ret;
	int firstport = -1;
	int lastport = -1;
	statement_len = strlen(statement);
	if((var_name = calloc((size_t)sizeof(char),statement_len-1)) == NULL){
		printf("failed : malloc(var_name).\n");
		return(-1);
	}
	if((s_port = calloc((size_t)sizeof(char),statement_len-3)) == NULL){
		printf("failed : malloc(s_port).\n");
		return(-1);
	}
	if(statement_len-4 > 0){
		if((maple_statement = calloc((size_t)sizeof(char),statement_len-4)) == NULL){
			printf("failed : malloc(maple_statement).\n");
			return(-1);
		}
	}
	if((maple_ret = calloc((size_t)sizeof(char),MAXMRETLEN)) == NULL){
		printf("failed : malloc(maple_ret).\n");
		return(-1);
	}
	if((new_maple_statement = calloc((size_t)sizeof(char),statement_len*2)) == NULL){
		printf("failed : malloc(new_maple_statement).\n");
		return(-1);
	}
	if(statement[0] == '\\'){
		printf("--command mode--\n");
		if(strcmp(statement,"\\exit") == 0){
			exit(0);
		}else if(strcmp(statement,"\\help") == 0){
			printf("\\allclear\t\t\tall-clear user variables.\n");
			printf("\\allnames\t\t\tscan user variable-hash at maximum size.\n");
			printf("\\exit\t\t\t\texit the program.\n");
			printf("\\help\t\t\t\tprint this message.\n");
			printf("\\names\t\t\t\tlist user variables.\n");
			printf("\\scan <port>-<port>\t\tscan at port <port>.\n");
			printf("\\startmaple <port>-<port>\tstart maple at port <port>.\n");
		}else if(strcmp(statement,"\\allnames") == 0){
			for(i=0;i<HASHSIZE;i++){
				printf("%s\t",vars[i].type);
				printf("%s\t",vars[i].name);
				printf("%s\n",vars[i].var);
			}
		}else if(strcmp(statement,"\\names") == 0){
			for(i=0;i<*vars_size;i++){
				printf("%s\t",vars[i].type);
				printf("%s\t",vars[i].name);
				printf("%s\n",vars[i].var);
			}
		}else if(strncmp(statement,"\\clear",6) == 0){
		}else if(strcmp(statement,"\\allclear") == 0){
			for(i=0;i<HASHSIZE;i++){
				vars[i].type = NULL;
				vars[i].name = NULL;
				vars[i].var = NULL;
			}
			*vars_size = 0;
		}else if(strncmp(statement, "\\startmaple",11) == 0){
			if(statement_len < 12){
				return(-1);
			}
			sscanf(statement+11,"%d-%d",&firstport,&lastport);
			if((firstport >= LOWERPORT)&&(lastport < LOWERPORT)){
				start_maple(firstport);
			}else if((firstport >= LOWERPORT)&&(lastport >= LOWERPORT)){
				for(i=firstport;i<=lastport;i++){
					start_maple(i);
				}
			}else{
				return(-1);
			}
		}else if(strncmp(statement,"\\scan",5) == 0){
			if(statement_len < 6){
				return(-1);
			}
			sscanf(statement+5,"%d-%d",&firstport,&lastport);
			printf("Port\tAlive\n");
			if((firstport >= LOWERPORT)&&(lastport < LOWERPORT)){
				printf("%d\t%s\n",firstport,maple_client(LOCALHOST,firstport,"1",1,0,1));
			}else if((firstport >= LOWERPORT)&&(lastport >= LOWERPORT)){
				for(i=firstport;i<=lastport;i++){
					printf("%d\t%s\n",i,maple_client(LOCALHOST,i,"1",1,0,1));
				}
			}else{
				return(-1);
			}
		}else if(strncmp(statement,"\\test",5) == 0){
			printf("---test mode---\n");
			to_maple_statement(maple_statement,*vars_size,vars,new_maple_statement);
			printf("N::%s::\n",new_maple_statement);
		}
	}else if(statement[0] == '#'){
		for(i=0;i<statement_len;i++){
			if(statement[i] == ';'){
				var_name[j] = '\0';
				inq = 1;
				j++;
				break;
			}else if(statement[i] == ':'){
				var_name[j] = '\0';
				subs = 1;
				j++;
				break;
			}else{
				var_name[j] = statement[i];
				j++;
			}
		}
		if(subs == 1){
			//printf("::::%c::::\n",statement[i]);
			//printf("len:%d\n",statement_len);
			if(statement_len-1-i < 1){
				return(-1);
			}
			//printf("--substitution mode--\n");
			for(i=j;i<statement_len;i++){
				if(statement[i] == '<'){
					s_port[k] = '\0';
					k++;
					break;
				}else{
					s_port[k] = statement[i];
					k++;
				}
			}
			for(i=j+k;i<statement_len;i++){
				maple_statement[l] = statement[i];
				l++;
			}
			maple_statement[l] = '\0';
			//printf("M:::%s:::\n",maple_statement);
			if(strlen(s_port) <= 1){
				dsubs = 1;
				//printf("---direct substitution mode---\n");
				//printf("var_name::::%s::::\n",var_name);
				/* (*substitution */
				for(i=0;i<*vars_size;i++){
					if(vars[i].name != NULL){
						if(strcmp(vars[i].name,var_name) == 0){
							vars[i].var = maple_statement;
							rep = 1;
						}
					}
				}
				if(rep == 0){
					vars[i].name = var_name;
					vars[i].var = maple_statement;
					sizeup++;
				}
				*vars_size = *vars_size + sizeup;
				rep = 0;
				sizeup = 0;
				/* *) */
			}else{
				dsubs = 0;
				//printf("---maple substitution mode---\n");
				if(s_port[strlen(s_port)-1] == '&'){
					printf("----threading subsitisution mode----\n");
					i_port = atoi(s_port);
				}else{
					//printf("----non-threading subsitisution mode----\n");
					i_port = atoi(s_port);
					to_maple_statement(maple_statement,*vars_size,vars,new_maple_statement);
					printf("N:::%s:::\n",new_maple_statement);
					maple_ret = maple_client(LOCALHOST,i_port,new_maple_statement,0,0,1);
					//maple_ret = maple_client(LOCALHOST,i_port,maple_statement,0,0,1);
					//printf("R::%s::\n",maple_ret);
					/* (* substitution*/
					for(i=0;i<*vars_size;i++){
						if(vars[i].name != NULL){
							if(strcmp(vars[i].name,var_name) == 0){
								vars[i].var = maple_ret;
								rep = 1;
							}
						}
					}
					if(rep == 0){
						vars[i].name = var_name;
						vars[i].var = maple_ret;
						sizeup++;
					}
					*vars_size = *vars_size + sizeup;
					rep = 0;
					sizeup = 0;
					/* *) */
				}
			}
		}else if(inq == 1){
			//printf("--inquiry mode--\n");
			strncpy(var_name,statement,statement_len-1);
			//printf("V:::%s:::\n",var_name);
			for(i=0;i<*vars_size;i++){
				//printf("%s\t",vars[i].type);
				//printf("%s\t",vars[i].name);
				if(strcmp(var_name,vars[i].name) == 0){
					printf("%s\n",vars[i].var);
				}
				//printf("%s\n",vars[i].var);
			}
		}
	}else{
		if((statement[0] == '0')||(statement[0] == '1')||(statement[0] == '2')||(statement[0] == '3')||(statement[0] == '4')||(statement[0] == '5')||(statement[0] == '6')||(statement[0] == '7')||(statement[0] == '8')||(statement[0] == '9')){
			//printf("--direct print mode--\n");
			direct_print = 1;
			for(i=0;i<statement_len;i++){
				if(statement[i] != '<'){
					s_port[j] = statement[i];
					j++;
				}else{
					j++;
					break;
				}
			}
			s_port[j] = '\0';
			if(s_port[strlen(s_port)-1] == '&'){
				//printf("---threading print mode---\n");
				i_port = atoi(s_port);
				for(i=j;i<statement_len;i++){
					if(statement[i] != '\0'){
						maple_statement[k] = statement[i];
						k++;
					}else{
						k++;
						break;
					}
				}
				maple_statement[k] = '\0';
				//printf("M:::%s:::\n",maple_statement);
				to_maple_statement(maple_statement,*vars_size,vars,new_maple_statement);
				printf("N:::%s:::\n",new_maple_statement);
				maple_client(LOCALHOST,i_port,new_maple_statement,0,1,0);
				//maple_client(LOCALHOST,i_port,maple_statement,0,1,0);
			}else{
				//printf("---non-threading print mode---\n");
				i_port = atoi(s_port);
				for(i=j;i<statement_len;i++){
					if(statement[i] != '\0'){
						maple_statement[k] = statement[i];
						k++;
					}else{
						k++;
						break;
					}
				}
				maple_statement[k] = '\0';
				//printf("M:::%s:::\n",maple_statement);
				to_maple_statement(maple_statement,*vars_size,vars,new_maple_statement);
				printf("N:::%s:::\n",new_maple_statement);
				maple_client(LOCALHOST,i_port,new_maple_statement,0,1,1);
				//maple_client(LOCALHOST,i_port,maple_statement,0,1,1);
				direct_print = 0;
			}

		}else{
			printf("NON_SUPPORT\n");
			return(-1);
		}
	}
	return(0);
}
