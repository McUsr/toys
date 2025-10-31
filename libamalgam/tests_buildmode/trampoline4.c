/* A dummy module to see if it works anyway. */

/* #define LA_BUILDMODE_USE */
#include <stdlib.h>
#include <stdio.h>
#include <amalgam.h>
#include "trampoline.h"

void juggler(void)
{
    LA_BuildMode curbuild = la_buildmode() ;
    
    if ( curbuild == LA_DBG )
     printf(" curbuild == LA_DBG\n" );
    else if ( curbuild == LA_DEV )
     printf(" curbuild == LA_DEV\n" );
    else if ( curbuild == LA_REL )
     printf(" curbuild == LA_REL\n" );
    else {
     printf(" curbuild == UNDEFINED\n" );
     exit(EXIT_FAILURE);
    }

    
    /* la_prbuildmode(); */
}
