/* 0x555350, the run handler of a transport event (a Ghidra miss; ROUGH): +0xb0 the source building, +0xb4 the target building, +0xac the transport id (0x4fe980 gives the transporter object in the scene 0x633444), +0xd0 started flag, +0xd4 a tick counter, +0xd8 .. the last position, +0xec the guide handle, +0xf2 silent flag. First round: the transporter is sent (0x4c5230, 0x4c52e0 "Trans", the guide 0x4c4920, +0xd0 = 1); later rounds check whether it arrived (the position change 0x453e80 against 100.0, or more than 6 rounds): then the guide is dropped (0x4c5260), the transporter gets the message 0x13 / 0x40, the news (0x1c43 / 0x1c44 / 0x1c39 / 0x1c41, by the building kind 0x1d / 0x1f and the 0x5024f0 flag) is formatted and shown to a player (0x53dee0 .., the text 0x608) unless the world map is in a mode (0x6f229c, 0x6f2310 ranges) and the event is posted again with the handle 3 (0x4c4c60 (3, ev), +0x84) */

int gm_FindObjectInst(int id);
void ai_Unk4c4920(int a, int b, int c);
void ai_Unk4c5260(int a, int b);
int gm_IsType10(unsigned char *b);
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);

void he_Unk555350(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int b0, b1, tr, kind, e, f;
    int a44, a45;
    int o, q;
    char text[0x400];
    int kd;

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    kd = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b0 * 0x24d;
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    tr = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a44;
    (void)a45;
    if (tr != 0) {
        he_Unk5552c0(ev, tr);
        if (*(unsigned char *)(ev + 0xd0) == 0) {
            ai_Unk4c5230(*(int *)(tr + 2));
            ai_Unk4c52e0(*(int *)(tr + 2), *(int *)(b1 + 1), -1, 0, 1, "Trans");
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(tr + 2));
            *(unsigned char *)(ev + 0xd0) = 1;
        } else if (*(int *)(tr + 0x3b) != 0 && *(int *)(*(int *)(tr + 0x3b) + 0x128) == 0) {
            o = *(int *)(tr + 0x3b);
            if ((*(int *)(o + 0x88) == (unsigned)&d3_Scenes && d3_VecEqual(*(int *)(o + 0x34) + 0x4c, ev + 0xd8, 100.0f) != 0) || *(int *)(ev + 0xd4) > 6) {
                if (gm_IsType10(b1) == 0) {
                    *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(b1 + 1), *(int *)(a45 + 1), *(int *)(tr + 2));
                    ai_Unk4c5260(*(int *)(tr + 2), 0);
                }
                cm_SendSetField(*(int *)(tr + 2), 0x13, 1, 0, 0x40);
                kind = *(unsigned char *)kd;
                if (gm_IsType10(b1) != 0)
                    txt_Format(text, 0x1c43);
                else if (kind != 0x1d && kind != 0x1f)
                    txt_Format(text, 0x1c41, 0x1c39, b0, *(unsigned short *)(b1 + 0x27), b1);
                else if (gm_IsType10(b1) != 0)
                    txt_Format(text, 0x1c43);
                else
                    txt_Format(text, 0x1c44, b1);
                e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
                if (e == 6 || e == 7) {
                    f = 0;
                    if (U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow) {
                        if (U_I((unsigned)&ai_PlayerBuilding) == b1)
                            f = 1;
                        else if (gm_IsType10(b1) != 0 && U_I((unsigned)&sv_PanelShown) != 0) {
                            int w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);

                            if ((w >= 0x92 && w <= 0x97) || w == 0x2db)
                                f = 1;
                        }
                    }
                    if (f == 0 && (*(unsigned char *)(tr + 0x20) & 1) == 0 && *(unsigned char *)(ev + 0xf2) == 0) {
                        q = gm_BuildingMainObject(b1);
                        if (q != 0)
                            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(tr + 2), text, 0, 0x608, *(int *)(b1 + 1), *(int *)(q + 2), 0, 0);
                        else
                            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(tr + 2), text, 0, 0x608, *(int *)(b1 + 1), -1, 0, 0);
                    }
                }
                *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            }
        }
    }
    d3if_SelectScene(sc, 1);
}
