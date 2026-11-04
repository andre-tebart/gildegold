/* tr.c -- original file name unknown ("tr_startup" string). Extent in GildeGold.exe (inferred):
   0x42ea90 (after the time base, tb.c) up to 0x42ec95; the virtual file system (vfs.c)
   starts at 0x42eca0. Its .data: tr_Key1/tr_Key2 at 0x5f8fd0 with their strings, then "tr_startup".
   The random helpers may be a TU of their own (no data to tell). Functions are kept in their original
   order. Names are ours except tr_startup.
   unit of rea 8cef7d5b (src/game/tr.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
#include "tr.h"
#include "mem.h"

char *tr_Key1 = "*bRaIn DeaD%&";  /* 0x5f8fd0 */
char *tr_Key2 = "%aRscH Ab :-)";  /* 0x5f8fd4 */
TRREC * tr_First = 0;  /* 0x6333f0 */
TRREC * tr_End = 0;  /* 0x6333f4 */
TRREC * tr_Table = 0;  /* 0x6333f8 */

/* 0x42ea90, allocates n records and fills them with the two marker strings. */
int tr_startup(unsigned short n)
{
    TRREC rec;
    TRREC *p;

    if (!n)
        return -1;
    if (tr_Table)
        return -2;
    tr_Table = mem_Alloc(n * sizeof(TRREC), "tr_startup");
    if (!tr_Table)
        return -3;
    tr_First = tr_Table;
    tr_End = tr_Table + n;
    TR_ZERO(&rec, sizeof(rec));
    strncpy(rec.key1, tr_Key1, 13);
    strncpy(rec.key2, tr_Key2, 13);
    for (p = tr_First; p < tr_End; p++)
        TR_COPY(p, &rec, sizeof(rec));
    return 0;
}
/* 0x42ebc0, frees the record table tr_startup allocated and clears tr_Table/tr_First/tr_End;
 * int 3 when no table is open or it lies below tr_First (an assertion kept from the original) */

void tr_shutdown(void)
{
    if (tr_Table && tr_Table >= tr_First) {
        mem_Free(tr_Table);
        tr_First = NULL;
        tr_End = NULL;
        tr_Table = NULL;
    } else
        __asm int 3
}
/* 0x42ec00 */

int util_Random(unsigned short n)
{
    if (n)
        return rand() % n;
    return 0;
}
/* 0x42ec20 */

float util_RandomF(void)
{
    return (float)rand() / RAND_MAX;
}
/* 0x42ec40 */

float util_RandomSq(void)
{
    float x = util_RandomF() * 2.0f - 1.0f;
    float s;

    s = x < 0.0f ? -1.0f : 1.0f;
    return s * x * x;
}
/* 0x42ec70 */

float util_RandomF2(void)
{
    float x = util_RandomF();

    return x * x;
}
/* 0x42ec80, seeds the C runtime random generator with the current time (timeGetTime, ms) */

void util_Randomize(void)
{
    srand(timeGetTime());
}
/* 0x42ec90, seeds the C run-time random generator */

void util_Seed(unsigned int seed)
{
    srand(seed);
}
