/* 0x5328f0, hosts a network game: the player count dialog (main_PlayerCountDialog), the player name / city choice (main_ChooseCity (1, 1)), then the network game record in ls_SavePath (106 bytes at
   +0xa0: the record size 0x6a, two words of 1, the city name from ai_DataPath + 0x220, the player count, the word at +0x84) is filled, the save name "<gamedata/cities>/<city>.net" and the host address
   127.0.0.1 are set, the network flag 0x10 of ai_GameFlags is raised and the record is copied to main_NetGameRecord; the view is set up (d3_SetViewport) and the mode mask 0x3f stored; returns 1,
   or 0 when cancelled (the screen is cleared and picture 0x797 shown) */

extern int ai_Unk5fe710, d2_LastWindow;

int main_HostNetworkGame(void)
{
    int i;
    char c;

    if (main_PlayerCountDialog() != 0) {
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
        if (main_ChooseCity(1, 1) != 0) {
            sprintf(main_NetSaveName, "%s/%s.net", "gamedata/cities", ai_DataPath + 0x220);
            strcpy(main_NetHost, "127.0.0.1");
            ai_GameFlags |= 0x10;
            ls_SavePath.net.players = ai_NumPlayers;
            ls_SavePath.net.size = 0x6a;
            ls_SavePath.net.a = 1;
            ls_SavePath.net.b = 1;
            ls_SavePath.net.x = ls_SavePath.cur;
            i = 0;
            do {
                c = ai_DataPath[0x220 + i];
                ls_SavePath.net.name[i] = c;
                i++;
            } while (c != 0);
            memcpy(main_NetGameRecord, &ls_SavePath.net, sizeof(struct NETHDR));
            ai_ModeMask = 0x3f;
            d3_SetViewport((float)(sv_ViewRectRight - sv_ViewRectLeft), (float)(sv_ViewRectBottom - sv_ViewRectTop), (float)sv_ViewRectLeft, (float)sv_ViewRectTop, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
            return 1;
        }
    }
    d2_ClearSurface(d2_ScreenSurf);
    d2_ShowPicture(0x797);
    return 0;
}
