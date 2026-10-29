/* 0x520f20, resets the screen clip and the scene state 0x631090 and the window metrics 0x5f3238 / 0x5f323c (screen height - 8, width) */

void sv_ResetScreenClip(void)
{
    d2_SetPointerSprite(0, 0);
    d2_ClearPointerItems();
    di_ClearButtons();
    sv_SetStatusText(ch_EmptyName);
    d2_CopyCaption(ch_EmptyName);
    di_Absolute = 1;
    di_MinX = 0;
    di_MinY = 0;
    di_MaxX = d2_ScreenW - 8;
    di_MaxY = (int)d2_ScreenH;
}
