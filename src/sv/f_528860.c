/* 0x528860, opens the building info panel ("panel\\infopanel_gebaeude", 0x623860) for the object o: the title text ("$Z%s$A%s", with the owner name "$A>%s<" when o+5 is set; kind 0x47 uses the player's name 0x870),
   the building image (0x427 + kind), the health bar 0x623868 (0x46a960) except for the kinds 0x47 / 0x1d / 0x57, and for owned buildings the buttons (0x528800: 0x8c at 0x623894, "Transport" 0xa6 at 0x62388c for
   kinds that carry goods). ROUGH */

void sv_Unk528860(unsigned char *o)
{
    int hp, k, v;
    char text[128];
    char *b;
    unsigned short owner;
    unsigned fl;
    char kind;

    hp = gm_BuildingLevel(o);
    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
    d2_SetForm(sv_PanelForm, 1);
    if (*o == 0x47) {
        io_PrintText("$Z%s$A%s", 0x870, (char *)(unsigned)&ai_PlayerSlots + *(int *)(o + 0x65) * 0x2f4);
    } else {
        io_CharacterNameMarkup(*(unsigned short *)(o + 0x27), 2, 1, text);
        if (o[5] != 0)
            io_PrintText("$Z%s$A%s$A>%s<", text, *o * 0xe + 0x48e, o + 5);
        else
            io_PrintText("$Z%s$A%s", text, *o * 0xe + 0x48e);
    }
    d2_SetForm(sv_PanelForm, 0);
    ai_Unk623864 = d2_WinOpenPic((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1, 0x48, *o + 0x427, d2_LastWindow);
    *(int *)(d2_Objs + 0x4c + ai_Unk623864 * 0x2ec) = 1;
    owner = *(unsigned short *)(o + 0x27);
    if (owner != 0xffff && owner != *(unsigned short *)ai_PlayerChar)
        *(char **)(d2_Objs + 0x2e8 + ai_Unk623864 * 0x2ec) = ai_Chars[owner];
    kind = *o;
    if (kind == 0x47 || kind == 0x1d || kind == 0x57)
        ai_Unk623868 = -1;
    else {
        ai_Unk623868 = d2_WinOpenSlider(0x15, 0x7b, 0x38, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, 0, 100, hp);
    }
    if ((o[0x5a] & 1) == 0 && *o != 0x47 && *o != 0x1d && *o != 0x57) {
        k = gm_BuildingClass(*o);
        b = ai_BTypes + *o * 0x24d;
        fl = unk_Unk503530(ai_PlayerId, o, 0, 0);
        v = ai_Unk1441f6c;
        if (*(short *)(o + 0x27) != (short)ai_PlayerId && *(short *)(o + 0x25) != (short)ai_PlayerId && (fl & 0x10) == 0)
            v = ai_Unk1441f74;
        ai_Unk623894 = sv_Unk528800(0, 0x8c, v);
        if ((k == 1 || k == 2 || k == 4 || k == 7 || k == 8 || *b == 0x10 || *b == 0x1e || *b == 0x1f || *b == 0x1d) && *b != 5) {
            if (ai_FallbackBuilding == 0)
                ai_Unk62388c = sv_Unk528800(0, 0xa6, (int)"Transport");
            ai_Unk62386c = -1;
            return;
        }
        ai_Unk62386c = -1;
        ai_Unk62388c = -1;
        return;
    }
    ai_Unk623894 = -1;
    ai_Unk62388c = -1;
    ai_Unk62386c = -1;
}
