/*
 * Testing labuildmode:
 */
#define LA_BUILDMODE_GET
#include <amalgam.h>
#undef LA_BUILDMODE_GET
#include "trampoline.h"

int main(int argc, char *argv[])
{
    juggler(); /* calls la_prbuildmode */
    return 0;
}
