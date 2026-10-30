/* 0x4b4eb0, mission condition: no building of the 0x501190(1, 6) list is held by a character of kind 5 */

int ai_Unk4b4eb0(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;
    unsigned short w;

    for (b = G13_501190(1, 6); b != 0; b = gm_BuildNext()) {
        w = *(unsigned short *)((char *)b + 0x27);
        if (w != 0xffff && ai_CharsT[w].kind == 5)
            return 0;
    }
    return 1;
}
