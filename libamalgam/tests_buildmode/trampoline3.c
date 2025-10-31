/* A dummy module to see if it works anyway. */

#define LA_BUILDMODE_USE
#undef LA_BUILDMODE_USE
#include <amalgam.h>
#include "trampoline.h"

void juggler(void)
{
    LA_BuildMode curbuild = la_buildmode() ;
    la_prbuildmode(curbuild); 
    /* la_prbuildmode(); */
}
