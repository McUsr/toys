/*
 * Testing poc without specifiying EC_PARSE_BUILDMODE:
 * This test should fail link time.
 */
#include <amalgam.h>
#include "trampoline.h"

int main(int argc, char *argv[])
{
    /* prbuildmode(); */
    juggler();
    return 0;
}
