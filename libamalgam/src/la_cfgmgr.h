#ifndef LA_CFGMGR_H
#define LA_CFGMGR_H

#   define AMALGAM_H
#   ifndef AMALGAM_H
#       error "include file not included by amalgam.h"
#   endif

#   define LA_BUILDMODE_H
#   ifndef LA_BUILDMODE_H
#       error "la_buildmode.h not included by amalgam.h"
#   endif

#   define LA_DBGLOG_H
#   ifndef LA_DBGLOG_H
#       error "la_buildmode.h not included by amalgam.h"
#   endif
/* The reason for default groups and debug level here, is for the logging
 * of values with respect to reading in config files.
 *
 * On the other hand, this is compiled, so  */

#define LA_CFGMGR_ENABLE_LOGCFGVALS la_cfgmgr_enable_logcfgvals()

#define LA_CFGMGR_DISABLE_LOGCFGVALS la_cfgmgr_disable_logcfgvals()

extern void la_cfgmgr_enable_logcfgvals();
extern void la_cfgmgr_disable_logcfgvals();
extern void config_load( char *fname );
extern void config_err_exit(char *msg);
extern char *config_get(char *name ) ;
extern int config_get_hex( char *name, int dflt);
extern int config_get_num( char *name, int dflt);
extern int config_get_true( char *name, int dflt);
extern void config_done(void);
#endif
