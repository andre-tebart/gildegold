/* 0x4d6b70, closes the fight UI: the view 0x6f0d64, 0x4d08c0, the portrait boxes */

void ai_Unk4d6b70()
{
    d2_CloseView(ai_Unk6f0d64);
    ai_Unk6f0d64 = 0;
    ai_Unk4d08c0();
    ai_Unk4cfe70();
}
