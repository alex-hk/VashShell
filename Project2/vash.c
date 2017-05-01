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
	int size = 1024;
	char str[size];
	struct scall * scal;
	while (1){
		scal = malloc(sizeof *scal);
		if(getInput(str, size-1) == 0){
			printf("Error getting input\n");
			exit(1);
		}
		str[strcspn(str, "\n")] = 0;
		scal->fmsg = str;
	//	printf("Message: %s\n", scal.fmsg);
		parser(scal);
		decision(scal);
		free(scal);
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
		else if(strncmp(scal->fmsg, "ls ", 3) == 0 ||
				strncmp(scal->fmsg, "ls\0", 3) == 0)
			scal->func = "ls";
		else if(strncmp(scal->fmsg, "grep ", 5) == 0 ||
				strncmp(scal->fmsg, "grep\0", 5) == 0)
			scal->func = "grep";
		else if(strncmp(scal->fmsg, "wait ", 5) == 0 ||
				strncmp(scal->fmsg, "wait\0", 5) == 0)
			scal->func = "wait";
		else if(strncmp(scal->fmsg, "timeout ", 8) == 0 ||
				strncmp(scal->fmsg, "timeout\0", 8) == 0)
			scal->func = "timeout";
		else if(strncmp(scal->fmsg, "kill ", 5) == 0 ||
				strncmp(scal->fmsg, "kill\0", 5) == 0)
			scal->func = "kill";
		else if(strncmp(scal->fmsg, "mkdir ", 6) == 0 ||
				strncmp(scal->fmsg, "mkdir\0", 6) == 0)
			scal->func = "mkdir";
		else if(strncmp(scal->fmsg, "rmdir ", 6) == 0 ||
				strncmp(scal->fmsg, "rmdir\0", 6) == 0)
			scal->func = "rmdir";
		else if(strncmp(scal->fmsg, "stat ", 5) == 0 ||
				strncmp(scal->fmsg, "stat\0", 5) == 0)
			scal->func = "stat";
		else if(strncmp(scal->fmsg, "sleep ", 6) == 0 ||
				strncmp(scal->fmsg, "sleep\0", 6) == 0)
			scal->func = "sleep";
		else if(strncmp(scal->fmsg, "diff ", 5) == 0 ||
				strncmp(scal->fmsg, "diff\0", 5) == 0)
			scal->func = "diff";
		else if(strncmp(scal->fmsg, "env ", 4) == 0 ||
				strncmp(scal->fmsg, "env\0", 4) == 0)
			scal->func = "env";
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
	scal->args = malloc(8 * sizeof *scal->args);
	for(int k = 0; k < 8; ++k){
		*(scal->args+k) = (char *)malloc(256);
	}
	char * str = (char *) malloc(sizeof scal->msgargs * 1024);
	//printf("%s\n", scal->msgargs);
	strcpy(str, scal->msgargs);
	if(str != NULL){
		int isQuotes = 0;
		int isArgs = 1;
		char * ptr = str;
		int j = 0;
		char * qptr;
		char * token;
		while(isArgs){
			if(*ptr == '\0')
				isArgs = 0;
			else if(*ptr == ' '){
				//printf("Skipping white space\n");
				while(*ptr == ' ') ptr++;
			}
			else if(*ptr == '\"' || *ptr == '\''){
				//printf("Quote string start\n");
				ptr++;
				j = 0;
				while(*ptr != '\0' && *ptr != '\n' && (*ptr != '"' && *ptr != '\'')){
					ptr++;
					j++;
				}
				strncpy(*(scal->args+i), ptr-j, j);
				(*(scal->args+i))[j] = '\0';
				//printf("String with quotes: %s\n", *(scal->args+i));
				i++;
				ptr++;
			}
			else{
				j = 0;
				while(*ptr != '\0' && *ptr != '\n' && *ptr != ' ' && *ptr != '\'' && *ptr != '\"'){
					j++;
					ptr++;
				}
				strncpy(*(scal->args+i), ptr-j, j);
				(*(scal->args+i))[j] = '\0';
				//printf("String without quotes: %s\n", *(scal->args+i));
				i++;
				ptr++;
			}
//			while(true)
//				printf("Parsing argument\n");
//				j = 0;
//				//ptr++;
//				if(*ptr == '"'){
//					ptr++;
//					isQuotes = false;
//				}
//				while(*ptr && ( *(ptr++) != '\n' || *ptr != '\0' )){
//					//if(*(ptr) == ' ') break;	
//					if(*(ptr) != '"') {
//						j++;
//					} else {
//						printf("Sizeof args string: %i\nPtr position: %c\nJ size: %i\n",sizeof scal->args+i, *(ptr-j), j); 	
//						strncpy(*(scal->args+i), ptr-j, j);
//						//(scal->args+i)[j] = '\0';
//						printf("String with quotes: %s\n", *(scal->args+i));
//						i++;
//					}
//					ptr++;
//				}
//			}
//			

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
	free(str);
	return 0;
}

void decision(struct scall * scal){
	pid_t pid;
	int status;

	if(strcmp(scal->func,"cat") == 0){		//CAT
		//printf("cat chosen\n");
		_cat(scal);
	}
	else if(strcmp(scal->func, "clear") == 0 || strcmp(scal->func, "clr") == 0){		//CLEAR
		_clear(scal);
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
	else if(strcmp(scal->func, "stat") == 0){		//WAIT
		_stat(scal);
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
