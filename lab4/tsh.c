/*
 * Shell Lab
 * Nathan Owen, ID:  930860382, ncowen
 */

/* With glibc, in order to call getpgid, define these: */
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h> // basic 
#include <stdlib.h> // standard library
#include <unistd.h> // 
#include <string.h>  // string functions
#include <ctype.h>  //
#include <signal.h>  //
#include <sys/types.h>  //
#include <sys/wait.h>  //
#include <errno.h>  //

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

/* Signal handlers you will implement */
void sigchld_handler(int sig);

/**
 * These signal handlers for the shell
 * must also be implemented, but they
 * just forward the signal to the current
 * foreground job (i.e., to all processes 
 * in the process group of the foreground job),
 * provided there is a foreground job. If
 * there is no foreground job, these signal
 * handlers should do nothing.
 */
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);  // breaks up cmdline into argument strings, give it cmdline and an array of pointers to char
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/* New functions */
int jobsInBackgroundOrStopped(void);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
        break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
        break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
        break;
    default:
            usage();
    }
    }

    /* Install the signal handlers */
    
    /* This is the signal handler you will need to implement for the shell */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* The signal handlers for SIGINT and SIGTSTP should just return */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    

    /* This one provides a clean way to kill the shell. Do not change this! */
    Signal(SIGQUIT, sigquit_handler); 

    /* Your shell will ignore these:*/
    Signal(SIGTTOU, SIG_IGN);
    Signal(SIGTTIN, SIG_IGN);



    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

    /* Read command line */
    if (emit_prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
        app_error("fgets error");
    }
    if (feof(stdin)) { /* End of file (ctrl-d) */
        fflush(stdout);
        exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID. Otherwise,
 * all children of the shell including background children would be
 * in the same process group and would receive a SIGINT (SIGTSTP) signal
 * when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char *cmdline)
{
    int bg, bi, n; // Holds return values
    char *argv[MAXARGS]; // Holds cmdline arguments 
    pid_t pid; // Holds Process Id
    sigset_t mask; // Holds Signal Mask for Blocking Signals
    
    /* call parseline and send commandline and pointer to argv array */
    bg = parseline(cmdline, argv);
            
    if(argv[0] == NULL)
    {   /* ignore empty lines */
        return; 
    }
    
    /* initialize empty signal set */
    if(sigemptyset(&mask)) unix_error("eval: sigemptyset error");
    /* add SIGCHLD signal to signal set */
    if(sigaddset(&mask, SIGTSTP)) unix_error("eval: sigaddset error");
    
    if((bi = builtin_cmd(argv)) == 0)
    {   /* block SIGCHLD  signal */
        if(sigprocmask(SIG_BLOCK, &mask, NULL)) unix_error("eval: sigprocmask error");
    
        /* create child process with fork command */
        if((pid = fork()) < 0) unix_error("eval: fork error");
            
        if(pid == 0)
        {   /* child process sets new group pid and executes */
            if(setpgrp() == -1) unix_error("eval: setpgrp error");
            
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        
            if(execvp(argv[0], argv) < 0)
            {   /* if execvp fails print message and exit */
                printf("%s: Command not found \n", argv[0]);
                exit(0);
            }
        }   
        
        /* Parent Process adds job to jobs list*/
        if((n = addjob(jobs, pid, (bg == 1 ? BG : FG), cmdline)) == 0) unix_error("eval: addjob error");
            
        /* then unblocks SIGCHLD */
        sigprocmask(SIG_UNBLOCK, &mask, NULL);  
            
        if(!bg)
        {   /* calls waitfg if foreground process */
            waitfg(pid);
        }
        else 
        {   /* background job prints job */
            printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
        }
    }
    /* bi == 1 */
    return;
}
  
/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
    buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
    buf++;
    delim = strchr(buf, '\'');
    }
    else {
    delim = strchr(buf, ' ');
    }

    while (delim) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces */
           buf++;

    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    }
    else {
        delim = strchr(buf, ' ');
    }
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
    return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) { 
    argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */

 int builtin_cmd(char **argv)
{   
    if(!strcmp(argv[0], "quit"))
    {   
        if(jobsInBackgroundOrStopped() == 1)
        {   
            printf("There are stopped jobs.");
            return 1;
        }
        else
        {   
        exit(0);
        }
    }
    else if(!strcmp(argv[0], "jobs"))
    {  
        listjobs(jobs);
        return 1;
    }
    else if(!strcmp(argv[0], "fg") || !strcmp(argv[0], "bg"))
    {  
        do_bgfg(argv);
        return 1;
    }
    else
    {   /* not a builtin_cmd so return 0 */
        return 0;
    }
}
    

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
    pid_t pid; 
    int returnVal;
    int jid; 
    struct job_t *job; 
    
    if(argv[1] == NULL)
    {   /* argument is missing */
        printf("%s command requires PID or %%jobid argument.\n", argv[0]);
        return;
    }
    else
    {   /* check PID */
        if((returnVal = sscanf(argv[1], "%d", &pid)) == 1)
        {   
            if(!(job = getjobpid(jobs, pid)))
            {   /* no process exists */
                printf("(%d): No such process.\n", pid); 
                return;
            }
            else
            {   
                if(!strcmp(argv[0],"bg"))
                {   
		    /*Check if killed */
                    if(kill(-(job->pid), SIGCONT) < 0) {
                        unix_error("do_bgfg: Kill error");
                    }
                    job->state = BG;
                    printf("[%d] (%d) %s", (job->jid), (job->pid), (job->cmdline));
                }
                else if(!strcmp(argv[0], "fg"))
                {   
		    /* Check if killed */
                    if(kill(-(job->pid), SIGCONT) < 0) {
                        unix_error("do_bgfg: Kill error");
                    }
                    job->state = FG;
                    waitfg(job->pid);
                }
            }
        }
        else if(returnVal == 0)
        {   
            if((returnVal = sscanf(argv[1], "%%%d", &jid)) == 0)
            {   
		/* Check that argument is valid */
                printf("%s: argument must be a PID or %%jobid.\n", argv[0]);
                return;
            }
            else
            {   
                if(!(job = getjobjid(jobs, jid)))
                { 
		    /* Check that argument exists */
                    printf("%s : No such job.\n", argv[1]); 
                    return;
                }
                else
                {   
                    if(!strcmp(argv[0],"bg"))
                    {   

			/* Check if killed */
                        if(kill(-(job->pid), SIGCONT) < 0) {
                            unix_error("do_bgfg: Kill error");
                        } 
                        job->state = BG; 
                        printf("[%d] (%d) %s", (job->jid), (job->pid), (job->cmdline));
                    }
                    else if(!strcmp(argv[0], "fg"))
                    {   
			/* Check if killed */
                        if(kill(-(job->pid), SIGCONT) < 0) {
                            unix_error("do_bgfg: Kill error");
                        }
                        job->state = FG;
                        waitfg(job->pid);
                    }
                }
            }
        }
        else
        {   /* error print message and exit */  
            printf("do_bgfg error");
            exit(0);
        }           
    }
    
    return;
}


/* 
 * waitfg - Block until process pid is no longer the foreground process 
 */
void waitfg(pid_t pid)
{
    struct job_t *job; // holds job
    
    /* Check if the foreground object is done */
    if(!(job = getjobpid(jobs, pid)))
    { 
        return;
    }
    
    /*  Continue while state is in foreground and of the same process */
    while(job->state == FG && job->pid == pid)
    { 
        sleep(2);
    }
    
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    int status;
    int jid;
    pid_t pid;
    struct job_t *job;
    
    
    while((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0)
    {  
        if(WIFSTOPPED(status))
        {  
            if(!(job = getjobpid(jobs, pid)))
            {  
                printf("(%d): No such process.\n", pid);
                return;
            }
            /* change state to stopped and print message */
            job->state = ST;
            jid = pid2jid(pid);
            printf("Job [%d] (%d) stopped by signal %d\n", jid, pid, WSTOPSIG(status));
        }
        else if(WIFSIGNALED(status))
        {   /* if signaled delete job and print message */
            jid = pid2jid(pid);
            deletejob(jobs, pid);
            printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status));
        }
        else if(WIFEXITED(status))
        {   /* if exited just delete job */
            jid = pid2jid(pid);
            deletejob(jobs, pid);
        }
        else
        {   /* unix error, print message */
        unix_error("waitpid error");
        }
    }
    
    return;
}


/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
    pid_t pid;
    
    if((pid = fgpid(jobs)) > 0)
    {
        if(kill(-pid, SIGINT) < 0) unix_error("sigint_handler: Kill Error.");
    }
    else
    {   /* sent sigint to shell, print message about shell */
        printf("shell's sigint_handler\n");
        printf("shell pid = %d, fg_pid1 = %d, fg_pid2 = %d\n", getpid(), getpgrp(), getppid());
        printf("sending SIGTSTP to process group %d\n", getpgrp());
    }
    
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig)
{
    pid_t pid;
    
    if((pid = fgpid(jobs)) > 0)
    {
        if(kill(-pid, SIGTSTP) < 0) unix_error("sigstp_handler: Kill Error.");
    }
    else
    {   /* sent sigtstp to shell, print message about shell */
        printf("shell's sigstp_handler\n");
        printf("shell pid = %d, fg_pid1 = %d, fg_pid2 = %d\n", getpid(), getpgrp(), getppid());
        printf("sending SIGSTP to process group %d\n", getpgrp());
    }
    
  return;
}


/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
    clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid > max)
        max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
    return 0;

    for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == 0) {
        jobs[i].pid = pid;
        jobs[i].state = state;
        jobs[i].jid = nextjid++;
        if (nextjid > MAXJOBS)
        nextjid = 1;
        strcpy(jobs[i].cmdline, cmdline);
        if(verbose){
            printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
    }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
    return 0;

    for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
        clearjob(&jobs[i]);
        nextjid = maxjid(jobs)+1;
        return 1;
    }
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].state == FG)
        return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
    return NULL;
    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid)
        return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
    return NULL;
    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid == jid)
        return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
    return 0;
    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid != 0) {
        printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
        switch (jobs[i].state) {
        case BG: 
            printf("Running ");
            break;
        case FG: 
            printf("Foreground ");
            break;
        case ST: 
            printf("Stopped ");
            break;
        default:
            printf("listjobs: Internal error: job[%d].state=%d ", 
               i, jobs[i].state);
        }
        printf("%s", jobs[i].cmdline);
    }
    }
}
/******************************
 * end job list helper routines
 ******************************/

 /***********************
 * Helper routines
 ***********************/
 
 /* jobsInBackgroundOrStopped function for checking background and stopped jobs */
 int jobsInBackgroundOrStopped(void)
 {
    int i;
    
    for(i = 0; i < MAXJOBS; i++){
        if(jobs[i].state == BG)
            return 1;
        else if(jobs[i].state == ST)
            return 1;
    }
    return 0; 
 }
 
/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}
