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


#ifdef LAREL
    #define LA_NODBGTRACE 1
#endif 
/* We wipe out logging macros 
 * in release builds.
 */

typedef enum {LA_FATAL=0,LA_LOGLVL1,LA_LOGLVL2,LA_LOGLVL3,
    LA_LOGLVL4, LA_LOGLVL5,LA_LOGLVL6,LA_LOGLVL17} La_loglvl ;

typedef enum {LA_EMERG=0,LA_LOGGRP1=0x01,LA_LOGGRP2=0x02,
             LA_LOGGRP3=0x04, LA_LOGGRP4=0x08,LA_LOGGRP5=0x12,
             LA_LOGGRP6=0x20,_LA_LOGGRP7=0x40,LA_LOGGRP8=0x80,
            LA_LOGGRP9=0x100,LA_LOGGRP10=0x200, LA_LOGGRP11=0x400,
             LA_LOGGRP12=0x800, LA_LOGGRP13=0x1000,LA_LOGGRP14=0x2000,
             LA_LOGGRP15=0x4000, LA_LOGGRP16=0x8000 } La_loggrp;

extern unsigned int LOGGING_LEVEL ; /* set to FATAL in la_dbgtrace.c */
extern unsigned int LOGGING_GROUP ; /* set to EMERG in la_dbgtrace.c */


extern unsigned int  STD_LOGGING_LEVEL; /* set to lowest level with highest value */
extern unsigned int STD_LOGGING_GROUP; /* we need to |= together the values. */



/* TODO: I need to set the filename later 
 * search for those functions, I know I have programmed
 * them earlier.
 */

extern FILE *dbgfp ;

extern void (*la_dbglog_close)() ;

extern void la_dbglogfn_set( char *fn);
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


/* DBGTR p.p 259 "C FOR FUN AND PROFIT" */
#define LA_DBGLOG_LOG( trace_level, trace_group, la_fprintf_call) \
{ if (la_dbglog_atlevel(trace_level,trace_group,\
   la_dbglogfn_get())) { la_fprintf_call; la_dbglog_close() ; } }

/* DBGTR1 p.p 259 "C FOR FUN AND PROFIT" this covers them all,
 * as nowadays we do have __VA_OPT__ and __VA_ARGS__  */
#define LA_DBGLOG_ADDTOLOG(fmt, ...)\
{ LA_DBGLOG_LOG(STD_LOGGING_LEVEL,STD_LOGGING_GROUP, \
   fprintf(dbgfp,fmt __VA_OPT__(,) (__VA_ARGS__))); }

#define LA_DBGLOG_HERE LA_DBGLOG_ADDTOLOG( "log in file %s at line %d\n",\
    __FILE__, __LINE__)

#define LA_DBGLOG_TIME LA_DBGLOG_ADDTOLOG("%S", la_dbglog_time())

#define LA_DBLOG_DISABLE( flag) la_dbglog_disable((flag))

#endif
