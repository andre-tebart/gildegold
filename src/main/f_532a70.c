/* 0x532a70, joins a saved network game: asks for the game (0x5d42b0), opens "gamedata/network/<name>.SAV" (0x430470, "rb"), reads its header (0x51cee0) and fills the game record 0x75e6e0 / 0x936470 (version >=
   0x10054: the second name and the extra fields) for the start; returns 1 on success */

int main_Unk532a70(void)
{
    int f, i;
    int hdr[0x80];
    char path[260];
    char c;
    int *src, *dst;

    if (ai_JoinGameWindow() != 0) {
        sprintf(path, "gamedata/network/%s.SAV", main_NetSaveName);
        f = vfs_Tfopen(path, (char *)(unsigned)"rb", U_I((unsigned)&d3_Unk6333fc));
        if (f != 0) {
            ls_ReadSaveHeader(f, hdr);
            strcpy(main_NetHost, "127.0.0.1");
            U_I((unsigned)&ai_GameFlags) |= 0x50;
            *(unsigned char *)(unsigned)&ai_NumPlayers = *((unsigned char *)hdr + 0x31);
            *(unsigned char *)((unsigned)&ls_SavePath + 0xc6) = *((unsigned char *)hdr + 0x31);
            *(unsigned char *)((unsigned)&ls_SavePath + 0xa4) = 0x6a;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xa0) = 1;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xa2) = 4;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xc8) = *(unsigned short *)((unsigned)&ls_SavePath + 0x84);
            i = 0;
            do {
                c = *((char *)hdr + 5 + i);
                *(char *)(((unsigned)&ls_SavePath + 0xa6) + i) = c;
                i++;
            } while (c != 0);
            i = 0;
            do {
                c = main_NetSaveName[i];
                *(char *)(((unsigned)&ls_SavePath + 0xca) + i) = c;
                i++;
            } while (c != 0);
            src = (int *)((char *)hdr + 0x78);
            dst = (int *)((unsigned)&ls_SavePath + 0xea);
            for (i = 8; i != 0; i--)
                *dst++ = *src++;
            src = (int *)((unsigned)&ls_SavePath + 0xa0);
            dst = (int *)&cm_NetGame.hdr;
            for (i = 0x1a; i != 0; i--)
                *dst++ = *src++;
            *(short *)dst = *(short *)src;
            dst = (int *)((unsigned)&ai_Unk9364e0 + 0x2c0);
            for (i = 0xc; i != 0; i--)
                *dst++ = 0;
            U_I(((unsigned)&ai_Unk9364e0 + 0x2e0)) = *((unsigned *)hdr + 0x60 / 4) & 0xff;
            U_I(((unsigned)&ai_Unk9364e0 + 0x2e4)) = *((unsigned *)hdr + 0x6c / 4) & 0xff;
            cm_NetGame.mode = 0x7f;
            i = 0;
            do {
                c = *((char *)hdr + 0x40 + i);
                *(char *)(((unsigned)&ai_Unk9364e0 + 0x2c0) + i) = c;
                i++;
            } while (c != 0);
            if (0x10053 < hdr[0]) {
                i = 0;
                do {
                    c = *((char *)hdr + 0x50 + i);
                    *(char *)(((unsigned)&ai_Unk9364e0 + 0x2d0) + i) = c;
                    i++;
                } while (c != 0);
                U_I(((unsigned)&ai_Unk9364e0 + 0x2e8)) = *((unsigned *)hdr + 0x60 / 4) >> 0x18;
                U_I(((unsigned)&ai_Unk9364e0 + 0x2ec)) = *((int *)hdr + 0x64 / 4);
            }
            vfs_fclose(f);
            return 1;
        }
    }
    return 0;
}
