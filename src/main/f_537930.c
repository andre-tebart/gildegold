/* 0x537930, starts a new single player game: the player count 1 (0x62489c) and the player setup 0x537230 (1, 0); returns 1 when done, else the start screen is rebuilt */

int main_Unk537930(void)
{
    int r;

    U_I((unsigned)&ai_GameFlags) = 0;
    d2_SetPointerSprite(0, 0);
    *(unsigned char *)(unsigned)&ai_NumPlayers = 1;
    r = main_ChooseCity(1, 0);
    if (r == 0) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_ShowPicture(0x797);
    }
    return r;
}
