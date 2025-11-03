#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "la_dbglog.h"
unsigned int LOGGING_LEVEL;
unsigned int LOGGING_GROUP ; /* set to EMERG in la_dbgtrace.c */
unsigned int STD_LOGGING_LEVEL = LA_FATAL; 
unsigned int  STD_LOGGING_GROUP = LA_EMERG ;


static  char *dbglogfallbackfn = "LA_DBGLOG.log";
static char *dbglogfn = NULL ;

/* this is the fallback filename compiled into the library. */
FILE *dbgfp = NULL;

bool (*la_dbgdologging)() ;/* not bad really , can be used later in a program */
bool la_dbgdolog(); 
bool la_dbgnologging(); 

char *(*la_dbglog_open_mode)() ; /* Address of the "real" function. */
char *la_dbglog_open_append();
char *la_dbglog_open_write();


void (*la_dbglog_close)() ;
void la_dbglog_close_append() ;
void la_dbglog_close_noappend() ;

void la_dbglog_init (void) __attribute__((constructor (101))); 
void la_dbglog_deinit (void) __attribute__((destructor (101))); 

/* TODO needs setter and gette for append
 * and a good and easy one for do/donotdo logging.
 */
void la_dbglog_init(void) 
{
    LOGGING_LEVEL = LA_FATAL ;
    LOGGING_GROUP = LA_EMERG;

   la_dbgdologging = la_dbgnologging; 
   la_dbglog_open_mode = la_dbglog_open_append ; 
   la_dbglog_close = la_dbglog_close_append;

    dbglogfn = dbglogfallbackfn ;
    return;
}

static bool logfnset = false ;
void la_dbglog_deinit(void) 
{
    if (logfnset) {
        free(dbglogfn);
        dbglogfn = NULL ;
    }
    return;
}
/* if we can't set the logfilename, we halt with 
 * error message immediately.
 */
void la_dbglogfn_set( char *fn)
{
    if ((fn == NULL) || *fn == '\0' )
        goto errnofn;
    char *fntmp ;
    if ( (fntmp = strdup(fn)) == NULL ) {
        goto errstrdup; 
    } else {
        dbglogfn = fntmp ;
        logfnset = true ;
        return;
    }
errstrdup:
    fprintf(stderr, " strdup couldn't return valid copy of const fn.\n");
    exit(EXIT_FAILURE);
errnofn:
    fprintf(stderr, "No filename passed as a parameter, exiting.\n");
    exit(EXIT_FAILURE);
}

char *la_dbglogfn_get()
{
    return dbglogfn;
}

bool la_dbgdolog() 
{
    return true ;
}


bool la_dbgnologging()
{
    return false ;
}

char *la_dbglog_open_append()
{
    return "a" ;
}


char *la_dbglog_open_write()
{
    return "w" ;
}
/* two below assigned to function pointer
 * void (*la_dbglog_close)() in la_dbglog.h
 */
void la_dbglog_close_append()
{
   fclose(dbgfp) ;
   dbgfp = NULL ;
} 
void la_dgblog_close_noappend() { 
    return;
}

/* open log file if one supplied, otherwise obtain from 
 * config data or use the default file name which in turn
 * uses a fallback value if nott set. -- pass NULL or ""
 * to use default value */
void la_dbgopenlog( const char *fname)
{
   if ((dbgfp == NULL)  && (*la_dbgdologging)()) {
       if ((fname == NULL) || fname[0] == '\0' ) {
           fname = dbglogfn ;
           dbgfp = fopen(fname,(*la_dbglog_open_mode)());
       }
       if (dbgfp == NULL ) {
           fprintf(stderr, "la_dgbopenlog  open error: file %s, mode %s\n",
                   fname,(*la_dbglog_open_mode)());
           exit(EXIT_FAILURE);
       }
       //GDB
   }
}

/* return 1 if trace for specified group at specified level is 
 * enabled, otherwise return 0 - open log file if needed.
 * In addition to that we will need to consider the current buildmode.
 * This version is as close to the original as possible, considering 
 * that we now are configured run time with respect to what libamalgam
 * concerns.
 */

bool la_dbglog_atlevel(La_loglvl level, La_loggrp loggroup, char *fname )
{
    bool ret;
    if (la_dbgdologging() == false ) {
        ret = false ;
    } else  {
        if (dbgfp == NULL ) {
            la_dbgopenlog(fname) ;
        }
        ret = (level<=LOGGING_LEVEL && (loggroup & LOGGING_GROUP));   
    }
    return ret;
} 

/* set logging level that must be met or undercut for log entry */
// Static baby, static!
void la_dbglog_setlevel(La_loglvl newlevel)
{
    LOGGING_LEVEL = newlevel ;
}

void la_dbglog_addgroup(La_loggrp newgroup )
{
    LOGGING_GROUP |= newgroup ;
}

void la_dbglog_delgroup(unsigned int oldgroup )
{
    LOGGING_GROUP ^= oldgroup ;
}

void la_dbglog_disable(bool flag)
{
    if (flag) { /* shall disable */
        la_dbgdologging = la_dbgnologging; 
   } else {  /* shall enable */
        la_dbgdologging = la_dbgdolog; 
   }
}

char *la_dbglog_time(void)
{
    time_t tm;
    char *p_tm;
    time(&tm);
    p_tm = ctime(&tm);
    return p_tm ;
}
#ifndef NOMAIN
int main(void)
{
    la_dbgopenlog(NULL);
    printf("that's all!\n") ;
    return 0;
}
#endif
