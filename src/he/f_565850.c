/* 0x565850, the run handler that gives a building (+0xb4: the object 0x502020) a new worker (ROUGH): kind 2 buildings get a random hire 0x4c4650 (2, -1, -1, random(5, ..) + 0x16), the others the work 0x4c55d0 with the last non-empty slot of the record (+0x223 .. +0x228) and +0x230; the result is stored at +0xac */

void he_Unk565850(int ev)
{
    unsigned char *p;
    char *rec;
    int r, i, n, v, u;
    char c;

    c = 0;
    p = (unsigned char *)gm_FindBuildingById(*(int *)(ev + 0xb4));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    rec = (char *)(U_I((unsigned)&ai_BTypes) + *p * 0x24d);
    if (*rec == 2) {
        u = (unsigned short)((int (*)(int, int))util_Random)(0xc, 1);
        v = gm_TitleNumber(u + 1);
        n = ((int (*)(int, int, int, int, int))util_Random)(5, -1, v, 0, 2);
        r = ((int (*)(int, int, int, int))cm_SendCreateCharacter)(2, -1, -1, n + 0x16);
    } else {
        i = 5;
        do {
            if (rec[i + 0x223] != 0) {
                c = rec[i + 0x223];
                break;
            }
            i--;
        } while (-1 < i);
        r = cm_SendMessage3d(p, 1, rec[0x230], c);
    }
    *(int *)(ev + 0xac) = r;
    *(HETIME *)(ev + 0x52) = ai_Time;
}
