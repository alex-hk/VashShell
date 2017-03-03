#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int parser(char** input);
void decision(int choice);
char** getInput();

int main(int argc, char* argv[]){
	char* str[1024];
	int choice;
	while (1){
		str = getInput();
		choice = parser(str);
		decision(choice);
	}
}

char** getInput(){
	char* input[1024];
	char* username[128];
	char* cdir[1024];
	if(getlogin_r(username, 128) == NULL)
		perror("getlogin_r() error");
	if(getcwd(cdir, 1024) == NULL)
		perror("getcwd() error");
	printf("%s:%s# ", username, cdir);
	gets(input);

	return input;
}

int parser(char** input){
	return 0;
}

void decision(int choice){
	switch(choice){
		case 1: printf("cat chosen\n");  break; //cat
		case 2: printf("cd chosen\n");   break; //cd
		case 3: printf("ls chosen\n");   break; //ls
		case 4: printf("grep chosen\n"); break; //grep
		case 0: printf("Invalid choice\n"); break; //invalid choice
	}
}
