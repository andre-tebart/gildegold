/* 0x5944e0, the pickpocket callback (685 B, ROUGH SKELETON from the disassembly; an unnamed Ghidra start, the
   callback of 0x594790): rec (0 = return); 0x53e400 (1, 0, 0x62) walks the buildings, the one with +0xb0 ==
   rec + 1 gives the text 0x5176d0 (0x1a91, rec + 0x27, rec) and 0x521150; else the first character flag
   0x770d08 (0x300 entries of 0x21c bytes); when none is set the message 0x1440ba4 goes to 0x521150; else
   0x553cc0 (rec) decides: 0 gives the text 0x1a89 (0x5176d0 / 0x521150), else the bribe result (ROUGH) */

int ai_Unk5944e0(int param_1, int rec)
{
    char text[0x100];
    int b;
    int i;

    if (rec == 0)
        return 0;
    b = he_FindEvent(1, 0, 0x62);
    while (b != 0) {
        if (*(int *)(b + 0xb0) == U_I(rec + 1))
            break;
        b = he_NextEvent();
    }
    if (b != 0) {
        txt_Format(text, 0x1a91, *(unsigned short *)(rec + 0x27), rec);
        sv_MessageBox(text, 0);
        return 0;
    }
    for (i = 0; i < 0x300; i++) {
        if (*(unsigned char *)((unsigned)&ai_Selected + i * 0x21c) != 0)
            break;
    }
    if (i >= 0x300) {
        sv_MessageBox(U_I((unsigned)&full_data_1440ba4), 0);
        return 0;
    }
    if (he_Unk553cc0(rec) == 0) {
        txt_Format(text, 0x1a89, *(unsigned short *)(rec + 0x27), rec);
        sv_MessageBox(text, 0);
        return 0;
    }
    /* ROUGH: the bribe result path (0x1a8a .. texts, the money of the character 0x91aaf0 / the record) */
    txt_Format(text, 0x1a8a, *(unsigned short *)(rec + 0x27), rec);
    sv_MessageBox(text, 0);
    return 0;
}
