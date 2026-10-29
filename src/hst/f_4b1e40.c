/* 0x4b1e40, marks the lists hst_Lists used and empty, frees the player's history texts, resets the text
   positions */

void ai_Unk4b1e40(void)
{
    char path[0x40];
    int i, j;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 1;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    sprintf(path, "historie\\text_H_%s", ai_PlayerSlots[ai_OwnPlayer]);
    txt_FreeFile(path);
    hst_TextFirst = -1;
    hst_TextLast = -1;
    hst_TextCurrent = -1;
    hst_KeyFirst = -1;
    ai_Unk601068 = -1;
    hst_KeyNext = -1;
}
