/* 0x5290f0, opens the person info panel ("panel\\infopanel_personen") for the character p (0 for none or several selected: the text "%i %s" with the count 0x6f2334 and 0x144adb0): the name and age
   (0x514e90, "%s (%i)"), the health bar 0x623868 (0x50c520 of the character, -127 .. 127), the standing image for types 5 / 6 / 7 (+0x54), the two small buttons 0x5df / 0x5e0 (0x52eaa0) for kinds 10 .. 12, and the portrait
   (+0x18c, or 0x521 for none) with the character noted in its widget */

void sv_Unk5290f0(unsigned short *p)
{
    char text[512];
    int hp, kind, age, x;
    short w;
    char t;

    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_personen");
    d2_SetForm(sv_PanelForm, 1);
    if (p == 0 || 1 < ai_Unk6f2334) {
        crt_Sprintf(text, "%i %s", ai_Unk6f2334, ai_Unk144adb0);
        io_PrintText(text);
    } else {
        hp = gm_Opinion(ai_PlayerId, *p);
        kind = 0;
        if (*((char *)p + 0x165) != 0)
            kind = gm_Unk504c00(*((char *)p + 0x165));
        unk_Unk4fc8e0(p);
        age = (int)0;
        io_CharacterNameMarkup(*p, 1, 4, text);
        io_PrintText("%s (%i)", text, age);
        d2_SetForm(sv_PanelForm, 0);
        w = *(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4);
        ai_Unk623868 = d2_WinOpenSlider(0x18, 0x6c, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, -127, 0x7f, hp);
        t = *((char *)p + 2);
        if (t == 5 || t == 6 || t == 7)
            d2_WinOpenPic(((w - 0x30) >> 1) - 8, 0x30, *(int *)(p + 0x2a), d2_LastWindow);
        if (kind == 0xb || kind == 0xc || kind == 10) {
            d2_WinOpenPic(0x19, 0x7e, 0x5df, d2_LastWindow);
            sv_Unk52eaa0(0x29, 0x7e, *((char *)p + 0x82), 0x4f1);
            d2_WinOpenPic(0x19, 0x8c, 0x5e0, d2_LastWindow);
            sv_Unk52eaa0(0x29, 0x8c, *((char *)p + 0x83), 0x4f1);
        }
    }
    d2_SetForm(sv_PanelForm, 0);
    x = (*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1;
    if (p != 0) {
        ai_Unk623880 = d2_WinOpenPic(x, 0x38, *(int *)(p + 0xc6), d2_LastWindow);
        *(unsigned short **)(d2_Objs + 0x2e8 + ai_Unk623880 * 0x2ec) = p;
        *(int *)(d2_Objs + 0x4c + ai_Unk623880 * 0x2ec) = 1;
        return;
    }
    d2_WinOpenPic(x, 0x38, 0x521, d2_LastWindow);
}
