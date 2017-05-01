#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "mycommands.h"
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

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
	//printf("Home Directory: %s\n", sc->args[0]);
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
	char * buf = (char*)malloc(1024 * sizeof(char)); //Buffer for lines
	size_t max_line = 1024;
	char * ptr;
	size_t strsz = strlen(*(sc->args));
	FILE ** f;
	int count = 1;
	size_t lsize;
	int ptrcnt = 0;
	int strcnt = 0;
	f = malloc(sizeof(FILE) * ((sc->argc)-1));
	while(count < (sc->argc)){
		if((f[count-1] = fopen(sc->args[count], "r")) != NULL){
			while((lsize = getline(&buf, &max_line, f[0])) != -1){
				if(strstr(buf, (*(sc->args))) != NULL){
					printf("%s: %s", *(sc->args+count), buf);
				}
			}
		} else {
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		fclose(f[count-1]);
		count++;
	}
	free(f);
	return 0;

				
				
				
//				ptr = buf;
//				ptrcnt = 0;
//				while(ptr && lsize > strsz){
//					if((ptrcnt < (lsize - strsz))){
//						if(*ptr == (*(sc->args)[0])){
//							if(strncmp((ptr), *(sc->args), strsz) == 0){
//								printf("%s: %s", *(sc->args+count), buf);
//								break;
//							} else { ptrcnt+=strsz; ptr+=strsz; }
//						} else { ptr++; ptrcnt++; }
//					} else break;
//				}
//
//			}
//		} else {
//			fprintf(stderr, "%s\n", strerror(errno));
//			return 1;
//		}
//		fclose(f[count-1]);
//		count++;
//	}
//	free(f);
//	return 0;
}

int _clear(struct scall * sc){
	return 0;
}

int _mkdir(struct scall * sc){
	struct stat st = {0};
	mode_t mode;
	int i = 0;
	while(i < sc->argc){
		//if(*(sc->args+2) != NULL)
		//	mode = strtol(*sc->args+2, NULL, 8);
		
		if(stat(*(sc->args+i), &st) == -1){
			//if((sc->args+1) != NULL && strcmp(*(sc->args+1), "-m") == 0){
			//	mkdir(*(sc->args+0), mode);
			//} else {
			//	mkdir(*(sc->args+0), 0700);
			//}
			
			mkdir(*(sc->args+i), 0700);
			i++;
			//return 0;
		} else {
			printf("Directory already exists\n");
			i++;
			//return 1;
		}
	}
	return 0;
}

int _rmdir(struct scall * sc){
	struct dirent *d;
	DIR * dir;
	int i = 0,j = 0;

	while(i < sc->argc){
		j = 0;
	//	printf("Directory to open: %s\n", *(sc->args+i));
		dir = opendir(*(sc->args+i));
		if(!dir){
			printf("Not a directory or doesn't exist\n");
			closedir(dir);
			i++;
			continue;
		}
		while((d = readdir(dir)) != NULL){
			if(++j > 2) break;
		}
		if(j <= 2)
			rmdir(*(sc->args+i));
		i++;
	}
	return 0;
}

int _timeout(struct scall * sc){
	pid_t pid;
	int status;

	pid = fork();
	if(pid == -1){
		perror("Failed to create child");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		printf("Executing: %s\n", *(sc->args+1));
		execv(*(sc->args+1), sc->args+2);
	}
	else {
		sleep(atoi(*(sc->args)));

		if(waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED) == -1){
			perror("wait error");
			return 1;
		}

		if(WIFEXITED(status)){
			int exits = WEXITSTATUS(status);
			printf("Child exit was %d\n", exits);
		}
	}
	return 0;
}

int _wait(struct scall * sc){
	int status;
	pid_t pid;

	pid = fork();
	if(pid == -1){
		perror("Failed to create child");
		exit(EXIT_FAILURE);	
	} else if(pid == 0){
		return 1;
	}
	if(sc->argc == 1){
		if(waitpid(atoi(*(sc->args)), &status, NULL) == -1){
			perror("Waitpid error");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

int _sleep(struct scall * sc){
	if(sc->argc == 1) sleep(atoi(*(sc->args)));
	else return 1;
	
	return 0;
}

int _kill(struct scall * sc){
	int sig = 15;
	int i = 0;
	if(sc->argc >= 1){
		if(strncmp(sc->args[0], "-9", 2) == 0 || strncmp(sc->args[0], "-SIGKILL", 8) == 0){
			sig = 9;
			i++;
		}
		if(strncmp(sc->args[0], "-1", 2) == 0 || strncmp(sc->args[0], "-SIGHUP", 7) == 0){
			sig = 1;
			i++;
		}
		if(strncmp(sc->args[0], "-15", 3) == 0 || strncmp(sc->args[0], "-SIGTERM", 8) == 0){
			sig = 15;
			i++;
		}
		
		while(i < sc->argc){
			kill(atoi(*(sc->args+i)), sig);
			i++;
		}
	}
	return 0;
}

int _stat(struct scall * sc){
	struct stat *st = malloc(sizeof(struct stat));
	stat(*(sc->args), st);
	int stchmod = st->st_mode&0777;
	struct passwd *passuid = getpwuid(st->st_uid);
	struct group *grpgid = getgrgid(st->st_gid);
	
	printf("  File: \'%s\'\n", *(sc->args));
	printf("  Size: %d\t\tBlocks: %d\tIO Block: %d\t",st->st_size,st->st_blocks, st->st_blksize);
	
	if(S_ISDIR(st->st_mode)) printf("directory\n");
	if(S_ISCHR(st->st_mode)) printf("charcter special file\n");
	if(S_ISBLK(st->st_mode)) printf("block special file\n");
	if(S_ISREG(st->st_mode)) printf("regular file\n");
	if(S_ISFIFO(st->st_mode)) printf("FIFO special file\n");
	if(S_ISLNK(st->st_mode)) printf("symbolic link\n");
	if(S_ISSOCK(st->st_mode)) printf("socket\n");
	
	printf("Device: %xh/%dd\tInode: %d\tLinks: %d\n",st->st_dev, st->st_dev, st->st_ino, st->st_nlink);
	printf("Access: (%4o/", stchmod);
	
	if(S_ISDIR(st->st_mode)) printf("d");
	else if(S_ISREG(st->st_mode)) printf("-");
	printf( (st->st_mode & S_IRUSR) ? "r" : "-");
	printf( (st->st_mode & S_IWUSR) ? "w" : "-");
	printf( (st->st_mode & S_IXUSR) ? "x" : "-");
	printf( (st->st_mode & S_IRGRP) ? "r" : "-");
	printf( (st->st_mode & S_IWGRP) ? "w" : "-");
	printf( (st->st_mode & S_IXGRP) ? "x" : "-");
	printf( (st->st_mode & S_IROTH) ? "r" : "-");
	printf( (st->st_mode & S_IWOTH) ? "w" : "-");
	printf( (st->st_mode & S_IXOTH) ? "x" : "-");
	
	printf(")\tUid: (%5d/%8s)\tGid: (%5d/%10s)\n", st->st_uid, passuid->pw_name, st->st_gid, grpgid->gr_name);

	char bufatime[40];
	char bufmtime[40];
	char bufctime[40];
	struct tm *ats, *mts, *cts;
	ats = localtime(&st->st_atime);
	mts = localtime(&st->st_mtime);
	cts = localtime(&st->st_ctime);

	strftime(bufatime, 40, "%F %T %z", ats);
	strftime(bufmtime, 40, "%F %T %z", mts);
	strftime(bufctime, 40, "%F %T %z", cts);




	printf("Access: %s\n", bufatime);
	printf("Modify: %s\n", bufmtime);
	printf("Change: %s\n", bufctime);
	printf(" Birth: %s\n", "-");
	free(st);
	return 0;
}



int _diff(struct scall * sc){
	FILE * fone;
	FILE * ftwo;
	size_t maxlines = 20;

	int bools[maxlines];
	int linenum;
	int count = 0;

	char lines[maxlines][1024];
	char lines2[maxlines][1024];

	char * bufone = (char*)malloc(1024 * sizeof(char));
	char * buftwo = (char*)malloc(1024 * sizeof(char));

	size_t max_line = 1024;

	//int flagsone[maxlines];
	//int flagstwo[maxlines];

	if((fone = fopen(*(sc->args), "r")) == NULL){
		perror("File does not exist");
		return -1;
	}
	if((ftwo = fopen(*(sc->args+1), "r")) == NULL){
		perror("File does not exist");
		return -1;
	}

	int lcount = 0;

	while(1){
		while(count < maxlines && getline(&bufone, &max_line, fone) != -1 && getline(&buftwo, &max_line, ftwo) != -1){
			if(strcmp(bufone, buftwo) != 0){
				bools[count] = 1;
				if(bufone != NULL){
					strcpy(lines[lcount], bufone);
				}
				if(buftwo != NULL){
					strcpy(lines2[lcount], buftwo);
				}
				lcount++;
			} else {
				bools[count] = 0;
			}
			count++;
		}
		break;
	}

	int i = 0;
	int n = 0;
	int start = 0;
	int j = 0;
	int lc1 = 0;
	int lc2 = 0;

	while(i < count){
		start = i+1;
		j = 0;
		n = 0;
		while(bools[i] == 1){
			n++;
			i++;
		}
		
		if(n > 0){
			if(n > 1) printf("%d,%dc%d,%d\n", start, start+n-1, start, start+n-1);
			else if(n == 1) printf("%dc%d\n", start, start);
			while(j < n){
				printf("< %s", lines[lc1]);
				lc1++;
				j++;
			}
			printf("---\n");
			j = 0;
			while(j < n){
				printf("> %s", lines2[lc2]);
				lc2++;
				j++;
			}
		}
		i++;
	}



	return 0;
}

int _env(struct scall * sc){
	extern char **environ;
	char * str = *environ;
	int i = 1;
	while(str){
		printf("%s\n", str);
		i++;
		str = *(environ+i);
	}
	return 0;
}
