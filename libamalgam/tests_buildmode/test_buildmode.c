/*
 * Testing labuildmode:
 */
#define LA_BUILDMODE_GET
#include <amalgam.h>
#undef LA_BUILDMODE_GET
#include "trampoline.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
#ifdef STUPID
    printf("STUPID == defined\n");
#endif
    juggler(); /* calls la_prbuildmode */
    return 0;
}
