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

//Function header for 
int _cat(struct scall * sc);
