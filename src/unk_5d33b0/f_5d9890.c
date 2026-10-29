/* 0x5d9890, the gypsy group window "Locations\zigeuner\blutrache" form (661 B): the building is the gypsy house.
   Opens the form (the player bar sv_OpenPlayerBar first), shows the texts 0x41e7 / 0x41e6 / 0xed6 and a row of two buttons
   (txt_Ptrs[0x41e8] / [0x41e9]). The right mouse button and the second button leave; the first button collects up to four
   characters of the group selection (flag +0x188, ai_Selected): none shows the message box txt_Ptrs[0x41ea], otherwise the long message
   0xa1 (flags 1) is sent (cm_SendLongMessage) with the sender (the player's net id), the building id, the game clock and the net
   ids of the collected characters; the window then ends. The pump ends with d2_CloseView, the player bar close and the camera
   follow. Reasoned: the name and the purpose follow from the message contents (the building id, the clock, the chosen players),
   the form is the one the other gypsy windows use; message 0xa1 is not read at its receiver. Body read whole
   against the disassembly. */

void ai_GypsyGroupWindow(int building)
{
    CURSEMSG msg;
    int chosen[4];                      /* the indices of the collected characters */
    int ids[2];                         /* the two buttons */
    int form, count, i;

    sv_OpenPlayerBar(building, 0);
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x41e7, 0x41e6, 0xed6);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41e8], txt_Ptrs[0x41e9]);
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                count = 0;
                for (i = 0; i < 768 && count < 4; i++) {
                    if (((AICHARVIEW *)(ai_Chars + i * 0x21c))->selected != 0) {
                        chosen[count] = i;
                        count++;
                    }
                }
                if (count != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xa1;
                    msg.flags = 1;
                    msg.to = ((AICHARVIEW *)(ai_Chars + ai_PlayerId * 0x21c))->netId;
                    msg.fromId = ((CURSEBLD *)building)->id;
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
                    for (i = 0; i < count; i++)
                        msg.ids[i] = ((AICHARVIEW *)(ai_Chars + chosen[i] * 0x21c))->netId;
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                } else {
                    sv_MessageBox((int)txt_Ptrs[0x41ea], 0);
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GypsyGroupWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
