/* 0x4f9e30, the statistics screen: opens the form "misc\statistics_taxes", prints one line per round
   ("Runde %i:   %T$N": the game time - 0x10 + i and the round's value of the income history, 17 of them),
   then waits in the sv_RunFrame loop until a mouse event pops the request */

void gm_StatisticsScreen(void)
{
    int f;
    int i;

    f = d2_OpenForm(0, 0, "misc\\statistics_taxes");
    d2_CentreViewWindows(f);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(f, 0);
    for (i = 0; i < 0x11; i++)
        io_PrintText("Runde %i:   %T$N", ai_TimeYear - 0x10 + i, ((int *)gm_IncomeHistory)[i]);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(gm_StatisticsScreen, 0x677c7) != 0);
    d2_CloseView(f);
}
