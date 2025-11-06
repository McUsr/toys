/* la_cfmgr.c: config data load and get
 *
 * config data format:
 * # comment
 * name=value string
 * name2=value string
 * #comment
 * name3=value string
 *
 * Any punctuation character at the start of the line will make that
 * line be ignored (as a comment). Be sure not to put spaces around the
 * '='. Everything after the '=' up to the '\n' will be the value, INCLUDING
 * BLANCS. Names must begin with any alphanumeric character and may contain
 * spaces and non alphanumeric characters (after the 
 * first letter). Leading spaces causes a line to be a comment.
 *
 * config_load() can be called more than once to load config data from
 * various configuration files - for example one file could hold
 * specific library related parameters, while another could be needed for 
 * an application. No config variable may be defined twice,
 * either in different files or within one file - this is checked for and 
 * is a FATAL error.
 * config entry "la_dbglog=y" is special - watched for here as indication 
 * to log trace.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdmacros.h>

#include <amalgam.h>
#include "la_cfgmgr.h"

typedef struct la_cfg_lst Cfgvallst;
struct la_cfg_lst {
    Cfgvallst *next;
    char *name;
    char *value;
} *cfgdata = NULL;


/* Kerrisk p.p 872 and Gemini s and https://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
 */
void init( void ) __attribute__( ( constructor( 101 ) ) );
/* setting of initial default values */
void init( void )
{
    LA_DBGLOG_SETSTD_GROUP( LA_LOGGRP1 );
    LA_DBGLOG_SETSTD_LEVEL( LA_LOGLVL1);
    return;
}

static void cfg_show_log_metadata()
{
#ifndef LA_NODBGLOG
    int curlevel = LA_DBGLOG_GETLEVEL;
    int curgroups = LA_DBGLOG_GETGROUP;
    fprintf(stderr, "dbglog: curlevel == %d, curgroups == %d\n",curlevel,curgroups);
    
    La_loggrp cur_std_grp = LA_DBGLOG_GETSTD_GROUP;
    La_loglvl cur_std_lvl = LA_DBGLOG_GETSTD_LEVEL;
    fprintf(stderr, "dbglog: stdlevel == %d, stdgroups== %d\n",cur_std_lvl,cur_std_grp);

    char *curfn = la_dbglog_get_fn(  );
    fprintf( stderr, "cur log file name is: %s\n", curfn );
#endif
}
static bool logcfgvals=true ;

void la_cfgmgr_enable_logcfgvals()
{
    logcfgvals=true ;   
}

void la_cfgmgr_disable_logcfgvals()
{
    logcfgvals=false ;   
    
}

void config_load( char *fname )
{
    FILE *fp = NULL; 
    char linebuf[512]; 
    Cfgvallst *newcfg = NULL; /* config data link. */
    char *pdup;
    static int trace_init = false; /* probably have an actuator for this one. */

    fp = fopen( fname, "r" );
    if ( fp == NULL ) {
        LA_DBGLOG_ADDTOLOG( "Config file open failed: %s\n", fname );
        return ;
    }
   /* loop for each line of input */
    while ( fgets( linebuf, sizeof( linebuf ), fp ) != NULL ) {
       /* if it is a line with config data */
        if ( isalnum( linebuf[0] ) && strchr( linebuf, '=' ) && strchr( linebuf, '\n' ) ) {
           /* allocate memory for config_data linked list link */
            newcfg = malloc( sizeof( Cfgvallst ) );
           /* allocate memory for config data value */
            pdup = strdup( linebuf );
            if ( newcfg == NULL || pdup == NULL )
                config_err_exit( "No config memory" );
            newcfg->name = pdup;
           /* NOTE!! blanks around '=' in file be treated as the ending of
              then nam and beginning of the value, which will probably create
              problems - so don't put them there. 

            25-11-06: Fixed! trailing newlines too! */
            pdup = strchr( pdup, '=' );
            char *trim = pdup;
            while ( *( --trim ) == ' ' )
                *trim = '\0';
            *pdup++ = '\0'; /* set to '\0', increment pointer */
            while ( *pdup == ' ' )
                pdup++;
            /* trimming away the newline. */
            trim = pdup;
            trim = strchr( trim, '\n');
            *trim = '\0' ; 
            newcfg->value = pdup; /* wthout trailing newline! */
           /* check for duplicate definition */
            pdup = config_get( newcfg->name );
           /* error if value is configured twice */
            if ( pdup ) {
                fprintf( stderr, "\nConfig error in file %s"
                         "name %s defined twice, first as:\n%s\n" "again as:\n%s\n", fname, pdup, newcfg->value, pdup );
                config_err_exit( "Duplicate definition\n" );
            }
           /* push new config entry on top of stack */
            newcfg->next = cfgdata;
            cfgdata = newcfg;
        }
    }
    fclose( fp );
    /* enable trace if specified in config data */
    if ( logcfgvals && !trace_init && config_get_true( "dbgtrace", false ) ) {
        /* so we don't re-enable! */
        trace_init = true;
        if (LA_DBGLOG_ISENABLED == false) 
            LA_DBGLOG_ENABLE;
    }
    /* BEHAVIOR:
     * If logging is enabled, we should assert that the read config values are 
     * logged to the logfile by default - it is also of interest to see the name
     * of the config file.
     *
     * If logging is enabled, we'll adapt whatever settings there were, 
     * 'logcfgvals' gives us a way to override logging of configuration values.
     * because just setting it in the cfg file may be overridden.
     * 
     */
    if  (logcfgvals && LA_DBGLOG_ISENABLED ) {
        /* temporary tuning of config values to get the values printed out:
         *
         */
        La_loglvl oldstdlevel = LA_DBGLOG_GETSTD_LEVEL;
        La_loglvl oldloglevel = LA_DBGLOG_GETLEVEL ;

        La_loggrp oldstdgrp = LA_DBGLOG_GETSTD_GROUP ; 
        La_loggrp oldloggrp =  LA_DBGLOG_GETGROUP ;
        LA_DBGLOG_DELGROUP(oldloggrp);
        La_loggrp newloggrp =  LA_DBGLOG_GETGROUP ;

        (void)newloggrp;

        LA_DBGLOG_SETLEVEL(LA_LOGLVL1);
        LA_DBGLOG_SETSTD_LEVEL(LA_LOGLVL1); 
        LA_DBGLOG_SETSTD_GROUP(LA_LOGGRP1);
        LA_DBGLOG_ADDGROUP(LA_LOGGRP1) ;
        cfg_show_log_metadata();
        /* list configuration in log */
        LA_DBGLOG_ADDTOLOG("config_load_file: %s The current configuration:\n",fname);
        LA_DBGLOG_ADDTOLOG("%-15s\t%s\n","name", "value");
        for (newcfg = cfgdata; newcfg; newcfg = newcfg->next )
            LA_DBGLOG_ADDTOLOG("%-15s\t%s\n",newcfg->name, newcfg->value);

        LA_DBGLOG_STR("\nconfig_load done%s.\n");

        LA_DBGLOG_SETLEVEL(oldloglevel);
        LA_DBGLOG_SETSTD_LEVEL( oldstdlevel);

        LA_DBGLOG_DELGROUP(LA_LOGGRP1);
        LA_DBGLOG_ADDGROUP(oldloggrp) ;
        LA_DBGLOG_SETSTD_GROUP(oldstdgrp);
    }
}

#if 0
int main( void )
{
   /* STD_LOGGING_GROUP=(La_loglvl)LA_LOGGRP1 ; */
    printf( "Hello world!\n" );
    return 0;
}
#endif
/* return the value associated with a name */
char *config_get( char *name )
{
    Cfgvallst *link;
    for ( link = cfgdata; link && strcmp( link->name, name ); link = link->next );
    return ( link ? link->value : NULL );
}

/* return the integer value (converted from hex) associated with a
 * name, or return a default value if no value for the name is 
 * configured
 */
int config_get_hex( char *name, int dflt)
{
    char *val;
    int rtn;

    val = config_get(name);
    /* return converted value if can, otherwise default */
    return (!val || 1 != sscanf(val,"%x", &rtn) ? dflt : rtn );
    /* if there is a value, return the converted one.  */
}

/* return the integer value  associated with a name, or return a default 
 * value if no value for the name is configured */
int config_get_num( char *name, int dflt)
{
    char *val;
    int rtn;

    val = config_get(name);
    /* return converted value if can, otherwise default */
    return (!val || 1 != sscanf(val,"%d", &rtn) ? dflt : rtn );
    /* if there is a value, return the converted one.  */
}

/* return the truth value (0 for false, 1 for true ) associated with a 
 * name, or return a default value if no value for the name is configured */
int config_get_true( char *name, int dflt)
{
    char *val;
    int rtn;
    val = config_get(name) ;
    /* return converted value if can otherwise return default */
    if (!val || !strrchr("yYnN10tTfF",*val))
        rtn = dflt ;
    else
        rtn = !!strchr("yY1tT",*val);
    return rtn;
}

/* free up memory and delete list holding  config stack entries - only
 * need to call this if may be running low on memory and no longer 
 * need to call config "get" functions, otherwise don't bother.
 */
void config_done(void)
{
   Cfgvallst *link, *next ;
   LA_DBGLOG_STR("Config done"); 
   for (link= cfgdata; link;) {
       /* get next link before freeing memory holding it. */
       next = link->next;
       free(link->name);
       free(link);
       link = next ;
   }
    cfgdata = NULL ;
}

/* output a fatal error message and exit */
void config_err_exit( char *msg )
{
    fprintf( stderr, "Fatal Configuration Manager Error: %s\n", msg );
    LA_DBGLOG_LOG( LA_LOGLVL1, LA_LOGGRP1, fprintf( dbgfp, "Fatal Configuration Manager Error: %s\n", msg ) );
    exit( EXIT_FAILURE );
}
