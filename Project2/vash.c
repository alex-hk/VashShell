#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <syscall.h>
#include "mycommands.h"


int parser(struct scall * scal);
void decision(struct scall * scal);
int getInput();
int parsefunc(struct scall * scal);
int parseargs(struct scall * scal);


int main(int argc, char* argv[]){
	char * str;
	int size = 1024;
	struct scall scal;
	while (1){
		if(getInput(str, size-1) == 0){
			printf("Error getting input\n");
			exit(1);
		}
		str[strcspn(str, "\n")] = 0;
		scal.fmsg = str;
	//	printf("Message: %s\n", scal.fmsg);
		parser(&scal);
		decision(&scal);
	}
	return 0;
}

int getInput(char* str, int size){
	char input[size];
	char username[128];
	char cdir[1024];
	if(getlogin_r(username, 128) != 0)
		perror("getlogin_r() error\n");
	if(getcwd(cdir, 1024) == NULL)
		perror("getcwd() error\n");
	printf("%s:%s# ", username, cdir);
	fgets(input, size, stdin);
	if(strncpy(str, input, size) == NULL)
		return 0;
	else return 1;
}

int parser(struct scall * scal){
	parsefunc(scal);
	parseargs(scal);
	if(scal->fmsg != NULL){
		if(strncmp(scal->fmsg, "cat ", 4) == 0 ||
				strncmp(scal->fmsg, "cat\0", 4) == 0)
			scal->func = "cat";
		else if(strncmp(scal->fmsg, "cd ", 3) == 0 ||
				strncmp(scal->fmsg, "cd\0", 3) == 0)
			scal->func = "cd";
		else if(strncmp(scal->fmsg, "cp ", 3) == 0 ||
				strncmp(scal->fmsg, "cp\0", 3) == 0)
			scal->func = "cp";	
		else if(strncmp(scal->fmsg, "ls ", 3) == 0 || strncmp(scal->fmsg, "ls\0", 3) == 0)
			scal->func = "ls";
		else if(strncmp(scal->fmsg, "grep ", 5) == 0 || strncmp(scal->fmsg, "grep\0", 5) == 0)
			scal->func = "grep";
		return 1;
	} else return 1;

	return 0;
}

int parsefunc(struct scall * scal){
	char func[8];
	int i = 0;
	char * str = scal->fmsg;
	if(str != NULL){
		while(str[i] != '\0' && str[i] != ' '){
			func[i] = str[i];
			i++;
		}
	}
	func[i] = '\0';
	//printf("Function for string \" %s \" is %s\n", str, func);
	scal->func = func;
	scal->msgargs = str+i+1;
	//printf("Message arguments: %s\n", scal->msgargs);
	return 0;
}

int parseargs(struct scall * scal){
	int i = 0;
	scal->args = malloc(10 * sizeof *scal->args);
	char * str = strdup(scal->msgargs);
	if(str != NULL){
		char * ptr = str;
		int j = 0;
		char * token;
		while(*ptr && *ptr != '\0'){
			if(*ptr == '"'){
				printf("Parsing argument\n");
				j = 0;
				//ptr++;
				while(*ptr && ( *(ptr++) != '\n' || *ptr != '\0' )){
					//if(*(ptr) == ' ') break;	
					if(*(ptr) != '"') {
						j++;
					} else {
						printf("Sizeof args string: %i\nPtr position: %c\nJ size: %i\n",sizeof scal->args+i, *(ptr-j), j); 	
						strncpy(*(scal->args+i), ptr-j, j);
						//(scal->args+i)[j] = '\0';
						printf("String with quotes: %s\n", *(scal->args+i));
						i++;
					}
					ptr++;
				}
			}
			else if(*ptr == ' '){
				printf("Skipping white space\n");
				while(*ptr == ' ') ptr++;
			}
			else{
				j = 0;
				while(*ptr != ' ' || *ptr != '\0' || *ptr != '\n'){
					j++;
					ptr++;
				}
				strncpy(*(scal->args+i), ptr-j, j);
				printf("String without quotes: %s\n", *(scal->args+i));
				i++;
			}

			//while((token = strsep(&str, " "))){
			//	printf("Token: %s\n", token);
			//	*(scal->args+i) = token;
			//	printf("%s\n", *(scal->args+i));
			//	i++;
			//	scal->argc = i;
			//}
		}
		//ptr++;
	} else {
		return 1;
	}
	scal->argc = i;
	return 0;
}

void decision(struct scall * scal){
	if(strcmp(scal->func,"cat") == 0){		//CAT
		//printf("cat chosen\n");
		_cat(scal);
	}
	else if(strcmp(scal->func, "clear") == 0 || strcmp(scal->func, "clr") == 0){		//CLEAR
		_cd(scal);
	}
	else if(strcmp(scal->func, "mkdir") == 0){		//MKDIR
		_mkdir(scal);
	}
	else if(strcmp(scal->func, "rmdir") == 0){		//RMDIR
		_rmdir(scal);
	}
	else if(strcmp(scal->func, "sleep") == 0){		//SLEEP
		_sleep(scal);
	}
	else if(strcmp(scal->func, "kill") == 0){		//KILL
		_kill(scal);
	}
	else if(strcmp(scal->func, "diff") == 0){		//DIFF
		_diff(scal);
	}

	else if(strcmp(scal->func, "cp") == 0){			//CP
		_cp(scal);
	}
	else if(strcmp(scal->func, "env") == 0){		//ENV
		_env(scal);
	}
	else if(strcmp(scal->func, "timeout") == 0){		//TIMEOUT
		_timeout(scal);
	}
	else if(strcmp(scal->func, "wait") == 0){		//WAIT
		_wait(scal);
	}
	else if(strcmp(scal->func, "cd") == 0){			//CD
		_cd(scal);
	}
	else if(strcmp(scal->func,"ls") == 0){			//LS
		//printf("ls chosen\n");
		_ls(scal);
	}
	else if(strcmp(scal->func, "grep") == 0){		//GREP
		//printf("grep chosen\n");
		_grep(scal);
	}
	else{
		printf("Invalid choice\n");		//DEFAULT
	}
}