/*Former ~/pub/libnumbers/src/read_num.c
 * Some simple rework to make it usable for 
 * simple programs that are just to read input from 
 * stdin.
 * First of all: needs more datataypes, like double and long.
 * I want a different error handling too.
 * AND I want passed in a prompt.
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

/** 
 * @brief
 * Very simple routine to read an integer.
 */
int readint(void)
{
    const int BUFSZ=64;
    char s[BUFSZ], *t,*u;
    long lval ;
    int val;
    if ((t=fgets(s,BUFSZ,stdin)) == NULL ) {
        fprintf(stdout,"^D\n");
        exit(EXIT_SUCCESS );
    } else { 
        lval= strtol(t,&u,10);
        if ( t == u && lval == 0 ) {
            fprintf(stderr,"No integer value received, exiting\n");
            exit(EXIT_FAILURE);
        } else if ( lval > (long) INT_MAX ||  lval < (long) INT_MIN) {
            fprintf(stderr,"Integer value received is out of range, exiting.\n");
            exit(EXIT_FAILURE);
        } else {
            val = (int) lval ;
            return val;
        }
    }
}

/** 
 * @brief
 * Very simple routine to read a float.
 */
float readfloat(void)
{
    const int BUFSZ=64;
    char s[BUFSZ], *t,*u;
    float val;
    if ((t=fgets(s,BUFSZ,stdin)) == NULL ) {
        fprintf(stdout,"^D\n");
        exit(EXIT_SUCCESS );
    } else { 
        val= strtof(t,&u);
        if ( t == u && val == 0 ) {
            fprintf(stderr,"No float value received, exiting\n");
            exit(EXIT_FAILURE);
        } else if ( val ==  FLT_MAX ||  val == -FLT_MAX ) {
            fprintf(stderr,"Float value received is out of range, exiting.\n");
            exit(EXIT_FAILURE);
        }
    }
    return val;
}
