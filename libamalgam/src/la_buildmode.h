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
 * @brief Parses build mode from command line feature request macro.
 * SPDX-License-Identifier: MIT
 */
#ifndef BUILDMODE_H
#define BUILDMODE_H

#   ifndef AMALGAM_H
#       error "include file not included by amalgam.h"
#   endif
/* Checking for illegal definitions */
#ifdef LA_DEV
#   undef LA_DEV
#   define LADEV
#endif
#ifdef LA_DBG
#   undef LA_DBG
#   define LADBG
#endif
#ifdef LA_REL
#   undef LA_REL
#   define LAREL
#endif
/**> global type used by all clients of the buildmode module */
typedef enum { LA_DEV=0,LA_DBG=1, LA_REL=2 } LA_BuildMode ;

LA_BuildMode la_buildmode(void);
/* extern LA_BuildMode la_buildmode(void); */
extern LA_BuildMode la_dbgbuildmode(void);
extern LA_BuildMode la_devbuildmode(void) ;
extern LA_BuildMode la_relbuildmode(void) ;

#if defined (LADBG)
#   if defined(LADEV) || defined(LAREL)
#       error "You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros." 
#   endif
#   define la_buildmode() la_dbgbuildmode()
#elif defined (LADEV)
#   if defined(ECDBG) || defined(LAREL)
#       error "You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros." 
#   endif
#   define la_buildmode() la_devbuildmode()
#elif defined (LAREL)
#   if defined(ECDBG) || defined(LADEV)
#       error "You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros." 
#   endif
#   define la_buildmode() la_relbuildmode()
#else
#   define LADEV
#   define la_buildmode() la_devbuildmode()
#endif
/* prbuildmode: A debug function to help analyze any problems. */
void la_prbuildmode(LA_BuildMode curmode );
#endif /* include guard */
