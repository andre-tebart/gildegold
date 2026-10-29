/* 0x4d8b50, shows the fight time (d3_Time minus the start 0x6f0da4, 14 ms per tick) as "mm : ss : ms ms" in a box at the top */

void ai_Unk4d8b50()
{
    char buf[128];

    d2_SetFont(0x54);
    sprintf(buf, "%2i : %2i : %3i ms", (d3_Time * 14 - ai_Unk6f0da4 * 14) / 60000, (d3_Time * 14 - ai_Unk6f0da4 * 14) / 1000 % 60,
            (d3_Time * 14 - ai_Unk6f0da4 * 14) % 1000);
    d2_DrawTextBox(0, 10, 200, 24, d2_ScreenSurf, buf, 0x30);
    d2_AddDirty(0, 10, d2_TextDrawnW, d2_TextDrawnH);
}
