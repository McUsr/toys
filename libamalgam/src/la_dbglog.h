#ifndef LA_DBGLOG_H
#define LA_DBGLOG_H
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
#include <stdbool.h>

/* defining LA_DODGBLOG on the compiler command line gives you opportunity
 * to override the * wipeout of logging in LAREL (LA_REL) release buildsm,
 * if it should be necessary to log in release builds.
 */
#ifdef LAREL
#   ifndef LA_DODGBLOG
/*  TODO: rename to: LA_LOG_REL_DOLOG */
#       define LA_NODBGLOG 1
        /* Wipe out logging macros in release builds.  */
#   else
#       define LA_DBGLOG_DATUM 1
#   endif
#endif 


#ifndef LA_NODBGLOG 

/* DBGTR p.p 259 "C FOR FUN AND PROFIT" */
#       define LA_DBGLOG_LOG(trace_level,trace_group,la_fprintf_call) \
{ if (la_dbglog_atlevel(trace_level,trace_group,\
   la_dbglog_get_fn())) { la_fprintf_call; la_dbglog_close() ; } }

#ifndef LA_DBGLOG_DATUM

/* DBGTR1 p.p 259 "C FOR FUN AND PROFIT" this covers them all,
 * as nowadays we do have __VA_OPT__ and __VA_ARGS__  */
#       define LA_DBGLOG_ADDTOLOG(fmt, ...)\
{ LA_DBGLOG_LOG(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(), \
   fprintf(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }

#       define LA_DBGLOG_HERE \
{ if (la_dbglog_atlevel(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(),\
   la_dbglog_get_fn())) {\
    fprintf(dbgfp,"log in file %s at line %d\n",(__FILE__), (__LINE__)); \
    la_dbglog_close() ; } }

#       define LA_DBGLOG_TIME LA_DBGLOG_ADDTOLOG("%s", la_dbglog_time())

#   elif defined(LA_DBGLOG_DATUM) 


#       define LA_DBGLOG_ADDTOLOG(fmt, ...)\
{ LA_DBGLOG_LOG(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(), \
   la_dbglog_logtimetoo(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }

#       define LA_DBGLOG_HERE \
{ if (la_dbglog_atlevel(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(),\
   la_dbglog_get_fn())) {\
    la_dbglog_logtimetoo(dbgfp,"log in file %s at line %d\n",(__FILE__), (__LINE__)); \
    la_dbglog_close() ; } }

#       define LA_DBGLOG_TIME LA_DBGLOG_ADDTOLOG("%s", "")

#   endif

#define LA_DBGLOG_STR(str) LA_DBGLOG_ADDTOLOG( "%s", str)



#define LA_DBGLOG_DISABLE la_dbglog_disable((true))

#define LA_DBGLOG_ENABLE la_dbglog_disable((false))

#define LA_DBGLOG_ISENABLED la_dbglog_isenabled()

#define LA_DBGLOG_OPEN_WRITE la_dbglog_cfg_open_write()

#define LA_DBGLOG_CLOSE { if (dbgfp != NULL ) { fclose(dbgfp); dbgfp = NULL ; } ; }

#define LA_DBGLOG_SETLEVEL(level) la_dbglog_setlevel((level)) 

#define LA_DBGLOG_GETLEVEL la_dbglog_getlevel() 

#define LA_DBGLOG_ADDGROUP(group) la_dbglog_addgroup((group)) 

#define LA_DBGLOG_DELGROUP(group) la_dbglog_delgroup((group)) 

#define LA_DBGLOG_GETGROUP la_dbglog_getgroup() 

#define LA_DBGLOG_SETSTD_LEVEL(level) la_dbglog_set_stdlevel((level))

#define LA_DBGLOG_GETSTD_LEVEL la_dbglog_get_stdlevel()

#define LA_DBGLOG_SETSTD_GROUP(group) la_dbglog_set_stdgroup((group))

#define LA_DBGLOG_GETSTD_GROUP la_dbglog_get_stdgroup()

#define LA_DBGLOG_FN_SET(fname) la_dbglogfn_set((fname))

#else
/* maybe I should define the empty macros to ((void)0)
 */
#define LA_DBGLOG_LOG(trace_level,trace_group,la_fprintf_call) ((void)0)

#define LA_DBGLOG_ADDTOLOG(fmt, ...) ((void)0)

#define LA_DBGLOG_STR(str) ((void)0)

#define LA_DBGLOG_HERE ((void)0)

#define LA_DBGLOG_TIME  ((void)0)

#define LA_DBGLOG_DISABLE ((void)0)

#define LA_DBGLOG_ENABLE ((void)0)

#define LA_DBGLOG_ISENABLED ((void)0)

#define LA_DBGLOG_OPEN_WRITE ((void)0)

#define LA_DBGLOG_CLOSE ((void)0)

#define LA_DBGLOG_SETLEVEL(level) ((void)0)

#define LA_DBGLOG_GETLEVEL ((void)0)

#define LA_DBGLOG_ADDGROUP(group) ((void)0)

#define LA_DBGLOG_DELGROUP(group) ((void)0)

#define LA_DBGLOG_SETSTD_LEVEL(level) ((void)0) 

#define LA_DBGLOG_GETSTD_LEVEL ((void)0) 

#define LA_DBGLOG_SETSTD_GROUP(group) ((void)0)

#define LA_DBGLOG_GETSTD_GROUP ((void)0)

#define LA_DBGLOG_GETSTD_GROUP     ((void)0)

#define LA_DBGLOG_FN_SET(fname)  ((void)0)

#endif

typedef enum {LA_FATAL=0,LA_LOGLVL1,LA_LOGLVL2,LA_LOGLVL3,
    LA_LOGLVL4, LA_LOGLVL5,LA_LOGLVL6,LA_LOGLVL17} La_loglvl ;

typedef enum {LA_EMERG=0,LA_LOGGRP1=0x01,LA_LOGGRP2=0x02,
             LA_LOGGRP3=0x04, LA_LOGGRP4=0x08,LA_LOGGRP5=0x12,
             LA_LOGGRP6=0x20,_LA_LOGGRP7=0x40,LA_LOGGRP8=0x80,
            LA_LOGGRP9=0x100,LA_LOGGRP10=0x200, LA_LOGGRP11=0x400,
             LA_LOGGRP12=0x800, LA_LOGGRP13=0x1000,LA_LOGGRP14=0x2000,
             LA_LOGGRP15=0x4000, LA_LOGGRP16=0x8000 } La_loggrp;

extern FILE *dbgfp ;

extern void la_dbglog_cfg_open_write(void);
extern void (*la_log_close)() ;

extern void la_dbglogfn_set( char *fn);
extern char *la_dbglog_get_fn();

extern bool la_log_onlytofile_atlevel(La_loglvl level, La_loggrp loggroup,
        char *fname );

extern void la_dbglog_setlevel(La_loglvl newlevel);
extern La_loglvl la_dbglog_getlevel();

extern void la_dbglog_addgroup(La_loggrp newgroup );
extern void la_dbglog_delgroup(La_loggrp oldgroup );
extern La_loggrp la_dbglog_getgroup(void);

extern void la_dbglog_set_stdlevel(La_loglvl stdlevel);
extern La_loglvl la_dbglog_get_stdlevel(void);

extern void la_dbglog_set_stdgroup(La_loggrp stdgroup );
extern La_loggrp la_dbglog_get_stdgroup(void );

extern void la_dbglog_disable(bool flag);
extern bool la_dbglog_isenabled(void);
extern char *la_dbglog_time(void);
extern void la_dbglog_log_fatal(const char * fatal_emsg );
extern void la_dbglog_logtimetoo(FILE *restrict stream, const char *format, ...);
#endif
