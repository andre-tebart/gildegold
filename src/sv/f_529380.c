/* 0x529380, the per-frame refresh of the info panel 0x623860 (the progress bar +0x1e4 of the widget 0x623864, the owner's tools row 0x62386c ..): for a selected building (0x75ec60) the title and the progress of its
   current job (the object lists 0x53e400 (2, 0, kind 0x1c / 0x1d / 0x1e / 0x1f / 2, 3, owner) the elapsed fraction by 0x4fe280 of the start +0x44 against the end), without one the selected character 0x7662c0 (or
   the transporter 0x75ec5c) is shown: its name, the images of 0x524900, and the progress of its action by the action byte (+0x2f8: 0x8f / 0x16 timers, 0x40 (state +0x70), 0x61, 0x43, 0x62, 0x65, 0x48, 0x6c). ROUGH */

void sv_Unk529380(void)
{
    int i;
    int *q;
    float prog;
    unsigned short *tr;
    char *b;
    char *job;
    int n, a, d;
    char text[256];
    unsigned short fl;

    prog = 0.0f;
    if (sv_PanelForm == -1)
        return;
    b = (char *)ai_Unk75ec60I;
    if (b == 0 || ai_Unk623868 == -1 || ai_Unk623864 == -1) {
        if (ai_Unk7662c0 == 0) {
            q = &ai_Unk62386c;
            for (i = 0; i < 4; i++, q++)
                if (*q != -1) {
                    d2_RemoveObj(*q);
                    *q = -1;
                }
            if (ai_Unk62387c == -1 || ai_Unk75ec5c == 0)
                return;
            tr = ai_Unk75ec5c;
            if (ai_Goods[*tr * 0x41] != 0x1d)
                return;
            if (ai_Unk623890 != -1 && d2_ClickedId != -1 && ai_Unk5fe710 == ai_Unk623890 && (sv_FrameMaskU & 0x2000) != 0) {
                rt_Unk4bf5b0(tr);
                tr = ai_Unk75ec5c;
            }
            if (ai_Unk623868 != -1)
                d2_SetObjValue(ai_Unk623868, 0, 100, (char)tr[9]);
            d2_SetForm(sv_PanelForm, 2);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            return;
        }
        if (ai_Unk6f2334 < 2 && ai_Unk7662c0 != 0) {
            d2_SetForm(sv_PanelForm, 1);
            unk_Unk4fc8e0(ai_Unk7662c0);
            io_CharacterNameMarkup(*(unsigned short *)ai_Unk7662c0, 1, 4, text);
            io_PrintText("$C%s (%i)", text, 0);
        }
        return;
    }
    n = gm_BuildingLevel(b);
    d2_SetForm(sv_PanelForm, 1);
    io_PrintText("$Z%s$A%s", (unsigned char)*b * 0xe + 0x48e, "");
    d2_SetObjValue(ai_Unk623868, 0, 100, n);
    fl = *(unsigned short *)(b + 0x5a);
    if ((fl & 4) == 0) {
        if ((fl & 1) != 0) {
            job = (char *)he_FindEvent(2, 0, 0xd, 3, *(int *)(b + 1));
            prog = 0.05f;
            if (job != 0) {
                if (3 < *(int *)(job + 0x70))
                    prog = (float)*(int *)(job + 200) / (float)*(int *)(job + 0xc4);
                if (prog < 0.01f)
                    prog = 0.01f;
            }
        } else if ((fl & 0x40) != 0) {
            job = (char *)he_FindEvent(2, 0, 0x1e, 3, *(int *)(b + 1));
            if (job == 0)
                return;
            a = gm_MinutesBetween(job + 0x44, job + 0x52);
            d = gm_MinutesBetween(job + 0x44, ai_Time);
            prog = (float)d / (float)a;
        } else if (!(fl & 0x80)) {
            if (ai_Unk62386c == -1)
                return;
            d2_RemoveObj(ai_Unk62386c);
            ai_Unk62386c = -1;
            return;
        } else {
            job = (char *)he_FindEvent(2, 0, 0x1f, 3, *(int *)(b + 1));
            if (job == 0)
                return;
            a = gm_MinutesBetween(job + 0x44, job + 0x52);
            d = gm_MinutesBetween(job + 0x44, ai_Time);
            prog = (float)d / (float)a;
        }
    } else {
        job = (char *)he_FindEvent(2, 0, 0x1c, 3, *(int *)(b + 1));
        if (job == 0)
            return;
        a = gm_MinutesBetween(job + 0x44, job + 0xb8);
        d = gm_MinutesBetween(job + 0x44, ai_Time);
        prog = (float)d / (float)a;
    }
    *(float *)(d2_Objs + 0x1e4 + ai_Unk623864 * 0x2ec) = prog;
    if (ai_Unk62386c == -1 && (fl & 1) != 0) {
        d2_SetForm(sv_PanelForm, 0);
        ai_Unk62386c = d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
    }
}
