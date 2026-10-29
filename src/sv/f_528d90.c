/* 0x528d90, opens the info panel for the transporter p ("panel\\infopanel_transporter"): the title ("$Z%2N1$A%s": the owner's id and the name 0x9a8 + 2 * id), the image 0xdf + id, for the own transporter the button 0xac
   (0x623890), the load bar 0x623868 (+0x12 byte) and notes the panel's form 0x62387c and the position record 0x631078 */

void sv_Unk528d90(unsigned short *p)
{
    short *owner;
    int x, w;

    if (sv_PanelForm != -1)
        return;
    owner = (short *)gm_Unk50a050(p);
    if (owner == 0)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_transporter");
    d2_SetForm(sv_PanelForm, 1);
    io_PrintText("$Z%2N1$A%s", *owner, *p * 2 + 0x9a8);
    d2_SetForm(sv_PanelForm, 0);
    x = ((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1;
    w = d2_WinOpenPic(x, 0x30, *p + 0xdf, d2_LastWindow);
    *(int *)(d2_Objs + 0x4c + w * 0x2ec) = 1;
    if (*owner == (short)ai_PlayerId)
        ai_Unk623890 = sv_Unk528800(x, 0xac, ai_Unk144ad24);
    ai_Unk623868 = d2_WinOpenSlider(0x18, 0x66, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
    d2_SetObjValue(ai_Unk623868, 0, 100, (char)p[9]);
    d2_SetForm(sv_PanelForm, 2);
    ai_Unk62387c = d2_LastWindow;
    ai_Unk631078 = *(int *)((char *)p + 0x3b);
}
