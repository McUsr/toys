/*
 * Testing poc:
 */
#define LA_BUILDMODE_GET
#include <poc.h>
#undef LA_BUILDMODE_GET
#include "trampoline.h"

int main(int argc, char *argv[])
{
    /* prbuildmode(); */
    juggler();
    return 0;
}
