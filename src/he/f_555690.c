/* 0x555690, the run handler of the return leg of the transport event (a Ghidra miss; ROUGH, same record as 0x555350): the first round stops the transporter (0x4c5230), puts it on the road (0x4c5260 (id, 1) unless the target is a 0x5024f0 building), sends it home ("Trans", 0x4c52e0 (id, b1 id, -1, 0, 0, ..)) and starts the guide 0x4c4920; later rounds check the arrival (0x524810 (transporter, b1)), restart the guide and format the arrival news (0x1c39 + 0x1c3b / 0x1c3d / 0x1c3f / 0x1c43 / 0x1c44 by the kinds 0x47 / 0x1d .. 0x20 of the two buildings and the player's name 0x7e3f40) which a player character gets through 0x53dee0 (text 0x609, the strings 0x627318 / 0x627304) unless the world map shows the building (0x6f229c / 0x6f230c); the event is then posted again with the handle 3 */

int sv_Unk524810();

void he_Unk555690(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, tr, e;
    int skip;
    char text[0x400];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    tr = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a45;
    if (tr != 0) {
        he_Unk5552c0(ev, tr);
        if (*(unsigned char *)(ev + 0xd0) == 0) {
            ai_Unk4c5230(*(int *)(tr + 2));
            if (gm_IsType10(b0) == 0)
                ai_Unk4c5260(*(int *)(tr + 2), 1);
            ai_Unk4c52e0(*(int *)(tr + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
            if (gm_IsType10(b0) == 0)
                *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(tr + 2));
            *(unsigned char *)(ev + 0xd0) = 1;
        } else if (sv_Unk524810(tr, b1) != 0) {
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(tr + 2));
            if (*(unsigned char *)b1 == 0x47) {
                txt_Format(text, 0x1c3b, 1, 0x1c39, (char *)(unsigned)&ai_PlayerSlots + *(int *)(b1 + 0x65) * 0x2f4);
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, (char *)((unsigned)&ai_Unk625820 + 0x1af8), 0);
            } else if (*(unsigned char *)b0 == 0x47) {
                txt_Format(text, 0x1c3d, 1, 0x1c39);
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, (char *)((unsigned)&ai_Unk625820 + 0x1ae4), 0);
            } else {
                e = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b1 * 0x24d);
                if (e == 0x1d || e == 0x1f) {
                    if (gm_IsType10(b1) != 0)
                        txt_Format(text, 0x1c43);
                    else
                        txt_Format(text, 0x1c44, b1);
                } else {
                    txt_Format(text, 0x1c3f, 1, 0x1c39);
                }
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, 0, 0);
            }
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        }
    }
    d3if_SelectScene(sc, 1);
}
