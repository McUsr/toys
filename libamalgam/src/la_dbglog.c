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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "la_dbglog.h"

static unsigned int LOGGING_LEVEL = LA_FATAL;
static unsigned int LOGGING_GROUP = LA_EMERG; 
static unsigned int STD_LOGGING_LEVEL = LA_FATAL; 
static unsigned int STD_LOGGING_GROUP = LA_EMERG ;


static  char *dbglogfallbackfn = "LA_DBGLOG.log";
static char *dbglogfn = NULL ;

/* this is the fallback filename compiled into the library. */
FILE *dbgfp = NULL;

bool (*la_dbgdologging)() ;/* not bad really , can be used later in a program */
bool la_dbgdolog(); 
bool la_dbgnologging(); 

char *(*la_dbglog_open_mode)() ; /* Address of the "real" function. */
static char *la_dbglog_open_append();
static char *la_dbglog_open_write();


void (*la_dbglog_close)() ;
void la_dbglog_close_append() ;
void la_dbglog_close_noappend() ;

void la_dbglog_init (void) __attribute__((constructor (101))); 
void la_dbglog_deinit (void) __attribute__((destructor (101))); 

static bool dbglog_disabled;
void la_dbglog_init(void)  /* default values */
{

   la_dbgdologging = la_dbgnologging;            /* no logging     */
   dbglog_disabled = true ;
   la_dbglog_open_mode = la_dbglog_open_append ; /* append to file */
   la_dbglog_close = la_dbglog_close_append;     /* and close it   */

    dbglogfn = dbglogfallbackfn ;                /* hardcoded name */
    return;
}


static bool logfnset = false ; /* if have allocated string */
void la_dbglog_deinit(void) 
{
    if (logfnset) {
        free(dbglogfn);
        dbglogfn = NULL ;
    }
    return;
}
/* configures the log to keep log file open and not close it
 * after every log call.
 * It is much easier to save the current file if logging is disabled first.
 */
void la_dbglog_cfg_open_write(void)
{
    if (dbglog_disabled == true ) {
        la_dbglog_open_mode = la_dbglog_open_write ;    /* open for writing  */
        la_dbglog_close = la_dbglog_close_noappend;     /* don't close it    */
    } else {
        fprintf(stderr, "Error: la_dbglog_cfg_open_write:\n"
               "Configuring open mode while logging is enabled...exiting!\n");
        exit(EXIT_FAILURE);
    }
    
}

static void la_dbgopenlog( const char *fname)
{
   if ((dbgfp == NULL)  && (*la_dbgdologging)()) {
       if ((fname == NULL) || fname[0] == '\0' ) 
           fname = dbglogfn ;
       dbgfp = fopen(fname,(*la_dbglog_open_mode)());
       if (dbgfp == NULL ) {
           fprintf(stderr, "la_dgbopenlog  open error: file %s, mode %s\n",
                   fname,(*la_dbglog_open_mode)());
           exit(EXIT_FAILURE);
       }
   }
}

/* logs a message to the current log in release mode,
 * so that the user can reproduce it when filing a bug.
 * Made for use by the `la_assert` and `la_failfast` modules
 * (by proxy).
 */
void la_dbglog_log_fatal(const char * fatal_emsg )
{
    if (dbgfp == NULL ){
        la_dbgopenlog(dbglogfn) ;
        fprintf(dbgfp,"%s\n",fatal_emsg); 
        la_dbglog_close();
    }
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
        if (logfnset )  {
            /* already alloced space for a file name */ 
            fprintf(stderr, "dgblogfn != NULL:  %p\n",dbglogfn) ;
            free(dbglogfn);
        }
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
/* main purpose is to open the logfile from the error strategies.
 * I'm frankly not  sure if I need it if it is internal.
 *
 * the other thing that isn't encapsulated is the file handle.
 * This should be more protrected?
 */
char *la_dbglog_get_fn()
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
    //GDB
   fclose(dbgfp) ;
   dbgfp = NULL ;
} 

void la_dbglog_close_noappend() { 
    return;
}

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

/* set logging level that must be met or undercut for log entry
 * Those are the control values that la_dbglog_atlevel uses
 * to determine whether a log entry should be submitted or not.
 */
void la_dbglog_setlevel(La_loglvl newlevel)
{
    LOGGING_LEVEL = newlevel ;
}

La_loglvl la_dbglog_getlevel(void)
{
    return LOGGING_LEVEL ;
}

void la_dbglog_addgroup(La_loggrp newgroup )
{
    LOGGING_GROUP |= newgroup ;
}

void la_dbglog_delgroup(La_loggrp oldgroup )
{
    LOGGING_GROUP ^= oldgroup ;
}

La_loggrp la_dbglog_getgroup(void) 
{
    return LOGGING_GROUP  ;
}



/* Unlike the setters above, those below sets the
 * default values for the "shorthand" macros that emit 
 * log entries (emitters).
 * LA_DBGLOG_ADDTOLOG,LA_DBGLOG_STR, LA_DBGLOG_HERE,LA_DBGLOG_TIME.
 */
void la_dbglog_set_stdlevel(La_loglvl stdlevel)
{
    STD_LOGGING_LEVEL = stdlevel ;
}

La_loglvl la_dbglog_get_stdlevel(void)
{
    return STD_LOGGING_LEVEL  ;
}

void la_dbglog_set_stdgroup(La_loggrp stdgroup )
{
    STD_LOGGING_GROUP = stdgroup ;
}

La_loggrp la_dbglog_get_stdgroup(void )
{
    return STD_LOGGING_GROUP  ;
}

void la_dbglog_disable(bool flag)
{
    if (flag) { /* shall disable */
        la_dbgdologging = la_dbgnologging; 
        dbglog_disabled = true ;
   } else {  /* shall enable */
        la_dbgdologging = la_dbgdolog; 
        dbglog_disabled = false;
   }
}

bool la_dbglog_isenabled(void)
{
    return (dbglog_disabled == false );
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
