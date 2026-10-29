/* 0x4f4d30, the record id of the 0x91f700 entry (0x4f36e0) into: *c = the character's slot, *v = its liking (0x50c550 (c, *a, 1)) as an int, *k = the table record's low byte; 1 when the record exists and its table record has
   +0x18 set and the owner character is alive. ROUGH */

int unk_Unk4f4d30(int id, short *a, int *v, short *c, unsigned *k)
{
    int i;
    short *ch;
    int rec[9];

    if (id != -1) {
        i = unk_Unk4f36e0(id);
        if (i != -1) {
            if (gv_GetTableRecord(*(unsigned char *)(gv_Graveyard + 0x1c + i * 0x2d), rec) != 0 && rec[8] != 0) {
                ch = (short *)gm_FindChar(*(int *)(gv_Graveyard + 0x16 + i * 0x2d));
                if (ch != 0) {
                    *c = *ch;
                    gm_Unk505e60(*ch, 1);
                    *v = (int)gm_Liking(*ch, *a, 1);
                    *k = rec[0] & 0xff;
                    return 1;
                }
            }
        }
    }
    return 0;
}
