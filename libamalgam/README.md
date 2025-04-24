README.md
=========

libamalgam:
-----------

The motiviation is to have an easy way to configure error handlling for the
different build modes through a compile time feature request macro.

You choose among the different error handling strategies design time.

The different build modes are: "Development", "Release" and "Debug".

During Development I wan't to use "normal" out of the box error handling.

During Debug, I want to be taken straight into GDB at the point it crashed, so I
can inspect variables and the call stack.

During Release I want programs to produce a general error message, to the
comfort of the user, which should be spared the details, and for security
reasons, making a tiny bit harder for an adversary to exploit it.

libamalgam because the library amalagates compile time setting of build mode
with detection of run time build modes  with respect to error handling,
providing different error handlling strategies that each work  for the build
modes "Development", "Release" and "Debug".

Because libamalgam uses exceptions for error handling an exception handling
module is also provided for the end user, since it is an intrinsic part of the
library.


### Error handling strategies

 *  "Failfast" : miscellanous wrappers for libc functions that fails with an
 error message, detailed with the error constant if it is wanted.

 * "Assert" : more or less like regular assertions, but acts differently for
 the different build-modes

 * "Runtime" : standardized runtime checking of linux system calls and libc
 functions.

### Notes

libamalgam is made to be as portable as possible, but as it stands it is only
possible to compile link it as an ELF executable. This can be remedied by
removing the mechanism for making the library executable and just add a call 
to the libraries `licence()` function, since the idea behind the executable
mechanism is to provide versioning and licence information.


  Last updated:25-04-24 11:59

<!--
vim: foldlevel=99 nospell
-->

