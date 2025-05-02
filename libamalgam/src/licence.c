/**
 * @file
 * Contains licencsing function and a stand alone entrypoint
 * for showing the library when the the librar is executed.
 * Most of the code in here is
 * Copyright (c) 2021 Andrew G. Morgan <morgan@kernel.org>
 * https://web.git.kernel.org/pub/scm/libs/libcap/libcap.git/tree/libcap/execable.h
 *
 * We pass in the version triplet of the librar as a compile time define
 * BEFORE we create and commit the version tag to github, which happens
 * after the version has been tested.
 * This, for syncronicity between releases and numbers.
 */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
char *LIBNAME = "libmamalgam.so"; /* we add in the rest of the name here. */
/* You need to update the name here, by seeing what name libtool gives, and
 * remake the project with everything else intact.
 */
char *LIBVERSION = CUR_VER;

const char dl_loader[] __attribute__((section(".interp"))) = DL_LOADER;

static void __execable_parse_args(int *argc_p, char ***argv_p)
{
	int	   argc = 0;
	char **argv = NULL;
	FILE  *f	= fopen("/proc/self/cmdline", "rb");
	if (f != NULL) {
		char  *mem	= NULL, *p;
		size_t size = 32, offset;
		for (offset = 0;; size *= 2) {
			char *new_mem = realloc(mem, size + 1);
			if (new_mem == NULL) {
				perror("unable to parse arguments");
				fclose(f);
				if (mem != NULL) {
					free(mem);
				}
				exit(1);
			}
			mem = new_mem;
			offset += fread(mem + offset, 1, size - offset, f);
			if (offset < size) {
				size	  = offset;
				mem[size] = '\0';
				break;
			}
		}
		fclose(f);
		for (argc = 1, p = mem + size - 2; p >= mem; p--) {
			argc += (*p == '\0');
		}
		argv = calloc(argc + 1, sizeof(char *));
		if (argv == NULL) {
			perror("failed to allocate memory for argv");
			free(mem);
			exit(1);
		}
		for (p = mem, argc = 0, offset = 0; offset < size; argc++) {
			argv[argc] = mem + offset;
			offset += strlen(mem + offset) + 1;
		}
	}
	*argc_p = argc;
	*argv_p = argv;
}
static void mu_help(void);
static void mu_usage(void);
static void mu_licence(void);

/* mu_main is our own ENTRY-POINT. */
__attribute__((force_align_arg_pointer)) void mu_main(void) /* attribute to help
															   i386 SEE
															   compatibility. */
{
	char **myargs = NULL;
	int	   numargs;
	__execable_parse_args( &numargs, &myargs );
	for ( int i = 1; i < numargs; i++ ) {
        if (!(strcmp(myargs[i],"--usage")))
            mu_usage() ;
        else if (!(strcmp(myargs[i],"--licence")))
            mu_licence() ;
        else if (!(strcmp(myargs[i],"--help")))
            mu_help() ;
        else {
	        fprintf(stderr, "%s: %s isn't a valid argument.\n", LIBNAME,myargs[i]);
            mu_help() ;
            break ;
        }
    }
    if (numargs == 1 )
        mu_help() ;

    free(*myargs);
    free(myargs);
	exit(0);
}

static void mu_help(void)
{
	fprintf(stderr, "%s [--help | --usage | -- licence ] \n", LIBNAME);
	fprintf(stderr, "%s version: %s\n", LIBNAME, LIBVERSION);
	fprintf(stderr, "Copyright (c) 2025 Tommy Bollman -- MIT licence.\n");
	fprintf(stderr, "%s --licence for full copyright and licence information.\n", LIBNAME);
}

static void mu_usage(void)
{
	fprintf(stderr, "%s version: %s\n", LIBNAME, LIBVERSION);
	fprintf(stderr, "Copyright (c) 2025 Tommy Bollman.\n");
    fprintf(stderr,"%s usage:\n",LIBNAME);
    fprintf(stderr,"See: \"man 7 amalgam\" and \"man 3 amalgam.h\" for documentation.\n"); 
}

static void mu_licence(void)
{
	fprintf(stderr, "%s version: %s\n", LIBNAME, LIBVERSION);
	fprintf(stderr, "Copyright (c) 2025 Tommy Bollman -- MIT licence.\n");
	fprintf(stderr, "Parts Copyright (C) 1999 Lucent Technologies \n");
	fprintf(
		stderr,
		"Parts of the code used from  B.W Kernighan and R. Pike an \"The Practice of Programming\".\n");
	fprintf(
		stderr,
		"Parts Copyright (c) 1985-2005 by Marc J. Rochkind. All rights reserved.\n");
	fprintf(
		stderr,
		"Parts of the code used from  Marc J. Rochking \"Advanced Unix Programming.\".\n");
    fprintf(
		stderr,
		"Parts Copyright (c) 1996 by David R. Hanson. All rights reserved.\n");
	fprintf(
		stderr,
		"Parts of the code used from  David R. Hanson \"C Interfaces and Implementation\".\n");
	fprintf(stderr, "Parts Copyright (c) 2021 Andrew G. Morgan <morgan@kernel.org>:\n"
					"the code and howto for making the library executable.\n");
	fprintf(stderr, "License:\n");
	fprintf(
		stderr,
		"This is free software: you are free to change and redistribute it, as long as\n");
	fprintf(
		stderr,
		"you don't infringe upon the copyright of the other copyright holders.\n");
	fprintf(
		stderr,
		"-You must include their copyright notice  and licensing terms as written in their\n"
		" source code, reproducible in binary form.\n");
	fprintf(stderr,
			"There are NO WARRANTIES to the extent permitted by law.\n");
}
