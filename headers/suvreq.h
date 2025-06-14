/*
	Asking the system what it has (header)
	AUP2, Sec. 1.05.3

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

LICENCE:

This copyright notice applies to example source code files posted on this Web site that
are from the book Advanced UNIX Programming, 2nd Edition, to associated C++ (Ux) and Java
(Jtux) bindings, and to any other files whose copyright notice or other directives
reference this page. It's based on the BSD License.  This software is OSI Certified Open
Source Software. OSI Certified is a certification mark of the Open Source Initiative.

Copyright (c) 1985-2005 by Marc J. Rochkind. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met: bullet	Redistributions of
source code must retain the above copyright notice, this list of conditions and the
following disclaimer.  bullet	Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.  bullet	The name
of the copyright holder, Marc J. Rochkind, may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

WARNING: Some of the Example Files contain errors that were put in deliberately for
educational purposes, as explained in the book.
*/

#ifndef _SUVREQ_H_
#define _SUVREQ_H_
/*[suvreq-h]*/
/*
	Header to request specific standard support. Before including it, one
	of the following symbols must be defined (1003.1-1988 isn't supported):

		SUV_POSIX1990	for 1003.1-1990
		SUV_POSIX1993	for 1003.1b-1993 - real-time
		SUV_POSIX1996	for 1003.1-1996
		SUV_SUS1		for Single UNIX Specification, v. 1 (UNIX 95)
		SUV_SUS2		for Single UNIX Specification, v. 2 (UNIX 98)
		SUV_SUS3		for Single UNIX Specification, v. 3
*/

#ifdef _POSIX_SOURCE /* tmp */
#error
#endif

#if defined(SUV_POSIX1990)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 1

#elif defined(SUV_POSIX1993)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199309L

#elif defined(SUV_POSIX1996)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199506L

#elif defined(SUV_SUS1)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#elif defined(SUV_SUS2)
#define _POSIX_SOURCE
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199506L
#define _XOPEN_SOURCE 500
#define _XOPEN_SOURCE_EXTENDED 1

#elif defined(SUV_SUS3)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED 1

#elif defined(SUV_SUS4)
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED 1
#endif
/*[]*/

#endif /* _SUVREQ_H_ */
