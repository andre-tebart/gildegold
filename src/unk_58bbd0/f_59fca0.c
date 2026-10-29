/* 0x59fca0, the town cash office listener (144 B): with the player's byte 0x770ce6 + player * 0x21c set, the
   pump 0x5309a0 (0x59fca0, 0x67fff) looks up "ob_STADTKASSE" (0x14409f8, class 0x16) and opens 0x59fbf0 */

void ai_Unk59fca0(int param_1)
{
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk59fca0, 0x67fff) != 0) {
        if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
            c = sv_RegisterNamePlate("ob_STADTKASSE", U_I((unsigned)&full_data_14409f8), 0x16);
        else
            c = 0;
        if (U_I((unsigned)&ai_ClickedObject) != 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk59fbf0(param_1);
    }
}
