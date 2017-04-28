/*
 * Header file for command calls
 */

/*
 * Scall struct:
 *
 * struct scall {
 * 	char * func; //function name (cd, cat, ls, grep)
 * 	char * argv[]; //Arguments for functions
 * };
 */

struct scall {
	char *	fmsg;
	char *	msgargs;
	char *	func;
	char **	args;
	int 	argc;
	char ** opts;
	int	optc;
};

//Function header for cd
int _cd(struct scall * sc);

//Function header for cp
int _cp(struct scall * sc);

//Function header for ls
int _ls(struct scall * sc);

//Function header for grep
int _grep(struct scall * sc);

//Function header for cat 
int _cat(struct scall * sc);

//Function header for clear
int _clear(struct scall * sc);

//Function header for mkdir
int _mkdir(struct scall * sc);

//Function header for rmdir
int _rmdir(struct scall * sc);

//Function header for timeout
int _timeout(struct scall * sc);

//Function header for wait
int _wait(struct scall * sc);

//Function header for sleep
int _sleep(struct scall * sc);

//Function header for kill
int _kill(struct scall * sc);

//Function header for stat
int _stat(struct scall * sc);

//Function header for diff
int _diff(struct scall * sc);

//Function header for env
int _env(struct scall * sc);
