/* 0x5b2090, the player's screen image (235 B): sprintf (path, 0x62cce8, 0xf8ef80, 0x7e3f40 + the player
   0x6f1bc5 * 0x2f4 (the names 0x7e3f40, 0x2f4 bytes per player)); the picture 0x478b50 (path) gives its
   width +4 and height +8; the 0x40-byte record at +0 is cleared and gets them at +4 / +8, then 0x478cc0 (&rec)
   resolves the picture 0x6f26ac, 0x479020 loads it and 0x478bd0 (0, 0, the width, the height, the +0x1c, 0, 0,
   the id) builds the control; the globals 0x6f4ee8 = the width, 0x6f4ee4 = the height; the scale 0x62c270 =
   7.5 (0x40f00000), 5.33 (0x40aa8f5c) below the width 0x780; 0x4790e0 / 0x478fc0 free the picture */

void ai_Unk5b2090(void)
{
    char path[0x104];
    int rec[0x10];
    int w;
    int h;
    int id;
    int bmp;

    sprintf(path, (char *)(unsigned)&full_data_62cce8, (char *)(unsigned)&ai_Unkf8ef80,
            (char *)((unsigned)&ai_PlayerSlots + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4));
    bmp = d2_LoadBmpSurface(path);
    if (bmp != 0) {
        w = *(int *)(bmp + 4);
        h = *(int *)(bmp + 8);
        memset(rec, 0, sizeof(rec));
        rec[1] = w;
        rec[2] = h;
        id = d2_MakeSurface(rec, 0xb);
        U_I((unsigned)&ai_Unk6f26ac) = id;
        d2_LockSurface(id);
        d2_ConvertRgbTo16(0, 0, w, h, *(int *)(bmp + 0x1c), 0, 0, U_I((unsigned)&ai_Unk6f26ac));
        U_I((unsigned)&ai_MapSize) = w;
        U_I((unsigned)&full_data_6f4ee4) = h;
        U_I((unsigned)&full_data_62c270) = 0x40f00000;
        if (w < 0x780)
            U_I((unsigned)&full_data_62c270) = 0x40aa8f5c;
        d2_UnlockSurface(U_I((unsigned)&ai_Unk6f26ac));
        d2_FreeSurface(bmp);
    }
}
