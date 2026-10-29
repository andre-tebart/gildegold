/* 0x577a00, the tutorial's escape check: -4 without a player / run record; 0 while the scene 0x6f262c is not a
   record of the type byte 0x87, while the window / object is not the one in S+0x20 (or, with 0xf8a668 == -1, while
   the last key is not 0x1c); else the window S+0x14 is released (0x426f20 / 0x427290) and 1 answers */

int ai_Unk577a00(void)
{
    char *st;

    st = S;
    if (!*(int *)(st + 0x1c))
        return -4;
    if (!*(int *)(*(char **)(st + 0x1c) + 0x74))
        return -4;
    if (!U_I((unsigned)&full_data_6f262c))
        return 0;
    if (!*(char **)U_I((unsigned)&full_data_6f262c) || *(unsigned char *)*(char **)U_I((unsigned)&full_data_6f262c) != 0x87)
        return 0;
#ifdef REA_START
    /* REA_START=tutorial (`uv run play|dbg --start tutorial`): accept the tutorial's intro message (the 0x87
       event, whose "Weiter" text object 0x577740 stores in S+0x20) without a mouse click, so a run without GUI
       automation gets past it. Jump straight to the answer like the keyboard arm does - forging a click in
       d2_ClickedId / d2_PointerObj would leak the phantom click into the rest of the frame. Without the flag
       the message waits for the real click / Return. */
    {
        char *rea = getenv("REA_START");
        if (rea != 0 && strcmp(rea, "tutorial") == 0)
            goto answered;
    }
#endif
    if (U_I((unsigned)&d2_ClickedId) != -1) {
        if (U_I((unsigned)&ai_Unk5fe710) != *(int *)(st + 0x20))
            return 0;
    } else {
        if (*(unsigned char *)(unsigned)&kb_Last != 0x1c)
            return 0;
    }
#ifdef REA_START
answered:
#endif
    if (U_I(st + 0x14)) {
        if (sb_IsPlaying(U_I(st + 0x14)))
            sb_Stop(U_I(st + 0x14), 0);
        U_I(st + 0x14) = 0;
    }
    return 1;
}
