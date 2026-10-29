/* 0x525440, closes the player bar (d2_CloseView, sv_PlayerBarForm), shows the info panel (sv_PanelForm) again and
   refreshes it (sv_UpdatePanel) */

void sv_ClosePlayerBar(void)
{
    d2_CloseView(sv_PlayerBarForm);
    sv_PlayerBarForm = -1;
    d2_ClearPointerItems();
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 1);
    sv_UpdatePanel();
}
