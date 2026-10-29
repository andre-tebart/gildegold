/* 0x4d8c40, frees the 16 slots of 0x92ca80 */

void ai_Unk4d8c40()
{
    int i;

    for (i = 0; i < 16; i++) {
        if (ai_Unk92ca80[i] != 0)
            ai_Unk4e3250(ai_Unk92ca80[i]);
    }
}
