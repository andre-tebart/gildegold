/* 0x5b4590, a window over the player's character record (641 B, ROUGH SKELETON): the record 0x770b80 +
   0x6f25b8 * 0x21c, the window "0x62cda8" (0x4741f0 / 0x475600 / 0x475f70), a 0x8c8-byte frame; the body
   (the controls, the texts and the pump loop) is left as a comment */

int ai_Unk5b4590(void)
{
    char *per;
    int win;

    per = (char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    win = d2_OpenForm((char *)(unsigned)&full_data_62cda8, 0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    /* ROUGH SKELETON: the controls of the record, the texts 0x5176d0 and the pump loop 0x5309a0 */
    (void)per;
    return 0;
}
