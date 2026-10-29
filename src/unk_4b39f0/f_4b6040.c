/* 0x4b6040, the mission dialog with text 0x1b97 for the character ai_MissionPartner (another player's mission);
   quits (0x4b5950) when ai_GameOver */

void ai_MissionPartnerDialog(int id)
{
    AICHAR *c;
    int r, view, flags;

    r = 0;
    c = gm_FindChar(ai_MissionPartner);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b97, c->id, c->id);
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (ai_MissionPartner == -1) {
            r = (ai_GameOver != 0) + 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionPartnerDialog, flags));
    d2_CloseView(view);
    if (r == 2)
        ai_QuitGame(id);
}
