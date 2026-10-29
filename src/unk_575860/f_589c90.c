/* 0x589c90, the slot keys each frame (86 B): U_B (0x1468410) set and U_I (0x75e8c0) not 1; the key 0x58 ->
   0x589670 (); 0x5894d0 (); each slot 0x6f5ec0 + i * 0xc up to 0x6f5f44 whose byte 0 is the key, with
   U_B (0x14681ea) set -> 0x589570 (i) */

void ai_Unk589c90(void)
{
    char *p;
    int i;

    if (*(unsigned char *)(unsigned)&kb_Last == 0)
        return;
    if (U_I((unsigned)&sv_PanelFlag) == 1)
        return;
    if (*(unsigned char *)(unsigned)&kb_Last == 0x58)
        ai_Unk589670();
    ai_Unk5894d0();
    i = 0;
    for (p = (char *)(unsigned)&full_data_6f5ec0; p < (char *)(unsigned)((unsigned)full_data_6f5ec0 + 0x84); p += 0xc) {
        if (*(unsigned char *)p == *(unsigned char *)(unsigned)&kb_Last && *(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
            ai_Unk589570(i);
        i++;
    }
}
