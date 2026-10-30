/* 0x4e3650, sets the object tables 0x5fb5b0 up with the two 0xc0 byte callbacks 0x512de0 / 0x512e40 (0x438e60) after 0x4fe350, then 0x5128d0 */

void ai_Unk4e3650(void)
{
    gm_ClockSeason(&ai_Time);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, init_PlantTextureSetHook);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, init_TextureSetHook);
    init_SetSeasonTextures();
}
