
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
#define LA_CONF_SENTINEL
/* So that we not inadvertently compiles the run time func in the
 * library! */
#include "labuildmode.h"
#undef LA_CONF_SENTINEL

#include <stdio.h>
/**
 * @brief A debug function to help analyze any problems.
 */
void la_prbuildmode(void)
{
    LA_BuildMode curmode = LA_buildmode() ;
    switch(curmode) {
        case LA__DEV:
          printf("LA__DEV\n") ;  
          break;
        case LA__DBG:
          printf("LA__DBG\n") ;  
          break;
        case LA__REL:
          printf("LA__REL\n") ;  
          break;
        default:
          printf("Can't happen! Something is very wrong!\n");
          break;
    }
}
