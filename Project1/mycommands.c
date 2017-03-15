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
	return 0;
}

int _ls(struct scall * sc){
	DIR *dir;
	struct dirent *de;
	if((dir = opendir(".")) != NULL){
		while((de = readdir(dir)) !=  NULL){
			if(strcmp(sc->args[0],"-a") == 0 || (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0))
				printf("%s ", de->d_name);
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
	FILE * f;
	return 0;
}
