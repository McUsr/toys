/* Copyright © 1999 Lucent Technologies. All rights reserved. Sun Feb 3 10:16:10
 * EST 2013
 * https://web.archive.org/web/20230326061221/https://www.cs.princeton.edu/~bwk/tpop.webpage/code.html
 * Excerpted from 'The Practice of Programming'
 * by Brian W. Kernighan and Rob Pike
 * You may use this code for any purpose, as long as you leave the copyright
 * notice and book citation attached.
 *
 * Uses code from Marc J. Rochkind's Advanced Unix Programming version 2 that
 * comes with its own Licence. (../private/macrostr.h)
 *
 * 25-02-03 Now enhanced a little bit in order to work in collusion with code
 * kindly provided by David R. Hanson from his "C Interfaces and Implementation,
 * namely excepct.c and assert_cii.c in order to unify error messages in general
 * when in end-user (REL-release) mode. That code I believe is copyright him.
 *
 * All faults are mine, and no warranties what so ever to the extent permitted
 * by law!
 *
 * 25-02-03 McUsr/Tommy Bollman.
 */
#include "amalgam.h"
#include "la_programinfo.h"

#if 1 == 1
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#endif

/* Revision: [main cefba3a]
 * I have started to use exception handling, to trap assertions, in order to
 * provide a more "end-user" friendly error message, so exception and assert is
 * now incorporated in libmiscutil.
 *
 * Include the regular `assert.h` during initial development for maximum details
 * when a checked runtime error occurs (assert() is triggered). Use this when
 * the software is released, including the TRY ELSE FINALLY ENDTRY except.h
 * exception handling in main. (When using the regular assert, that code is
 * stale, but that is what you want anyway then ).
 *
 * I developed the idea to cover up for the "eprintf" routines as well, which
 * deal with system calls for the most part that we discover has gone awry by
 * their return value, and errno, and not assertions.
 *
 * The module is now runtime configurable by choice, and for the case we
 * configure the system by default to be in REL - release mode, we print a user
 * friendly message, this message we can write ourselves, and serves as a
 * unified general message if something other than user error ("die()") happens.
 *
 * In end user mode (REL - release), the "die" message still provides an error
 * message as to what went wrong, giving the user a fair chance to remedy the
 * sitution, but any details such as error codes are omitted.
 *
 * The warning message (weprintf) are muted when in end user mode in order to
 * not create uncertainty.
 *
 * In order to get end-user error messsages you need to
 * 'setErrorMessageMode(REL)' in your code, since 'DEV' is the default.
 *
 *
 */


static const char *fall_back_release_emsg = "An unrecoverable error happened.";
static char		  *release_emsg = NULL; /* mem freed by eprintf if used by it */

/* should be called before we Reraise when in dev mode. */
void la_freegeneralerrormsg()
{
	if (release_emsg != NULL) {
		free(release_emsg);
        release_emsg = NULL;
	}
}

void la_setgeneralerrormsg(char *s)
{
    if (s == NULL ) {
		fprintf(stderr,"la_setReleaseEmsg: the string s == NULL. Exiting.\n" );
        exit(EXIT_FAILURE) ;
    }
	release_emsg = strdup(s);
	if (atexit(la_freegeneralerrormsg) != 0) {
		fprintf(stderr,"la_setReleaseEmsg: Couldn't install atextit handler. Exiting.\n" );
        exit(EXIT_FAILURE) ;
	}
}

const char *la_generalerrormsg()
{
	if (release_emsg == NULL)
		return fall_back_release_emsg;
	else
		return release_emsg;
}


static char *la_prog_name_str = NULL;

void la_freeprogname(void)
{
    if ( la_prog_name_str != NULL ) {
        free(la_prog_name_str);
        la_prog_name_str = NULL ;
    }
}

/* la_setprogname: sets the progname for use in messaages, using a fallback if argv[0]
 * haven't been set.  */
void la_setprogname(const char *fallbackname, const char *argv_null,
				 const int nrargs )
{
	char *tmpname;
	if (nrargs > 0) {
		if ((tmpname = strdup(argv_null)) == NULL) {
			fprintf(stderr,
				"la_setprogname: strdup couldn't allocate memory for argv_null. Exiting.\n");
            exit(EXIT_FAILURE);
        } else if ((la_prog_name_str = strdup(basename(tmpname))) == NULL) {
			fprintf(stderr,
				"la_setprogname: strdup couldn't allocate memory for tmpname. Exiting.\n");
            exit(EXIT_FAILURE);
        } else {
			free(tmpname);
        }
	} else if (fallbackname == NULL) { // TODO: Missing test here for fbn[0] == '\0' ); then, remove assert.
		fprintf(stderr,"la_setprogname: fallbackname == NULL. Exiting.\n");
        exit(EXIT_FAILURE);
    } else if (fallbackname[0] == '\0') {
		fprintf(stderr,"la_setprogname: fallbackname[0] == '\\0'. Exiting.\n");
        exit(EXIT_FAILURE);
    } else if ((la_prog_name_str = strdup(fallbackname)) == NULL) {
		fprintf(stderr,
			"setprogname: strdup couldn't allocate memory for fallbackname. Exiting\n");
        exit(EXIT_FAILURE);
    }

    if (atexit(la_freeprogname) != 0) {
        fprintf(stderr,"la_setprogrname atexit failed when installing la_freeprogname. Exiting.\n");
        exit(EXIT_FAILURE);
    }
}

/* returns the progname or NULL if it isn't set. */
char *la_progname(void)
{
	return la_prog_name_str;
}

static char *pversion = NULL;

void la_setprogver(char *versionstr)
{
    if (versionstr != NULL && versionstr[0] != '\0' )
	    pversion = versionstr;
}

/* returns the progver or NULL if it isn't set. */
char *la_progver(void)
{
	return pversion;
}
