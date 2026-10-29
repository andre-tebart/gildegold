/* 0x4dfff0, asks the player (0x4dfcb0 without the flag 0x6f0d7c, else the choice 0x4dfdd0) */

void ai_Unk4dfff0(char *m)
{
    if (ai_Unk6f0d7c == 0)
        ai_Unk4dfcb0(m);
    else
        ai_Unk4dfdd0(m);
}
