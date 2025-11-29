#ifndef LA_LOG_H
#define LA_LOG_H
/**
 * @file
 * @brief Logs values and execution during development and release mode.
 * SPDX-License-Identifier: MIT
 */
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum { LA_FILEONLY = 1, LA_TOSTDERR = 2, LA_DBGTEE = 3 } LA_Behavior;
typedef enum { LA_NODATUM = 1, LA_WITHDATUM } LA_Specifity;


extern void emitlogentry( FILE * restrict stream, const char *fmt, ... );

typedef enum { LA_LOG_EMERG = 0, LA_LOG_ALERT, LA_LOG_CRIT, LA_LOG_ERR,
    LA_LOG_WARNING, LA_LOG_NOTICE, LA_LOG_INFO, LA_LOG_DEBUG
} La_loglvl;

typedef enum { LA_EMERG_GRP= 0, LA_LOGGRP1 = 0x01, LA_LOGGRP2 = 0x02,
    LA_LOGGRP3 = 0x04, LA_LOGGRP4 = 0x08, LA_LOGGRP5 = 0x12,
    LA_LOGGRP6 = 0x20, _LA_LOGGRP7 = 0x40, LA_LOGGRP8 = 0x80,
    LA_LOGGRP9 = 0x100, LA_LOGGRP10 = 0x200, LA_LOGGRP11 = 0x400,
    LA_LOGGRP12 = 0x800, LA_LOGGRP13 = 0x1000, LA_LOGGRP14 = 0x2000,
    LA_LOGGRP15 = 0x4000, LA_LOGGRP16 = 0x8000
} La_loggrp;

/* clang-format off */
/* this part is about configuring the log module for no  logging. */
/* PASS1: dealing with setting the defaults for LAREL, since it is off 
* by default.
 *
 * If LA_LOG_OFF  is defined, then we don't implement logging macros in
 * our code at all, and makes our macro statements vanish.
  */
#ifndef LA_LOG_OFF

/* defining LA_LOG_LA_REL on the compiler command line gives you opportunity
 * to override the wipeout of logging in LAREL (LA_REL) release buildsm,
 * if it should be necessary to log in release builds, maybe  with syslog.
 */

#   if defined (LAREL)
#       if  defined(LA_LOG_REL)
#           define LA_LOG_DATUM 1
            static const LA_Behavior LA_BEHAVIOR = LA_FILEONLY;
#       elif defined(LA_LOG_REL_NODATUM)
#           if defined(LA_LOG_DATUM)
#               undefine LA_LOG_DATUM
#           endif
            static const LA_Behavior LA_BEHAVIOR = LA_FILEONLY;
#       else
#           define LA_LOG_OFF 1
#       endif
#   endif
#endif

/* PASS2: tailoring the macros in accordance to the configuration. 
 * The build mode and any override feature request macros to override default
 * behavior is considered.
 */
#ifndef LA_LOG_OFF

/* getting correct config for mode */
#   if defined(LADEV)
#       if defined(LA_LOG_ONLYTOFILE)
            static const LA_Behavior LA_BEHAVIOR = LA_FILEONLY;
#       else
            static const LA_Behavior LA_BEHAVIOR = LA_TOSTDERR;
#       endif
#   elif defined(LADBG)
#       if defined(LA_LOG_ONLYTOFILE)
            static const LA_Behavior LA_BEHAVIOR = LA_FILEONLY;
#       else
#           if defined(LA_LOG_DATUM)
#               undef LA_LOG_DATUM
#           endif
            static const LA_Behavior LA_BEHAVIOR = LA_DBGTEE;
#       endif
#   endif

/* Getting correct detail level whether we should use datum is probed in the
 * la_log_rt_behavior.
 */
#   if defined(LA_LOG_DATUM)
        static const LA_Specifity LA_SPECIFITY = LA_WITHDATUM;
#   else
        static const LA_Specifity LA_SPECIFITY = LA_NODATUM;
#   endif
#endif /* ifndef LA_LOG_OFF */


/* Macro definitions for inserting  logging  statements in source code: */

/* This macro should really not be used by anybody, except
 * the programmer of LOG macros!  Using this macro only leads to problems.
 */
#define LA_PRIVATE_LOG(log_level, log_group,printlogentry) \
{ if (la_log_atlevel(log_level,log_group,\
   la_dbglog_get_fn())) { printlogentry; la_log_close() ; } }


#define LA_LOG_LOG_FATAL(fmt, ...)\
{ LA_PRIVATE_LOG(LA_LOG_EMERG,LA_EMERG_GRP, \
   emitlogentry(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }

/* The two macros above should always be available, because the LA_LOG_FATAL should 
 * allways work! */
/* The manual log entries for just inserting one log entry */

#ifndef LA_LOG_OFF

#define LA_LOG_LOG(log_level, log_group, fmt, ...)\
{ LA_PRIVATE_LOG(log_level,log_group, \
   emitlogentry(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }

#define LA_LOG_HERE(log_level, log_group)\
{ if (la_log_atlevel(log_level,log_group,\
   la_dbglog_get_fn())) {\
    emitlogentry(dbgfp,"log in file %s at line %d\n",(__FILE__), (__LINE__)); \
    la_log_close() ; } }

#define LA_LOG_TIME(log_level, log_group)\
{    LA_LOG_LOG(log_level, log_group, "%s\n", la_dbglog_time()); }


#define LA_LOG_STR(log_level, log_group, str)\
{ LA_LOG_LOG(log_level, log_group, "%s", str); }

/* The standard macro for adding log entries.  More convenient
 * for inserting debugging log statements for a whole module.
 */
#define LA_LOG_ADDTOLOG(fmt, ...)\
{ LA_PRIVATE_LOG(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(), \
   emitlogentry(dbgfp,fmt __VA_OPT__(,) __VA_ARGS__)); }



#define LA_LOG_ADDHERE \
{ if (la_log_atlevel(la_dbglog_get_stdlevel(),la_dbglog_get_stdgroup(),\
   la_dbglog_get_fn())) {\
    emitlogentry(dbgfp,"log in file %s at line %d\n",(__FILE__), (__LINE__)); \
    la_log_close() ; } }


#define LA_LOG_ADDTIME LA_LOG_ADDTOLOG("%s\n", la_dbglog_time())

#define LA_LOG_ADDSTR(str) LA_LOG_ADDTOLOG( "%s", str)

#define LA_LOG_ENABLE(cur_beh,cur_spc) la_log_setrt_behavior(cur_beh,cur_spc)

#define LA_LOG_DISABLE la_dbglog_disable((true))

#define LA_LOG_REENABLE la_dbglog_disable((false))

#define LA_LOG_ISENABLED la_dbglog_isenabled()

#define LA_LOG_OPEN_WRITE la_dbglog_cfg_open_write()

#define LA_LOG_CLOSE { if (dbgfp != NULL ) { fclose(dbgfp); dbgfp = NULL ; } ; }

#define LA_LOG_SETLEVEL(level) la_dbglog_setlevel((level))

#define LA_LOG_GETLEVEL la_dbglog_getlevel()

#define LA_LOG_ADDGROUP(group) la_dbglog_addgroup((group))

#define LA_LOG_DELGROUP(group) la_dbglog_delgroup((group))

#define LA_LOG_GETGROUP la_dbglog_getgroup()

#define LA_LOG_SETSTD_LEVEL(level) la_dbglog_set_stdlevel((level))

#define LA_LOG_GETSTD_LEVEL la_dbglog_get_stdlevel()

#define LA_LOG_SETSTD_GROUP(group) la_dbglog_set_stdgroup((group))

#define LA_LOG_GETSTD_GROUP la_dbglog_get_stdgroup()

#define LA_LOG_FN_SET(fname) la_dbglogfn_set((fname))

#else
/* maybe I should define the empty macros to ((void)0)
 */

#define LA_LOG_LOG(log_level, log_group, fmt, ...) ((void)0)

#define LA_LOG_HERE ((void)0)

#define LA_LOG_TIME  ((void)0)

#define LA_LOG_STR(str) ((void)0)

#define LA_LOG_ADDTOLOG(fmt, ...) ((void)0)


#define LA_LOG_ADDHERE ((void)0) 

#define LA_LOG_ADDTIME ((void)0)

#define LA_LOG_ADDSTR(str) ((void)0)

#define LA_LOG_ENABLE(cur_beh,cur_spc) ((void)0)

#define LA_LOG_DISABLE ((void)0)

#define LA_LOG_REENABLE

#define LA_LOG_ISENABLED 0

#define LA_LOG_OPEN_WRITE ((void)0)

#define LA_LOG_CLOSE ((void)0)

#define LA_LOG_SETLEVEL(level) ((void)0)

#define LA_LOG_GETLEVEL 0

#define LA_LOG_ADDGROUP(group) ((void)0)

#define LA_LOG_DELGROUP(group) ((void)0)

#define LA_LOG_GETGROUP 0

#define LA_LOG_SETSTD_LEVEL(level) ((void)0)

#define LA_LOG_GETSTD_LEVEL  17

#define LA_LOG_SETSTD_GROUP(group) ((void)0)

#define LA_LOG_GETSTD_GROUP   0x8000

#define LA_LOG_FN_SET(fname)  ((void)0)

#endif

/* clang-format on */

/* fptr that gets the correct handler in the correct "enable" routine. */
extern bool ( *la_log_atlevel )( La_loglvl level, La_loggrp loggroup, char *fname );
/* TODO: use this name in the macros */
extern FILE *dbgfp;

extern void la_dbglog_cfg_open_write( void );
extern void ( *la_log_close )(  );

extern void la_dbglogfn_set( char *fn );
extern char *la_dbglog_get_fn(  );

extern bool la_log_onlytofile_atlevel( La_loglvl level, La_loggrp loggroup, char *fname );

extern void la_dbglog_setlevel( La_loglvl newlevel );
extern La_loglvl la_dbglog_getlevel(  );

extern void la_dbglog_addgroup( La_loggrp newgroup );
extern void la_dbglog_delgroup( La_loggrp oldgroup );
extern La_loggrp la_dbglog_getgroup( void );

extern void la_dbglog_set_stdlevel( La_loglvl stdlevel );
extern La_loglvl la_dbglog_get_stdlevel( void );

extern void la_dbglog_set_stdgroup( La_loggrp stdgroup );
extern La_loggrp la_dbglog_get_stdgroup( void );

extern void la_dbglog_disable( bool flag );
extern bool la_dbglog_isenabled( void );
extern char *la_dbglog_time( void );
extern void la_dbglog_log_fatal( const char *fatal_emsg, ... );

/* This is the function call for the main appliance or initialization of 
 * logging.  it is called by the LA_LOG_ENABLE logging.
 */
extern void la_log_setrt_behavior( LA_Behavior cur_bhv, LA_Specifity cur_spc );

/* precompiled modules that need to find the current states needs to get
 * the current configuration through getters of the run time values set by
 * `la_log_setrt_behavior`.
 */
extern LA_Behavior la_getrt_behavior(  );
extern LA_Specifity la_getrt_specifity(  );
extern bool ( *dologging )(  );
#endif /* ifndef LA_LOG_H */

/* private metholds that are to be used from build time agnostic libraries 
 * that utilizes logging of some kind, like the logging of read configuration values
 * in the cfgmgr module.
 * Those functions are not to be used from clients of the library, only internally by
 * other modules.
 */
/* used by 'parallel' modules in the library to figure out if logging is enabled. */
