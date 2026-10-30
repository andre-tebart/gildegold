/* 0x521e70, builds the main screen layout for the scene s: clears the clip, creates the bar widgets 0x623850 / 0x623840 (a 0x288 x 0x30 strip), redraws the background bitmap (0x6ed92c, the rectangle 0x6248a4 ..
   0x6248b0) and sets up the groundplan view 0x6f22c8 (0x478cc0 with a table of 0x10 words: 0x68 / 0x6e) and the button 0x6238a4 (0x46cd80 (0x286, 0x23c, 0x544)). ROUGH */

void sv_BuildScreenLayout(int s)
{
    int t, i;
    int tab[16];
    int *q;

    d2_SetClipRect(0, 0, (int)d2_ScreenW, (int)d2_ScreenH);
    ai_Unk623850 = d2_AddHitRect(d2_ScreenW - 0x50, 0x11, 0x30, 0x30, 0);
    ai_Unk623840 = d2_CreateWindow(4, 10, 0x288, 0x30, 0);
    d2_PlaceWindowScaled(ai_Unk623840, 1);
    if (ai_Unk6f22c0 != 0) {
        mem_Free(ai_Unk6f22c0);
        ai_Unk6f22c0 = 0;
    }
    t = d2_GetPic(s);
    if (d2_LockSurface(ai_Unk6ed92c) != 0) {
        d2_ClearSurface(ai_Unk6ed92c);
        d2_DrawShape(0, 0, ai_Unk6ed92c, t, 0);
        ai_Unk6f0454[0] = 0;
        ai_Unk6f0454[1] = 0;
        ai_Unk6f0454[2] = 0;
        ai_Unk6f22c0 = d2_GrabShape(sv_ViewRectLeft, sv_ViewRectTop, (sv_ViewRectRight - sv_ViewRectLeft) + 1, sv_ViewRectBottom - sv_ViewRectTop, ai_Unk6ed92c, 0, 1);
        d2_ClearSurface(ai_Unk6ed92c);
        d2_UnlockSurface(ai_Unk6ed92c);
    }
    ai_Unkfb9068 = sv_ViewRectRight;
    q = ai_Unk7662e0;
    for (i = 0xf; i != 0; i--)
        *q++ = -1;
    ai_Unkfb9060 = sv_ViewRectLeft;
    ai_Unkfb9064 = sv_ViewRectTop;
    q = tab;
    for (i = 0x10; i != 0; i--)
        *q++ = 0;
    sv_PanelPic = s;
    ai_Unkfb906c = sv_ViewRectBottom;
    tab[1] = 0x68;
    tab[2] = 0x6e;
    ai_Unk6f22c8 = d2_MakeSurface(tab, 3);
    d2_ClearSurface(ai_Unk6f22c8);
    ai_Unk6238a4 = d2_CreatePicObj(0x286, 0x23c, 0x544);
    *(int *)(d2_Objs + 0x4c + ai_Unk6238a4 * 0x2ec) = 1;
    d2_EnableObject(ai_Unk6238a4, 0);
}
