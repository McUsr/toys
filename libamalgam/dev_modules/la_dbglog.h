#ifndef LA_DBGLOG_H
#define LA_DBGLOG_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <amalgam.h>
// ifndef amalgam. ...
#   define AMALGAM_H
#   ifndef AMALGAM_H
#       error "include file not included by amalgam.h"
#   endif

#   define LA_BUILDMODE_H
#   ifndef LA_BUILDMODE_H
#       error "la_buildmode.h not included by amalgam.h"
#   endif

extern bool la_dbgdolog(); 
extern bool la_dbgnologging(); 

#ifdef LAREL
    #define LA_NODBGTRACE 1
#endif 
/* We wipe out logging macros 
 * in release builds.
 */

#   ifndef LA_NODBGLOGGING
#       ifndef LA_DBGLOG
#           define LA_DBGLOG
#       endif

bool (*la_dbgdologging)() = la_dbgdolog; /* not bad really , can be used later in a program */

#else 
#       ifndef LA_DBGLOG
#           define LA_DBGLOG
#       endif

bool (*la_dbgdologging)() = la_dbgnologging; 

#endif 

typedef enum {LA_FATAL=0,LA_LOGLVL1,LA_LOGLVL2,LA_LOGLVL3,
    LA_LOGLVL4, LA_LOGLVL5,LA_LOGLVL6,LA_LOGLVL17} La_loglvl ;

typedef enum {LA_EMERG=0,LA_LOGGRP1=0x01,LOGGRP2=0x02,
             LOGGRP3=0x04, LOGGRP4=0x08,LOGGRP5=0x10,
             LOGGRP6=0x20,LOGGRP7=0x40, LOGGRP8=0x80,
             LOGGRP9=0x100,LOGGRP10=0x200,LOGGRP11=0x400,
             LOGGRP12=0x800,LOGGRP13=0x1000,LOGGRP14=0x2000,
             LOGGRP15=0x4000, LOGGRP16=0x8000 } La_loggrp;

extern unsigned int LOGGING_LEVEL ; /* set to FATAL in la_dbgtrace.c */
extern unsigned int LOGGING_GROUP ; /* set to EMERG in la_dbgtrace.c */


extern unsigned int  STD_LOGGING_LEVEL; /* set to lowest level with highest value */
extern unsigned int STD_LOGGING_GROUP; /* we need to |= together the values. */

/* this is default for me, but should be able
 * to turn off by config manager.
 */
#   ifndef LA_DBGLOG_JUST_CLOSE
#       define LA_DBGLOG_CLOSE_APPEND 1
#endif



extern char *la_dbglog_open_append();
extern char *la_dbglog_open_write();

extern void la_dbglog_close_append() ;

extern void la_dbglog_close_noappend() ;
#ifdef LA_DBGLOG_CLOSE_APPEND 

/* if this doesnt't work as anticipated, then I might either add to the function pointer
 *  specificatin (*fptr)()) to avoid statical linking to  the function address compile
 *  time of the library. maybe that ensures that the compiler will look up the address
 *  at that spot run time. */

    char *(*la_dbglog_open_mode)() = la_dbglog_open_append ; /* Address of the "real" function. */
    void (*la_dbglog_close)() = la_dbglog_close_append;

#else

    char *(*la_dbglog_open_mode)() = la_dbglog_open_write ; /* Address of the "real" function. */
    void (*la_dbglog_close)() = la_dbglog_close_noappend;
#endif

/* there is a   pattern here, just like in build_mode to get this to workii
 * I think I will utilize the fact that the libraries are giving one instance
 * of itself to each and every program that uses it.
 *
 * I am not sure if this works though, because this is a function we call
 * from within the library. And the library is alread compiled.
 * Maybe it is more apt to use a function pointer in
 * this case.
 * I mean that is what I'll do if this isn't working.
 */



/* TODO: I need to set the filename later 
 * search for those functions, I know I have programmed
 * them earlier.
 */

extern FILE *dbgfp ;
extern bool la_dbglogfn_set( char *fn) ;
extern char *la_dbglogfn_get();
extern void la_dbgopenlog( const char *fname);
extern bool la_dbglog_atlevel(La_loglvl level, La_loggrp loggroup,
        char *fname );
bool la_dbglog_atlevel(La_loglvl level, La_loggrp loggroup, char *fname );
void la_dbglog_setlevel(La_loglvl newlevel);
void la_dbglog_addgroup(La_loggrp newgroup );
void la_dbglog_delgroup(unsigned int oldgroup );
void la_dbglog_disable(bool flag);
char *la_dbglog_time(void);

/* another construct that won't work as it was */


/* MACRO MATERIAL:
#include <stdio.h>
#define eprintf(format, ...) \
  fprintf (stderr, format __VA_OPT__(,) __VA_ARGS__)
int main(void)
{
    eprintf("Hello world\n");
    return 0;
}
*/
/* DBGTR p.p 259 "C FOR FUN AND PROFIT" */
#define LA_DBGLOG_LOG( trace_level, trace_group, la_fprintf_call) \
{ if (la_dbglog_atlevel(trace_level,trace_group,\
   la_dbglogfn_get())) { la_fprintf_call; la_dbglog_close() ; } }

/* DBGTR1 p.p 259 "C FOR FUN AND PROFIT" this covers them all,
 * as nowadays we do have __VA_OPT__ and __VA_ARGS__  */
#define LA_DBGLOG_ADDTOLOG(fmt, ...)\
{ LA_DBGLOG_LOG(STD_LOGGING_LEVEL,STD_LOGGING_GROUP,\
   fprintf(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }

#define LA_DBGLOG_HERE LA_DBGLOG_ADDTOLOG( "log in file %s at line %d\n",\
    __FILE__, __LINE__)

#define LA_DBGLOG_TIME LA_DBGLOG_ADDTOLOG("%S", la_dbglog_time())

#define LA_DBLOG_DISABLE( flag) la_dbglog_disable((flag))

#endif
