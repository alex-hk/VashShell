#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "mycommands.h"
#include <dirent.h>

int _cat(struct scall * sc){
	char lbuf[1024]; //Buffer for line
	FILE * f;
       	if((f = fopen(sc->args[0], "r")) == NULL){
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		return 1;
	}

	while(fgets(lbuf, sizeof lbuf, f) != NULL)
		printf("%s",lbuf);
	fclose(f);	
	return 0;
}

int _cd(struct scall * sc){
	if(strcmp(sc->args[0], "~") == 0){
		printf("Home directory: %s\n", getenv("HOME"));
		chdir(getenv("HOME"));
		return 0;
	}
	printf("Home Directory: %s\n", sc->args[0]);
	if(chdir(sc->args[0]) < 0){
		fprintf(stderr, "No such directory: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}

int _cp(struct scall * sc){
	//Instructions
	//cp file.txt file2.txt
	//Open file.txt
	//Open dest
	//Make file in dest
	//copy file.txt into src with same name
	//



	FILE ** f;
	char buf[1024];
	f = malloc(sizeof(FILE) * (sc->argc));
	DIR * dirsrc;
	DIR * dirdest;
	struct dirent *de;
	int i = 0;


	//if(strcmp(sc->args[0], "-r") == 0 ||
	//		strcmp(sc->args[0], "-R") == 0 ||
	//	        strcmp(sc->args[0], "--recursive")){
	//	if((dirsrc = opendir(sc->args[1])) != NULL){
	//		if((dirdest = opendir(sc->args[sc->argc-1])) == NULL){
	//			printf("Directory %s does not exist\n", sc->args[sc->argc])
	//		}
	//		while( i <= sc->argc - 1){
	//			if((f[i] = fopen(sc->args[i], "r")) == NULL){
	//				printf("No such file or directory\n");
	//			} else {
	//				
	//			}
	//		}
	//	} else {
	//		fprintf(stderr, "Directory does not exist: %s\n", 
	//				strerror(errno));
	//	}
	//
	//}
		
	if((f[0] = fopen(sc->args[0], "r")) != NULL){
		if((f[1] = fopen(sc->args[1], "w+")) != NULL){
			while(fgets(buf, 1023, f[0]) != NULL){
				fputs(buf, f[1]);
			}
		}else{
			printf("Something went completely wrong. Could not open or create file.\n");
			return 1;
		}
	} else {
		fprintf(stderr, "%s\n", strerror(errno));
		return 1;
	}
	fclose(f[0]);
	fclose(f[1]);

	return 0;
}

int _ls(struct scall * sc){
	int nfidir = 0;
	DIR *dir;
	struct dirent *de;
	if((dir = opendir(".")) != NULL){
		while((de = readdir(dir)) !=  NULL){
			if(nfidir == 5){
				printf("\n");
				nfidir = 0;
			}
			if((sc->argc != 0 && strcmp(sc->args[0],"-a") == 0) ||
					(strcmp(de->d_name, ".") != 0 &&
					 strcmp(de->d_name, "..") != 0)){
				printf("%s ", de->d_name);
				nfidir++;
			}
		}
		printf("\n");
		closedir(dir);
	} else {
		fprintf(stderr, "Error opening directory: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}

int _grep(struct scall * sc){
	char buf[1024]; //Buffer for found values
	FILE ** f;
	int count = 0;
	char * lookup = malloc(sizeof(sc->args[0]));
	strncpy(lookup, sc->args[0]+1, sizeof(strlen(sc->args[0]-1)));
	printf("String: %s\n", lookup);
	f = malloc(sizeof(FILE) * (sc->argc-1));
	while(count < (sc->argc - 1)){
		if((f[0] = fopen(sc->args[1], "r")) != NULL){
			
		} else {
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
	}
	fclose(f[0]);
	return 0;
}

int _clear(struct scall * sc){
	return 0;
}

int _mkdir(struct scall * sc){
	return 0;
}

int _rmdir(struct scall * sc){
	return 0;
}

int _timeout(struct scall * sc){
	return 0;
}

int _wait(struct scall * sc){
	return 0;
}

int _sleep(struct scall * sc){
	return 0;
}

int _kill(struct scall * sc){
	return 0;
}

int _stat(struct scall * sc){
	return 0;
}

int _diff(struct scall * sc){
	return 0;
}

int _env(struct scall * sc){
	return 0;
}
