/* A dummy module to see if it works anyway. */

#define LA_CONF_SENTINEL
#include <poc.h>
#undef LA_CONF_SENTINEL
#include "trampoline.h"

void juggler(void)
{
    prbuildmode();
}
