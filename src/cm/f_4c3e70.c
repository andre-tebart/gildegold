/* 0x4c3e70, starts the second field block about p (its id at +4) */

void ai_Unk4c3e70(char *p)
{
    ai_Unka68ca0.id = *(int *)(p + 4);
    ai_Unka68ca0.n = 0;
    memset(ai_Unka68ca0.buf, 0, sizeof(ai_Unka68ca0.buf));
    ai_Unka68d20 = 0;
    ai_Unka68d34 = p;
}
