/* 0x5d7e90, the pause loop (120 B): sets tb_Paused, shows the "Pause" text (sv_SetStatusText) and
   clears ai_GameState; the pump sv_RunFrame (frame flags sv_FrameMask & ~0x2000 | 0x100000) runs
   while it returns nonzero and the last key (kb_Last) is not '9'. Then the status text is reset
   (ch_EmptyName), tb_Paused cleared and ai_GameState set. */

void ai_PauseGame(void)
{
    int flags;
    int r;

    flags = sv_FrameMask & ~0x2000 | 0x100000;
    tb_Paused = 1;
    sv_SetStatusText("Pause");
    ai_GameState = 0;
    r = sv_RunFrame(0, flags);
    while (r != 0 && kb_Last != '9')
        r = sv_RunFrame(0, flags);
    sv_SetStatusText(ch_EmptyName);
    tb_Paused = 0;
    ai_GameState = 1;
    return 0;
}
