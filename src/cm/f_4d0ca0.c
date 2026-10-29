/* 0x4d0ca0, walks the objects of the scene with the callback 0x4d0a80 (the flag dummies) */

void ai_Unk4d0ca0()
{
    ai_Unk6f0d58 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x100, ai_Unk4d0a80, 0);
}
