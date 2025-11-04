#ifndef AMALGAM_H
#define AMALGAM_H
/*  Copyright (C) 2025 Tommy Bollman
 *  Copyright 1985-2006 Marc J. Rochkind All rights reserved.
 *  Source code Copied and modified from "advanced Unix Programming.
 *  His licence and copyright must be reproducible in binary form.
 *  Parts Copryight (C) Rob Pike and Brian W. Kernighan
 *  Source code coped and modified from the book "The Practice Of Programming."
 *  Parts Copyright (C) 1999 Lucent Technologies
 *  Parts Copryight (C) 1996 David R. Hansson.
 *  Soure code copied and modifed from the book "C Interfaces and Implementation"
 *  Parts Copyright (c) 2021 Andrew G. Morgan <morgan@kernel.org>:
 *  The code and howto for making the library executable.
 *  There are NO WARRANTIES to the extent permitted by law.
 *  You have the licence to use this freely according to the Mit licence, and provided
 *  that this licence header stays that include the copyright notice of all the 
 *  stakeholders in the software and that you can reproduce the
 *  licence statment in binary form. 
 *  SPDX-License-Identifier: MIT
 */

#ifdef _ASSERT_H
#   if _ASSERT_H == 1
#       error "standard assert.h included before amalgam.h"
#   endif
#endif

#include "ccclibreqs.h"
#include <stdbool.h>
#include "la_buildmode.h"
/* header file for setting the build mode compile time. See: `man 7 amalgam`
 * `man 3 amalgam.h` `man 3 labuildmode.h`. */
#include "la_dbglog.h"
#include "la_programinfo.h"
/* header file for setting the release message for LA_REL mode, and other 
 * program meta data like program name and version. */
typedef enum {EC_ERRNO = 0, EC_EAI = 1, EC_GETDATE = 2, EC_NONE = 3} EC_ERRTYPE;

/* EC_ERRTYPE is a common type shared between both "la_syserr" and "la_happypath" */

#define EC_EINTERNAL INT_MAX
/* EC_EINTERNAL: value to signal an internal error in a library func or whatever. */
#include "la_syserr.h"
/* #include "la_logf.h" */
#include "la_happypath.h"

#endif
