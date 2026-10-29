/* 0x4d7000, the fighter slot f picks up the object lying at its place (+0x10): the object (0x449690) is marked as owned
   by the character's side, event 0x4d3ae0 */

void ai_Unk4d7000(char *f)
{
    char *c, *side, *o, *ev;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        side = *(char **)(c + 0x16c);
        ch_StopActions(*(char **)(c + 0x184));
        o = (char *)d3_FindObject(0, 0x40, 0, f + 0x10);
        if (o != 0) {
            *(int *)(*(char **)(o + 0x200) + 0xa) = G20_US(side);
            ev = (char *)ch_AddActionCB(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3ae0, 5, 0x2e, 1);
            *(char **)(ev + 0x17c) = c;
            *(char **)(ev + 0x180) = o;
            strcpy(ev + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
        }
    }
}
