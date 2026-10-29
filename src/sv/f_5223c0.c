/* 0x5223c0, destroys the screen: the widgets of the table 0x7662e0, the groundplan views 0x6f22c4 / 0x6f22c8 (0x478fc0), the background 0x6f22c0, the button 0x6238a4, the strips 0x623850 / 0x623840, the panel
   widgets (0x522280), 0x529fb0, and sets the flag 0x6237cc */

void sv_DestroyScreen(void)
{
    int *q;

    q = ai_Unk7662e0;
    do {
        if (*q != -1) {
            d2_RemoveObj(*q);
            *q = -1;
        }
        q++;
    } while ((int)q < (int)((unsigned)&ai_Unk7662e0 + 0x3c));
    if (ai_Unk6f22c4 != 0) {
        d2_FreeSurface(ai_Unk6f22c4);
        ai_Unk6f22c4 = 0;
    }
    if (ai_Unk6f22c8 != 0) {
        d2_FreeSurface(ai_Unk6f22c8);
        ai_Unk6f22c8 = 0;
    }
    if (ai_Unk6f22c0 != 0) {
        mem_Free(ai_Unk6f22c0);
        ai_Unk6f22c0 = 0;
    }
    if (ai_Unk6238a4 != -1) {
        d2_RemoveObj(ai_Unk6238a4);
        ai_Unk6238a4 = -1;
    }
    if (ai_Unk623850 != -1)
        d2_RemoveHitRect(ai_Unk623850);
    sv_Unk522280();
    sv_PanelPic = -1;
    if (ai_Unk623840 != -1) {
        d2_RemoveWindow(ai_Unk623840);
        ai_Unk623840 = -1;
    }
    sv_Unk529fb0();
    ai_Unk6237cc = 1;
}
