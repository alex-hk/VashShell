#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <syscall.h>

int parser(char* input);
void decision(int choice, char* str);
int getInput();

int main(int argc, char* argv[]){
	char* str;
	int size = 1024;
	int choice;
	while (1){
		if(getInput(str, size-1) == 0){
			printf("Error getting input\n");
			exit(1);
		}
		printf("Message: %s\n", str);
		choice = parser(str);
		decision(choice, str);
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
	gets(input);
	if(strncpy(str, input, size) == NULL)
		return 0;
	else return 1;

}

int parser(char* input){
	int choice = 0;
	if(input != NULL){
		if(strncmp(input, "cat ", 4) == 0 || strncmp(input, "cat\0", 4) == 0)
			choice = 1;
		if(strncmp(input, "cd ", 3) == 0 || strncmp(input, "cd\0", 3) == 0)
			choice = 2;
		if(strncmp(input, "ls ", 3) == 0 || strncmp(input, "ls\0", 3) == 0)
			choice = 3;
		if(strncmp(input, "grep ", 5) == 0 || strncmp(input, "grep\0", 5) == 0)
			choice = 4;
	}
	return choice;
}

void decision(int choice, char* str){
	int rc;
	char args[1024];
	switch(choice){
		case 1: printf("cat chosen\n"); //cat
		case 2: printf("cd chosen\n");
			strcpy(args, str+3);
			printf("Args for cd: %s\n", args);
			if(strcmp(args, "~") == 0)
				strcpy(args,getenv("HOME"));
			if(chdir(args) == -1)
				fprintf(stderr, "chdir failed, errno = %d\n", errno);
			break;//cd
		case 3: printf("ls chosen\n");   break; //ls
		case 4: printf("grep chosen\n"); break; //grep
		case 0: printf("Invalid choice\n"); break; //invalid choice
	}
}
