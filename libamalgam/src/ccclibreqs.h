/*
 * This include file sees to that we are compiled with what we need with respect to
 * feature request macros on the gcc command line that ensures that we have the standard
 * we need.
 *
 *
 */
#ifndef CCCLIBREQS_H
#define CCCLIBREQS_H
#ifdef __STDC_VERSION__
#   if !  (__STDC_VERSION__ >= 199901L) 
#       error "Needs support for c99." 
#   endif
#else
#   error "__STDC_VERSION__ is not defined (likely an older C standard)"
#endif

/* Now that that's sorted telling the system what we want. */
#ifdef _XOPEN_SOURCE
    #undef _XOPEN_SOURCE
#endif 
#define _XOPEN_SOURCE 500
#define _POSIX_C_VERSION 200809L

#ifdef __GNUC__
#   ifndef _GNU_SOURCE 
#       define _GNU_SOURCE /* Need for EAI_ADDRFAMILY and EAI_NODATA */
#   endif
#endif
#include <unistd.h>
/* we need to have unistd.h or other includedd before checking __GLIBC__ 
 * symbols. */

#ifdef __GLIBC__
#   if __GLIBC__ >= 2
#       if  __GLIBC_MINOR__ < 19 
#           error "This library requires glibc >= 2.19"
#       endif
#   else
#           error "This library requires glibc >= 2.19"
#   endif
#else
#   error "This library requires glibc >= 2.19"
#endif


/* checking if the system can deliver what we need 
 * define _BSD_SOURCE or _SVID_SOURCE yourself if thats' the only options.
 * you have for making strdup work.
 *
 * Please make a pull request if you think you have a better way to ensure
 * compatibility.
 *
 */
#if ! defined(_BSD_SOURCE) && !defined(_SVID_SOURCE )
#   if defined(_POSIX_VERSION)
#       if _POSIX_C_VERSION < 200809L
#           error "POSIX_C_VERSION < 200809L"
#       endif
#   elif defined(_XOPEN_UNIX )
#       if _XOPEN_VERSION < 500
#           error "XOPEN_VERSION < 500"
#       endif
#   else
#       error "not _XOPEN_UNIX nor POSIX SYSTEM nor BSD or SVID compatibility."
#   endif
#endif

#endif
