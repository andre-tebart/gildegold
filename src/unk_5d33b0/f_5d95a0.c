/* 0x5d95a0, the "Ehrerbietung" (homage) window "Locations\zigeuner\blutrache" (745 B): the building is the gypsy house.
   A building without an owner (owner 0xffff) ends it at once (camera follow). When the player's status bit 0x20 (+0x1cc of
   its record) is set the green message box 0x4207 / 0x4208 refuses. Otherwise the person to honour is chosen with the
   special-amount menu ai_AmountMenu (the config of the other gypsy windows, the text 0x4209; none ends the window) and the form
   shows a slider (the gift, 2 .. 8, starting at 2) and the two buttons 0x420c / 0x420d. Each pass the gift value v is read
   and the text 0x420a (the person's id, v) is shown. The right mouse button and the second button leave; the first button
   pays when the player has the money (sv_CheckMoney of v): the status bit 0x20 is set (cm_SendSetField), v is paid
   (gm_SendMoneyDelta) and the person's relation to the player rises by v * 5 (cm_SendRelations); a player character (kind 6 / 7)
   gets the news 0x420e ("_NACHRICHTEN_ZIGEUNER_04", channel 0x5f0) with the id of the player (he_ShowNews). The pump ends
   with d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 223 = 223 instructions, 745 = 745 bytes; the owner / status guards, the amount-menu config (AMOUNTCFG with the const volatile 64-bit filter), the form with the 2 .. 8 gift slider and the buttons, the per-pass text, the confirm branch (status bit 0x20, money, relation v * 5, the news for the player characters through he_ShowNews) and the tail jumps all match. ONE hunk of 6 differing instructions is left at 0x5d9604: the original loads the hi dword of the filter (mov edx,[0x5eed6c]) before the first rep stosd and pushes the hoisted 0 (the amount menu's last arguments) after the second lea edi, ours pushes the 0 before the first rep stosd and loads the hi dword after the second; cfg is then 4 bytes lower ([esp+0x30] vs [esp+0x2c]). Scheduling of constant loads / pushes around the two inline memsets. Tried: the filter assigned first (8 instructions), the id clearing as a dword loop instead of memset (same code). */

void ai_HomageWindow(int building)
{
    char text[0x400];
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int ids[2];                         /* the two buttons */
    int slider, form;
    AICHARVIEW *target;
    int value;

    if (((CURSEBLD *)building)->owner == 0xffff) {
        sv_CameraFollow();
        return;
    }
    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 0x20) {
        sv_MessageBoxGreen((int)txt_Ptrs[0x4207], (int)txt_Ptrs[0x4208], 0);
        return;
    }
    memset(&cfg, 0, sizeof(cfg));
    memset(cfg.ids, 0xff, sizeof(cfg.ids));
    cfg.flags = 1;
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    target = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x4209], 0, 0);
    if (target == 0) {
        sv_CameraFollow();
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    slider = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue5(slider, 2, 8, 2, 1);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x420c], txt_Ptrs[0x420d]);
    do {
        value = d2_GetObjValue(slider);
        d2_SetForm(form, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x420a, 0x4207, target->id, value);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, value) == 1) {
                    cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 0x20, 0);
                    gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -value);
                    cm_SendRelations(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, target->netId, value * 5, 1);
                    if (target->kind == 6 || target->kind == 7) {
                        txt_Format(text, 0x420e, ai_PlayerId);
                        he_ShowNews(target->netId, -1, text, 0, 0x5f0, "_NACHRICHTEN_ZIGEUNER_04");
                    }
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_HomageWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
