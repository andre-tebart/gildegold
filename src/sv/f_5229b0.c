/* 0x5229b0, the per-frame update of the item buttons for the selected building: when the selection (0x6f2308 / 0x6f2310 / the scene 0x633444) changed, the buttons are freed and rebuilt (0x5224a0); without a
   selection the player's background bitmap is drawn; with the mouse (flag 0x2000 of param_1) over a button cell the item (0x4ffe90 / 0x4ffcc0 by the cell colour) is shown in the panel (0x478870) or, for a
   foreign one, 0x59e0a0 / 0x521960 is called. ROUGH */

void sv_Unk5229b0(unsigned flags)
{
    int i;

    if (d3_CurrentScene != ai_Unk623e8c || (int)sv_PanelShown != ai_Unk6f2360 || (int)ai_PlayerBuilding != ai_Unk6f235c || sv_ScreenDirty != 0) {
        ai_Unk6f235c = (int)ai_PlayerBuilding;
        ai_Unk6f2360 = (int)sv_PanelShown;
        for (i = 0; i < 15; i++) {
            if (ai_Unk7662e0[i] != -1) {
                d2_RemoveObj(ai_Unk7662e0[i]);
                ai_Unk7662e0[i] = -1;
            }
        }
        if (ai_PlayerBuilding != 0 || ai_FallbackBuilding != 0)
            sv_Unk5224a0();
        ai_Unk623e8c = d3_CurrentScene;
    }
    if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0) {
        sv_ScreenDirty = 0;
        return;
    }
    if (ai_Unk6f22c8 != 0)                    /* 0x522b0c: the surface 0x6f22c8 is blitted as the player's background strip at (screenW - 0x6e, 0x5b), keyed */
        d2_BlitPartMode(0, 0, *(int *)(ai_Unk6f22c8 + 4), *(int *)(ai_Unk6f22c8 + 8), (void *)ai_Unk6f22c8, (int)d2_ScreenW - 0x6e, 0x5b, d2_ScreenSurf, 1);
    if ((flags & 0x2000) == 0)
        sv_ScreenDirty = 0;
}
