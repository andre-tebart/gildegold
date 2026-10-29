/* 0x4e3390, releases the scene's sound / object slot 0x6f0db8 (0x45d800 first when 0x6f0db4 is set) */

int ai_Unk4e3390(void)
{
    if (ai_Unk6f0d8c == 0) {
        if (ai_Unk6f0db4 != 0)
            d3sky_DeleteLayer(ai_Unk6f0db8, ai_Unk6f0db4);
        d3sky_Destroy(ai_Unk6f0db8);
        ai_Unk633dd4 = 0;
    }
}
