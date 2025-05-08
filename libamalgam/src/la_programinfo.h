 /* Copyright © 1999 Lucent Technologies. All rights reserved. Sun Feb 3 10:16:10 EST 2013
  * https://web.archive.org/web/20230326061221/https://www.cs.princeton.edu/~bwk/tpop.webpage/code.html
  * Excerpted from 'The Practice of Programming'
  * by Brian W. Kernighan and Rob Pike (eprintf.[ch])
  * You may use this code for any purpose, as long as you leave the copyright notice and
  * book citation attached.
  * Parts Copyright © 2025 Tommy Bollman -- Mit Licence.  
  * I have made changes and additions but the founding idea is R.P's and B.W.K's.
  * The most significant change is the change of prefix from 'e' to 'la_e'.
  * (Other functions that originally didn't have an 'e' prefix has gotten an 'la_' prefix
  * as well.)
  * All eventual errors  are mine  the codes  comes with no warranties about what so ever, to
  * the extent permitted by law.
  * You may use it as you like as long as you retain the whole Copyright/Licence note.
  */

#ifndef LAPROGRAMINFO_H
#define LAPROGRAMINFO_H
 /*
 About eprintf2.c -- INTERFACE DOCUMENTATION

 * Most code, and most valuable parts by Brian W. Kernighan and Rob Pike, and Marc J. Rochkind.
 * I have amended/composed it to provide a kind of general solution for error communication
 * when making "tools", to have a one-stop for error and simple help messages, that should
 * be detailed enough, with the error code for any system call corresponding to your
 * platform (*Nix platform independent error codes.).
 *
 * This module concerns itself with "meta-output", i.e that is everything that is not
 * regular output from a programs normal operation. It also contains wrappers for system
 * calls that you would normall abort (fail fast) when they err with a message, and exit
 * with an exit code of 2 to signal to the user that it was a system call that failed.
 *
 * Exception: you define the usage()/help() function that provides detailed help about
 * command options, and other important information in your main program. This output goes
 * to stdout and not stderr. 
 *
 * You can provide a program name so that the programname is visible in any la_printf() or
 * la_die() messages you may have. This circumvents security concerns with regards to a
 * non existant argv[0].
 *
 * You can also provide a version string so that the version number is visible in
 * a simpleusage() function, and that you can reuse in your usage()/help() function, the -V version
 * of your commandline parsing is then implemented for free.
 *
 * Features:
 *
 * The `la_printf()`, `la_wprintf()` and `la_die()` functions has the quirk that if
 * their fmt strings * are ended by a  ':' then they will append an error
 * string, an error number and an error constant which * is specific for the
 * *nix platform in use.
 * (Some platforms vary in their naming of the error constants).
 *
 * Safety:
 *
 * the setprogname should be safe for exploitatin where the program is called without
 * argv[0], because then the fallback-name is used.
 *
 * 25-02-03 Further revision of the concepts: see comment marked "Revision: [main
 * cefba3a]" in eprintf2.c.
 *
 */

/* la_freeReleaseEmsg: frees the message installed for custom error message in relase
 * build LA_REL */ 
void la_freegeneralerrormsg();

/* la_setReleaseEmsg: Sets the message string for the generalized release error message. */

void la_setgeneralerrormsg(char *s);

/* la_releaseEmsg: Returns the release error message we have installed or the fallback message. */
const char *la_generalerrormsg();

/* la_freeprogname: frees program name. */
void la_freeprogname(void);

/* This call is optional, but no programname is ever written if it isn't called. */
void la_setprogname( const char *fallbackname, const char *argv_null,
        const int nrargs );

/* returns the program name  if it is set NULL otherwise. */
char *la_progname(void);

void la_setprogver( char *versionstr );

/* returns the program version string if it is set NULL otherwise. */
char *la_progver( void );

#endif


