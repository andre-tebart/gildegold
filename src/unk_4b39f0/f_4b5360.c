/* 0x4b5360, the mission dialog of type type, information only (cf. 0x4b3d50) */

void ai_MissionInfoDialog(int type, int id)
{
    char name[0x100];
    G13TYPE *t;
    int view;
    void *snd;

    t = ai_GetMissionType(type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)(t->text + 1));
    io_PrintText("$N");
    io_PrintText((char *)0x7d);
    sprintf(name, "_AUFTRAEGE_VERGABE_HS_%.2d", t->unk0c);
    snd = snd_PlayVoice(-7, 0, name, -1);
    while (sv_RunFrame((void (*)(void))ai_MissionInfoDialog, id)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    if (view != -1)
        d2_CloseView(view);
    if (sb_IsPlaying(snd))
        sb_Stop(snd, 0x19);
}
