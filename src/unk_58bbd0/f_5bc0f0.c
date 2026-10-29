/* 0x5bc0f0, the release of the picture 0x6f26ac (32 B): 0x478fc0 frees it, the id is reset */

void ai_Unk5bc0f0(void)
{
    if (U_I((unsigned)&ai_Unk6f26ac) != 0)
        d2_FreeSurface(U_I((unsigned)&ai_Unk6f26ac));
    U_I((unsigned)&ai_Unk6f26ac) = 0;
}
