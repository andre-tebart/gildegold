/* 0x588160, the drag window of a goods item (419 B, ROUGH: the calls and the text ids are written, the 0x1c-byte
   record at the frame start is assembled field by field from the globals 0x62a8f4 .. 0x62a908 (the words), the
   bytes 0x62a902 / 0x62a906 / 0x62a8fe and 0x631090, restored by the disassembly): idx = the building index
   (0x7e3f30 + idx * 0x24d). The window 0x62a8d8 with 0x475600, the section 1; the building kind (byte 0) not
   0xa: the text 0x27 (idx * 0xe + 0x48e, the record word at +0x10 + [bldg + 0x247] * 4), else 0x515960
   (idx * 0xe + 0x48e); the section 2, 0x4731d0 (0, 0, idx + 0x427, U_I (0x6ed938)) and the text
   idx * 0xe + 0x48f; not the kind 0xa: the section 3, the texts 0x2a (the record at +0x18 + [bldg + 0x247] *
   4), 0x28 (0x509dc0 (the player's character, idx)) and 0x29 (bldg + 0x243); the window */

int gm_Unk509dc0(int unused, int kind); char *_strupr(char *); int sprintf(char *buf, const char *fmt, ...);

int ai_Unk588160(int idx)
{
    char d[0x1c];
    char *bldg;
    int win;

    *(unsigned char *)(d + 0x00) = *(unsigned char *)(unsigned)&ch_EmptyName;
    *(unsigned short *)(d + 0x01) = 0;
    *(unsigned char *)(d + 0x03) = 0;
    *(unsigned short *)(d + 0x04) = *(unsigned short *)(unsigned)&full_data_62a908;
    *(unsigned short *)(d + 0x06) = 0;
    *(unsigned short *)(d + 0x08) = *(unsigned short *)(unsigned)&full_data_62a904;
    *(unsigned char *)(d + 0x0a) = *(unsigned char *)(unsigned)&full_data_62a906;
    *(unsigned char *)(d + 0x0b) = 0;
    *(int *)(d + 0x0c) = U_I((unsigned)&full_data_62a8f8);
    *(unsigned short *)(d + 0x10) = *(unsigned short *)(unsigned)&full_data_62a900;
    *(unsigned char *)(d + 0x12) = *(unsigned char *)(unsigned)&full_data_62a902;
    *(unsigned char *)(d + 0x13) = 0;
    *(unsigned short *)(d + 0x14) = *(unsigned short *)(unsigned)&full_data_62a8f4;
    *(unsigned short *)(d + 0x16) = 0;
    *(unsigned short *)(d + 0x18) = *(unsigned short *)(unsigned)&full_data_62a8fc;
    *(unsigned char *)(d + 0x1a) = *(unsigned char *)(unsigned)&full_data_62a8fe;
    *(unsigned char *)(d + 0x1b) = 0;
    bldg = (char *)U_I((unsigned)&ai_BTypes) + idx * 0x24d;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a8d8);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    if (*(unsigned char *)bldg != 0xa)
        io_PrintText(0x27, idx * 0xe + 0x48e, d + 0x10 + *(unsigned char *)(bldg + 0x247) * 4);
    else
        io_PrintText(idx * 0xe + 0x48e);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0x427), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 0xe + 0x48f);
    if (*(unsigned char *)bldg != 0xa) {
        d2_SetForm(0, 3);
        io_PrintText(0x2a, d + 0x18 + *(unsigned char *)(bldg + 0x247) * 4);
        io_PrintText(0x28, gm_Unk509dc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx));
        io_PrintText(0x29, *(int *)(bldg + 0x243));
    }
    return win;
}
