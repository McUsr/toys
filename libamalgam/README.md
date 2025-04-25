README.md
=========

libamalgam:
-----------

The motiviation is to have an easy way to configure error handlling for the
different build modes through a compile time feature request macro.

This library is more of a software engineering effort than a programming
effort since most of the code is provided by programmers far better than me,
but I have assembled and customized it a little in order to integrate the
various parts into a more coherent whole to make it easy and productive
to use as at least I see fit. All the various parts are opensourced, provided
their copyright and licence is retained, and reproducible in binary form.

libamalgam because the library amalagates compile time setting of build mode
with detection of run time build modes  with respect to error handling, also
amalgates different error handlling strategies from various sources that each
work uniformlyfor the build modes "Development", "Release" and "Debug".

Because libamalgam uses exceptions for error handling an exception handling
module is also provided for the end user, since it is an intrinsic part of the
library.

Brief
-----

* You choose among the different error handling strategies design time.

* You define a Build mode compile time.

* You get the behavior run time according to build mode.


The different build modes are: "Development", "Release" and "Debug".

During Development I wan't to use "normal" out of the box error handling.

During Debug, I want to be taken straight into GDB at the point it crashed, so I
can inspect variables and the call stack.

During Release I want programs to produce a general error message, to the
comfort of the user, which should be spared the details, and for security
reasons, making a tiny bit harder for an adversary to exploit it.


### Error handling strategies

 * "Assert" : more or less like regular assertions, but acts differently for
 the different build-modes

 * "Failfast" : miscellanous wrappers for libc functions that fails with an
 error message, detailed with the error constant if it is wanted.


 * "Happypath" : standardized runtime checking of linux system calls and libc
 functions.

### Notes

libamalgam is made to be as portable as possible, but as it stands it is only
possible to compile link it as an ELF executable, and it relies on the `proc`
filesystem. This is just in order to make the shared library executable in order
to provide a binary reproducible licence.
This can be remedied by removing the mechanism for making the
library executable and just add a prototype to call the library's `licence()`
function, since the idea behind the executable mechanism is to provide
versioning and licence information.


  Last updated:25-04-25 23:14

<!--
vim: foldlevel=99 nospell
-->

