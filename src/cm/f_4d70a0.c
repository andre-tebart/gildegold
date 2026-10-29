/* 0x4d70a0, the fighter slot f puts down the object it carries (+0x1ac) at its place */

void ai_Unk4d70a0(char *f)
{
    char *c, *o, *p, *a;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        ch_StopActions(*(char **)(c + 0x184));
        d3_FindObject(0, 0x40, 0, f + 0x10);
        o = *(char **)(c + 0x1ac);
        if (o != 0) {
            p = *(char **)(*(char **)(c + 0x184) + 0x34);
            d3_SetWorldPosRot(o, p + 0x4c, p + 0x18c);
            d3if_Show(o, 1);
            *(int *)(c + 0x1ac) = 0;
        }
        a = (char *)ch_AddAction(*(char **)(c + 0x184), 0x2e, 1);
        strcpy(a + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
    }
}
