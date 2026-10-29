/* 0x529fb0, closes the info panel (0x623860): frees the widgets 0x62386c .. (4 ids), destroys the form and clears all the panel's widget ids (0x623864 .. 0x623898) */

void sv_Unk529fb0(void)
{
    int *q;

    if (sv_PanelForm != -1) {
        q = &ai_Unk62386c;
        do {
            if (*q != -1) {
                d2_RemoveObj(*q);
                *q = -1;
            }
            q++;
        } while ((int)q < (int)((unsigned)&ai_Unk62386c + 0x10));
        d2_CloseView(sv_PanelForm);
        sv_PanelForm = -1;
        ai_Unk623864 = -1;
        ai_Unk623868 = -1;
        ai_Unk623898 = -1;
        ai_Unk623894 = -1;
        ai_Unk62388c = -1;
        ai_Unk623890 = -1;
        ai_Unk623884 = -1;
        ai_Unk623888 = -1;
        ai_Unk623880 = -1;
        ai_Unk62387c = -1;
    }
}
