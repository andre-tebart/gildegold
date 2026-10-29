/* 0x4fdfb0, frees the icon names */

void gm_FreeIconNames(void)
{
    if (ai_Unk6f1b94 != 0) {
        mem_Free(ai_Unk6f1b94);
        ai_Unk6f1b94 = 0;
    }
}
