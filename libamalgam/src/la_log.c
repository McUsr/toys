/*
MIT License

Copyright (c) 2025 Tommy Bollman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

/**
 * @file
 * @brief Logs values and execution during development and release mode.
 * SPDX-License-Identifier: MIT
 */
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "la_log.h"

static unsigned int LOGGING_LEVEL	  = LA_LOG_EMERG;
static unsigned int LOGGING_GROUP	  = LA_EMERG_GRP;
static unsigned int STD_LOGGING_LEVEL = LA_LOG_EMERG;
static unsigned int STD_LOGGING_GROUP = LA_EMERG_GRP;

static char *logfallbackfn = "LA_LOG.log";
/* this is the fallback filename compiled into the library. */

static char *logfn		   = NULL;

FILE *dbgfp = NULL;

bool (*dologging)(); /* not bad really , can be used later in a program
							 */
static bool dolog();
static bool dontlog();
static bool keeplogopen = false;

static char *(*open_mode)(); /* Address of the "real" function. */
static char *openforappend();
static char *openforwrite();

void (*la_log_close)(); /* this is extern, its called from macros. */
static void closewhenappending();
static void keepopenwhenwriting();
static void noop()
{
	return;
}

void la_dbglog_init(void) __attribute__((constructor(101)));
void la_dbglog_deinit(void) __attribute__((destructor(101)));

static void regular_emit(FILE *restrict stream, const char *fmt, va_list params);

typedef void (*emitentry_fn)(FILE *restrict stream, const char *fmt, va_list params);

static emitentry_fn emitlogmsg;
static bool logging_disabled;

void		la_dbglog_init(void) /* default values */
{
	dologging		 = dontlog; /* no logging     */
	logging_disabled = true;
	open_mode		 = openforappend; /* append to file */
	la_log_close	 = closewhenappending; /* and close it   */

	logfn = logfallbackfn; /* hard coded name */
    emitlogmsg = &regular_emit ;
	return;
}

static bool logfnset = false; /* if have allocated string */
void		la_dbglog_deinit(void)
{
	if (logfnset) {
		free(logfn);
		logfn = NULL;
	}
	return;
}

/* configures the log to keep log file open and not close it
 * after every log call.
 * It is much easier to save the current file if logging is disabled first.
 *
 * So, we think of this as a one way track really. we have no choice to
 * make the stream append -close agen after open_write.
 */
void la_dbglog_cfg_open_write(void)
{
	if (logging_disabled == true) {
		keeplogopen	 = true;
		open_mode	 = openforwrite; /* open for writing  */
		la_log_close = keepopenwhenwriting; /* don't close it    */
	} else {
		fprintf(stderr,
				"Error: la_dbglog_cfg_open_write:\n"
				"Configuring open mode while logging is enabled...exiting!\n");
		exit(EXIT_FAILURE);
	}
}

static void openlogfile(const char *fname);


/* ========================================================================*/
/*              intialize and configure for the different moods.           */
/* ========================================================================*/

/* open log file if one supplied, otherwise obtain from
 * config data or use the default file name which in turn
 * uses a fallback value if nott set. -- pass NULL or ""
 * to use default value */

/* return 1 if trace for specified group at specified level is
 * enabled, otherwise return 0 - open log file if needed.
 * In addition to that we will need to consider the current buildmode.
 * This version is as close to the original as possible, considering
 * that we now are configured run time with respect to what libamalgam
 * concerns.
 */

/* Actuators */
bool (*la_log_atlevel)(La_loglvl level, La_loggrp loggroup, char *fname);
/* set logging level that must be met or undercut for log entry
 * Those are the control values that la_dbglog_atlevel uses
 * to determine whether a log entry should be submitted or not.
 */
/* fptr that gets the correct handler in the correct "enable" routine. */
/* bool (*la_log_atlevel)(La_loglvl level, La_loggrp loggroup, char *fname ); */

static bool la_log_onlyto_file_atlevel(La_loglvl level, La_loggrp loggroup,
									   char *fname)
{
	bool ret;
	if (dologging() == false) {
		ret = false;
	} else {
		ret = (level <= LOGGING_LEVEL && (loggroup & LOGGING_GROUP));
		if (ret && dbgfp == NULL) {
			openlogfile(fname);
		}
	}
	return ret;
}

static bool la_log_onlyto_stderr_atlevel(La_loglvl level, La_loggrp loggroup,
										 char *fname)
{
	bool ret;
	(void) fname; /* UNUSED */
	if (dologging() == false) {
		ret = false;
	} else {
		ret = (level <= LOGGING_LEVEL && (loggroup & LOGGING_GROUP));
	}
	return ret;
}

/* Doesn't use stderr for anything, so it is pretty much like it always were
 * configuring for writing to a file works
 */
/* governors */
static void la_log_enable_tofileonly(void) /* datum is configured at detail level */
{
	dologging		 = dolog;
	logging_disabled = false;
	open_mode		 = openforappend; /* append to file */
	la_log_close	 = closewhenappending; /* and close it   */

	logfn		   = logfallbackfn;
	la_log_atlevel = la_log_onlyto_file_atlevel;
}

static bool redir_stderr = false;
static bool redir_stdout = false ;

#define STDERR_HANDLE 2
#define STDOUT_HANDLE 1
/* we log to stderr if it is redirected */
void la_log_enable_dev_stderr(void)
{
	dologging		 = dolog;
	logging_disabled = false;
	// GDB
	if (!isatty(STDERR_HANDLE)) {
		/* stderr is redirected and we should use it. for logging */
		open_mode	   = NULL; /* append to file */
		la_log_close   = noop;
		logfn		   = NULL;
		la_log_atlevel = la_log_onlyto_stderr_atlevel;
		dbgfp		   = stderr;
		redir_stderr   = true;
        if (!isatty(STDOUT_HANDLE)) 
            redir_stdout = true ;

	} else {
		/* business as usual */
		la_log_enable_tofileonly();
	}
}

static void la_log_enable_dbg_tee_logging(void);

#define CANTHAPPEN(situation)\
{\
    fprintf(stderr,"Cant happen: %s\n",(char *)situation);\
    exit(EXIT_FAILURE);\
}


static void regular_emit(FILE *restrict stream, const char *fmt, va_list params)
{
    vfprintf(stream,fmt,params);
}

static void logtogdbconsoletoo(FILE *restrict stream, const char *format, va_list ap );

static void logtimetoo(FILE *restrict stream, const char *format, va_list ap);

void emitlogentry(FILE *restrict stream, const char *fmt, ... )
{
    va_list ap;
    va_start(ap,fmt);
    emitlogmsg(stream, fmt,ap);
   va_end(ap); 
 
}
static LA_Behavior rt_behavior ;
static LA_Specifity rt_specifity;

/* the settings are vetted in `la_log.h` */
void la_log_setrt_behavior(LA_Behavior cur_bhv, LA_Specifity cur_spc )
{
    rt_behavior = cur_bhv;
    rt_specifity = cur_spc ;

   if (cur_bhv == LA_FILEONLY ) {
      la_log_enable_tofileonly(); 
      emitlogmsg = &regular_emit ;
   } else if (cur_bhv == LA_TOSTDERR ) {
       la_log_enable_dev_stderr();
       emitlogmsg = &regular_emit ;
   } else if (cur_bhv == LA_DBGTEE ) {
      la_log_enable_dbg_tee_logging();
      emitlogmsg = &logtogdbconsoletoo;
   } else 
       CANTHAPPEN("la_log_rt_behavior: unknown behavior enum!");

   /* specifity = cur_spc; */
    if (cur_bhv != LA_DBGTEE ) {
      if (cur_spc == LA_NODATUM )
         emitlogmsg = &regular_emit ;
      else if (cur_spc == LA_WITHDATUM ) {
         fprintf(stderr,"DEBUG WE'RE LOGGING TIME TOO\n");
         fflush(stderr);;
         emitlogmsg = &logtimetoo; 
      } else 
           CANTHAPPEN("la_log_rt_behavior: unknown specificity enum!");
    }
}

/* getters for returning runtime values in modules
 * that implements logging.
 */
LA_Behavior la_getrt_behavior()
{
    return rt_behavior;
}

LA_Specifity la_getrt_specifity()
{
    return rt_specifity;
}

/* logs a message to the current log in release mode,
 * so that the user can reproduce it when filing a bug.
 * Made for use by the `la_assert` and `la_failfast` modules
 * (by proxy). I think we should add the "datum" (date and time)
 * in front of this line.
 */
void la_dbglog_log_fatal(const char *fatal_emsg,...)
{


	va_list ap;
	char	timebuf[30];
	time_t	tm;

	va_start(ap, fatal_emsg);
	tm = time(NULL);
	strftime(timebuf, sizeof(timebuf), "%Y-%b-%d %X", localtime(&tm));
	if (dbgfp == NULL) {
		openlogfile(logfn);

	fprintf(dbgfp, "%s: ", timebuf);
	vfprintf(dbgfp, fatal_emsg, ap);
	va_end(ap);
	la_log_close();
	}
}

/* if we can't set the logfilename, we halt with
 * error message immediately.
 */
void la_dbglogfn_set(char *fn)
{
	if ((fn == NULL) || *fn == '\0')
		goto errnofn;
	char *fntmp;
	if ((fntmp = strdup(fn)) == NULL) {
		goto errstrdup;
	} else {
		if (logfnset) {
			/* already alloced space for a file name */
			fprintf(stderr, "dgblogfn != NULL:  %p\n", logfn);
			free(logfn);
		}
		logfn	 = fntmp;
		logfnset = true;
		return;
	}
errstrdup:
	fprintf(stderr, " strdup couldn't return valid copy of const fn.\n");
	exit(EXIT_FAILURE);
errnofn:
	fprintf(stderr, "No filename passed as a parameter, exiting.\n");
	exit(EXIT_FAILURE);
}
/* main purpose is to open the logfile from the error strategies.
 * I'm frankly not  sure if I need it if it is internal.
 *
 * the other thing that isn't encapsulated is the file handle.
 * This should be more protrected?
 */
char *la_dbglog_get_fn()
{
	return logfn;
}

bool dolog()
{
	return true;
}

bool dontlog()
{
	return false;
}

static char *openforappend()
{
	return "a";
}

static char *openforwrite()
{
	return "w";
}
/* two below assigned to function pointer
 * void (*la_dbglog_close)() in la_dbglog.h
 */
static void closewhenappending()
{
	keeplogopen = false;
    if (dbgfp != NULL ) {
        fclose(dbgfp);
        dbgfp = NULL;
    } /* TODO: considerations concerning syslog */
}

static void keepopenwhenwriting()
{
	return;
}

void la_dbglog_setlevel(La_loglvl newlevel)
{
	LOGGING_LEVEL = newlevel;
}

La_loglvl la_dbglog_getlevel(void)
{
	return LOGGING_LEVEL;
}

void la_dbglog_addgroup(La_loggrp newgroup)
{
	LOGGING_GROUP |= newgroup;
}

void la_dbglog_delgroup(La_loggrp oldgroup)
{
	LOGGING_GROUP ^= oldgroup;
}

La_loggrp la_dbglog_getgroup(void)
{
	return LOGGING_GROUP;
}

/* Unlike the setters above, those below sets the
 * default values for the "shorthand" macros that emit
 * log entries (emitters):
 *      LA_LOG_ADDTOLOG
 *      LA_LOG_STR
 *      LA_LOG_HERE
 *      LA_LOG_TIME.
 * The must be used in order to get consistent behavior.
 */
void la_dbglog_set_stdlevel(La_loglvl stdlevel)
{
	STD_LOGGING_LEVEL = stdlevel;
}

La_loglvl la_dbglog_get_stdlevel(void)
{
	return STD_LOGGING_LEVEL;
}

void la_dbglog_set_stdgroup(La_loggrp stdgroup)
{
	STD_LOGGING_GROUP = stdgroup;
}

La_loggrp la_dbglog_get_stdgroup(void)
{
	return STD_LOGGING_GROUP;
}

void la_dbglog_disable(bool flag)
{
	if (flag) { /* shall disable */
		dologging		 = dontlog;
		logging_disabled = true;
	} else { /* shall enable */
		dologging		 = dolog;
		logging_disabled = false;
	}
}

bool la_dbglog_isenabled(void)
{
	return (logging_disabled == false);
}

static char la_dbglog_timebuf[30];

char *la_dbglog_time(void)
{
	time_t tm;
	tm = time(NULL);
	strftime(la_dbglog_timebuf, sizeof(la_dbglog_timebuf), "%Y-%b-%d %X\n",
			 localtime(&tm));
	return la_dbglog_timebuf;
}


static void logtimetoo(FILE *restrict stream, const char *format,va_list ap)
{
	char	timebuf[30];
	time_t	tm;

	tm = time(NULL);
	strftime(timebuf, sizeof(timebuf), "%Y-%b-%d %X", localtime(&tm));
	fprintf(stream, "%s: ", timebuf);
	vfprintf(stream, format, ap);
}

static bool run_from_gdb(void)
{
	char  path[48];
	bool  ret  = false;
	pid_t ppid = getppid();
	snprintf(path, 48, "/proc/%d/comm", (int) ppid);

	FILE *f = fopen(path, "r");
	if (f != NULL) {
		char p_name[256];
		if (fgets(p_name, sizeof(p_name), f) != NULL) {
			// Remove the trailing newline character, if present
			size_t len = strlen(p_name);
			if (len > 0 && p_name[len - 1] == '\n') {
				p_name[len - 1] = '\0';
			}
			if (!strcmp("gdb", p_name))
				ret = true;
		}
		fclose(f);
	} else {
		perror("Failed to open /proc/[PPID]/comm");
		/* you will probably need to rework the paths.
		 * This works on Debian Linux. You're on your own
		 * if you're using something else.
		 */
		fprintf(
			stderr,
			"Probably compiled this on something other than Debian/Linux\n");
		fprintf(stderr,"You neeed to sort out the access of the proc filesystem\n");
        fprintf(stderr,"in order to get the name of the parent process.\n"); 
        exit(EXIT_FAILURE);
	}

	return ret;
}

#define DIE(fmt, ...)                                    \
	{                                                    \
		fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__); \
		exit(EXIT_FAILURE);                              \
	}

/*
 This is the initial part of LADBG part that configures 
 the logging to show output in GDB when logging or otherwise
 show the log statements directly to the /dev/tty  If possible
 and don't tee output anywhere when  it's not. 
 The secondpart is the function `la_dbglog_logtogdbconsoletoo`
 below which is used by logging macros in `la_dbglog.h`
*/
static bool runfromgdb = false;
void la_log_enable_dbg_tee_logging(void)
{

	la_log_enable_dev_stderr();
	if (run_from_gdb()) {
		/* set up the teeing stuff */
		runfromgdb = true;
	} else {
        runfromgdb = false ;
        /* if we aren't run from gdb, it works like in the LADEV mode,
         * excpet that log output is teed to the tty if possible and 
         * like in LADEV, that no output is sent to the log file if 
         * stderr is redirected. The  log-datum can't be generated inr
         * LADBG mode. */
    }
}

/* 
 Provenance
 ---------- 
 This is part two of the LADBG mode that actually enforces the behavior
 described above the function la_dbglog_logtogdbconsoletoo:.

 la_dbglog_logtogdbconsoletoo enforces the teeing of output to tty,
 whereas the current tty is in gdb or the console window.

 The function  is called after the correct "atlevel" routine according
 to current stream redirection,has signaled that the the current log
 statement should be printed.

 Behavior
 --------
 When in LADBG mode and we're not running from gdb, then we tee to stderr,
 so that we get it on screen, if the stderr isn't already redirected
 and stderr isn't already redirected.  (When stderr is redirected the
 output is redirected from the file to stderr, and then if stdout isn't
 redirected we send it to stdout, if stdout is redirected, however then
 we don't generate any output of logging statments on the tty.

*/
/* void la_dbglog_logtogdbconsoletoo(FILE *restrict stream, const char *format, ...) { */

static void logtogdbconsoletoo(FILE *restrict stream, const char *format, va_list ap ) {
#define BUFLEN 254

	char	linebuf[BUFLEN + 1];
	vsnprintf(linebuf, BUFLEN, format, ap);
    /* Todo: this should go to syslog if syslog is defined. */
	fprintf(stream,"%s", linebuf);  /* This is the part that goes to stderr. */
    
	if (runfromgdb == true  ) { 
        printf("\t   %s",linebuf);
	} else if ( redir_stderr == false || redir_stdout == false) {
        printf("%s",linebuf);
    }
}

static void openlogfile(const char *fname)
{
	if ((dbgfp == NULL) && (*dologging)()) {
		if ((fname == NULL) || fname[0] == '\0')
			fname = logfn;
		dbgfp = fopen(fname, (*open_mode)());
		if (keeplogopen && (dbgfp != NULL))
			fcntl(fileno(dbgfp), F_SETFD, FD_CLOEXEC);
		else if (dbgfp == NULL) {
			DIE("openlogfile  open error: file %s, mode %s\n",
					fname, (*open_mode)());
		}
	}
}
