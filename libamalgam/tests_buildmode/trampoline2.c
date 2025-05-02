/* A dummy module to see if it works anyway. */

#define LA_BUILDMODE_USE
#include <amalgam.h>
#undef LA_BUILDMODE_USE
#include "trampoline.h"

void juggler(void)
{
    la_prbuildmode();
}
