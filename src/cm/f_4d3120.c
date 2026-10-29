/* 0x4d3120, the formation order for the character c of the fight record rec2 by its formation type (+0x1c4: 0 the best free
   cell near the character for the kind 2 (0x4d22d0 / 0x4cffd0), else to the closest selected (0x4d1f10); 1 the closest "WARE_"
   0x4d25a0 -> 0x4d0380; 2 the closest "sp_CONQUER" 0x4d2680 -> 0x4d02d0; 3 0x4d0520; 4 0x4d0460; 5 the closest "sp_ESCAPE"
   0x4d2490 -> 0x4d0200). Rough draft. */

void ai_Unk4d3120(char *rec, char *c)
{
    char *sc, *k, *o;
    int x, y, h;
    float v[4];
    int r;

    if (c != 0 && *(int *)(c + 0x184) != 0) {
        if (c[8] == 0)
            return;
        if (*(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) == 1)
            return;
    }
    switch ((unsigned char)c[0x1c4]) {
    case 0:
        k = ai_Unk4cda50(c);
        if ((*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            return;
        if (*(int *)(k + 0x58) == 2) {
            sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
            if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &x, &y, &h) == 0)
                return;
            r = (int)(500.0f / *(float *)(sc + 0x10));
            if (ai_Unk4d22d0(c, x, y, r, &x, &y) == 0)
                return;
            ai_Unk4cffd0(rec, c, 0, x, y);
            return;
        }
        o = ai_Unk4d1f10(c);
        if (o == 0)
            return;
        ai_Unk4cffd0(rec, c, o, 0, 0);
        return;
    case 5:
        o = ai_Unk4d2490(c);
        if (o == 0)
            return;
        ai_Unk4d0200(rec, c, o);
        return;
    case 1:
        o = ai_Unk4d25a0(c);
        if (o == 0)
            return;
        G19_4D0380(rec, c, o);
        return;
    case 2:
        o = ai_Unk4d2680(c);
        if (o == 0)
            return;
        ai_Unk4d02d0(rec, c, o);
        return;
    case 4:
        ((int (*)(char *, char *))ai_Unk4d0460)(rec, c);
        return;
    case 3:
        ai_Unk4d0520(rec, c);
        return;
    }
}
