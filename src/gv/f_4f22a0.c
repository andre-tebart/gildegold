/* 0x4f22a0, the entry of the list 0xff of the object o (0x4ef7c0 with 0x4ef770) whose byte +0xc is below m and the smallest of them; 0 when the owner is the player or has no item 0x1f8 (list 2, key 4) or none qualifies */

int unk_Unk4f22a0(char *o, unsigned char m)
{
    int best;
    unsigned e;

    best = 0;
    if (*(short *)(o + 0x27) != -1 && *(unsigned short *)(o + 0x27) != *(unsigned short *)ai_PlayerCharC
        && gm_Query(*(int *)(o + 0x5d), 2, 4, 0x1f8, 6) != 0) {
        for (e = gv_FindEntry(0, (unsigned char *)o, 0, gv_EntryIsUsed, 0xff); e != 0;
             e = gv_FindEntry(0, (unsigned char *)o, e, gv_EntryIsUsed, 0xff)) {
            if (*(unsigned char *)(e + 0xc) < m && (best == 0 || *(unsigned char *)(e + 0xc) < *(unsigned char *)(best + 0xc)))
                best = e;
        }
        return best;
    }
    return 0;
}
