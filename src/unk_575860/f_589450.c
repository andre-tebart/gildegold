/* 0x589450, validates the 12-byte slots 0x6f5ec4 .. 0x6f5f4c (113 B): a slot's +0 (not -1) through
   0x502020 (id): 0 -> 0x589400 (the slot); else the +4 value through 0x4fe980: 0 while +4 is not -1 ->
   0x589400, else 0x503530 (the player word, id, that, 0) without bit 0 -> 0x589400 */

char * gm_FindBuildingById(int id); int gm_FindObjectInst(int id); int unk_Unk503530();

void ai_Unk589450(void)
{
    char *p;
    int id;
    int e;

    for (p = (char *)(unsigned)&full_data_6f5ec8; p < (char *)(unsigned)((unsigned)full_data_6f5ec8 + 0x84); p += 0xc) {
        if (*(int *)(p - 4) == -1)
            continue;
        id = gm_FindBuildingById(*(int *)(p - 4));
        if (id == 0) {
            ai_Unk589400((int *)(p - 8));
            continue;
        }
        e = gm_FindObjectInst(*(int *)p);
        if (e == 0 && *(int *)p != -1) {
            ai_Unk589400((int *)(p - 8));
            continue;
        }
        if ((unk_Unk503530(*(unsigned short *)(unsigned)&ai_PlayerId, id, e, 0) & 1) == 0)
            ai_Unk589400((int *)(p - 8));
    }
}
