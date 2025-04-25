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

/**> global type used by all clients of the buildmode module */
typedef enum { LA__REL=0, LA__DEV=1, LA__DBG=2 } LA_BuildMode ;

#   ifndef LA_CONF_SENTINEL
#       if defined(LA_BUILDMODE_GET)
#           ifdef LA_DBG
#               if defined(LA_DEV) || defined(LA_REL)
#                   error"You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros." 
#               endif
                LA_BuildMode LA_buildmode(void) 
                {
                    return LA__DBG ;
                }
#               undef LA_BUILDMODE_GET
#           elif defined (LA_DEV)
#               if defined(EC_DBG) || defined(LA_REL)
#                   error "You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros."
#               endif
                LA_BuildMode LA_buildmode(void) 
                {
                    return LA__DEV ;
                }
#               undef LA_BUILDMODE_GET
#           elif defined (LA_REL)
#               if defined(EC_DBG) || defined(LA_DEV)
#                   error "You can only define one of the LA_DEV, LA_REL and EC_DBG feature request macros."
#               endif
                LA_BuildMode LA_buildmode(void) 
                {
                    return LA__REL ;
                }
#               undef EC_PARSE_BUILDMODE
#           else
                LA_BuildMode LA_buildmode(void) 
                {
                    return LA__DEV ;
                }
                /* The function is declared here, free for use by clients
                 * that needs to know what kind of build mode this is.  */ 
#           endif
#       endif
#   else
        LA_BuildMode LA_buildmode(void) ;
        /* The function is declared here, free for use by clients
         * that needs to know what kind of build mode this is.  */ 
#   endif
/* prbuildmode: A debug function to help analyze any problems. */
void prbuildmode(void);
#endif /* include guard */
