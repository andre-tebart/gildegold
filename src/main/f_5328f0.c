/* 0x5328f0, hosts a network game: the player count dialog (0x5327e0), the player name / hero choice (0x537230 (1, 1)), then the game record 0x75e6e0 (26 ints) is filled (the city file "<gamedata/cities>/<name>.net",
   the player count 0x62489c, the host name, the flag 0x10 of 0x6f23f8) and copied to 0x936470 for the start (0x43b950 with the screen rectangle); returns 1, or 0 when cancelled */

extern int ai_Unk5fe710, d2_LastWindow;

/* ---- the network game dialogs (ROUGH) ---- */

int main_Unk5328f0(void)
{
    int i;
    int *src, *dst;
    char c;

    if (main_Unk5327e0() != 0) {
        d3io_NoopCallback();
        if (main_Unk537230(1, 1) != 0) {
            sprintf((char *)(unsigned)&full_data_75e360, "%s/%s.net", "gamedata/cities", (char *)((unsigned)&ai_DataPath + 0x220));
            U_I((unsigned)&full_data_75e1c0) = U_I((unsigned)&full_data_624b48);
            U_I((unsigned)&full_data_75e1c4) = U_I((unsigned)&full_data_624b4c);
            U_I((unsigned)&ai_GameFlags) |= 0x10;
            *(unsigned char *)((unsigned)&ls_SavePath + 0xc6) = *(unsigned char *)(unsigned)&ai_NumPlayers;
            U_I((unsigned)&full_data_75e1c8) = U_I((unsigned)&full_data_624b50);
            *(unsigned char *)((unsigned)&ls_SavePath + 0xa4) = 0x6a;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xa0) = 1;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xa2) = 1;
            *(unsigned short *)((unsigned)&ls_SavePath + 0xc8) = *(unsigned short *)((unsigned)&ls_SavePath + 0x84);
            i = 0;
            do {
                c = *(char *)(((unsigned)&ai_DataPath + 0x220) + i);
                *(char *)(((unsigned)&ls_SavePath + 0xa6) + i) = c;
                i++;
            } while (c != 0);
            src = (int *)((unsigned)&ls_SavePath + 0xa0);
            dst = (int *)(unsigned)&full_data_936470;
            for (i = 0x1a; i != 0; i--)
                *dst++ = *src++;
            *(short *)dst = *(short *)src;
            U_I((unsigned)&ai_Unk9364dc) = 0x3f;
            d3_SetViewport((float)(U_I((unsigned)&sv_ViewRectRight) - U_I((unsigned)&sv_ViewRectLeft)), (float)(U_I((unsigned)&sv_ViewRectBottom) - U_I((unsigned)&sv_ViewRectTop)), (float)U_I((unsigned)&sv_ViewRectLeft), (float)U_I((unsigned)&sv_ViewRectTop), *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH);
            return 1;
        }
    }
    d2_ClearSurface(d2_ScreenSurf);
    d2_ShowPicture(0x797);
    return 0;
}
