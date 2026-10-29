/* 0x4b44e0, counts event ev (11, 19, 23, 28, 40) for the running mission of m's owner when it is of that type */

#define G13_B(c, off) (((unsigned char *)(c))[off])
#define G13_501190 ((AIBLD *(*)(int, ...))gm_BuildQuery)  /* the variadic view (am.h repeats a 3-int one) */

int ai_TimedCondition(int x, int ok, int y);                                          /* 0x4b5160 */

int ai_Unk4b44e0(G13MIS *m, int ev)
{
    G13MIS *e = ai_FindOwnerMission(m);

    if (e != 0) {
        switch (ev) {
        case 11:
        case 19:
        case 23:
        case 28:
        case 40:
            if (e->type == ev)
                e->unk1c++;
            return 1;
        }
    }
    return 0;
}
