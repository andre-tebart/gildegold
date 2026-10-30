/* 0x5d90e0, the curse target picker "Locations\zigeuner\blutrache" (1216 B): the building is the gypsy house. A clock is
   made for the next day (the game time + 24 hours, the hour 21 + a random 0 / 1, the minute 0). The form has two sliders: the
   share (progress of a running curse, 0 .. 100, drawn as a wide bar at the top) and the victim (0 .. gm_CurseTargetCount - 1,
   the strength of the new curse) and two buttons 0x41f6 / 0x41f7. Every 0x8c ms or when the victim value changes the state is
   rebuilt: with no running curse (ai_FindCurseEvent of the building is 0) the clock is made again, the share bar goes back to 0
   and the text 0x41f5 shows the strength class (2 for a value below 4, 3 for 4 .. 6, 4 above) with the victim name 0x41fb + the
   value and the due clock; with a running curse the clock is its due time (two hours later while it is not yet done), the share
   is the minutes from its start to now over those to the due time (at most 1, times 100) and the text 0x41f8 (the victim
   0x41fb + its id) or 0x41f9 (done) is shown. The buttons are enabled and the victim slider is used only without a running
   curse, the share bar only with one. The right mouse button and the second button leave; the first button pays the class
   (sv_CheckMoney, gm_SendMoneyDelta) and sends the message 0xa0 (the building id, the game time, arg 0 = the strength,
   flags 2) with cm_SendLongMessage. The pump ends with d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 363 vs 365 instructions, the clock (GAMECLOCK, memcpy of ai_Time + gm_AddTime, hour 21 + random, minute 0), the two sliders, the 0x8c ms / value-change rebuild with the running-curse (ai_FindCurseEvent) and the new-curse branches in the original order, the buttons, the pay + 0xa0 message and the tail jump all match. The diff has 7 slot hunks (a 4 byte shift of the ids / tick locals: ours keeps a float local for the share ratio, the original has none) and ONE code hunk of 28 instructions at 0x5d92c7, the share ratio: the original stores the first gm_MinutesBetween result as an int ([esp+0x44]) and later does fild first; fidiv second (the second result also in memory), then fcom 1.0 / fnstsw / test ah,5 / jnp; ours converts the first result at once (fild; fstp float temp before the second call), then fild second; fdivr temp. Tried: one expression (float)f() / g() (this), named int locals total / elapsed (same early fild), the clamp as a ternary with > and >= (changes the compare to and eax,0x4100 / 0x100 and does not remove the early conversion), swapping the if / else order (fixed three other hunks). Allocation / scheduling of the FPU expression only. */

void ai_CurseTargetWindow(int building)
{
    CURSEMSG msg;
    GAMECLOCK when;
    int ids[2];                         /* the two buttons */
    int share, victim, form;
    int value, cls;
    unsigned tick;
    CURSEEVENT *ev;
    float ratio;

    tick = 0;
    cls = 2;
    value = 0;
    memcpy(&when, ai_Time, sizeof(GAMECLOCK));
    gm_AddTime((struct GCLOCK *)&when, 0x18, 0, 0);
    when.hour = util_RandomU(cls) + 0x15;
    when.min = 0;
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    share = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 10, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_SetObjValue4(share, 0, 100, 0);
    d2_SetForm(form, 2);
    victim = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    gm_CurseTargetCount((char *)building);
    d2_SetObjValue5(victim, 0, gm_CurseTargetCount((char *)building) - 1, 0, 1);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41f6], txt_Ptrs[0x41f7]);
    do {
        if (d3_Time > tick + 0x8c || d2_GetObjValue(victim) != value) {
            tick = d3_Time;
            ev = (CURSEEVENT *)ai_FindCurseEvent((struct AIBLD *)building);
            if (ev != 0) {
                when = ev->due;
                when.hour += ev->done ? 0 : 2;
                ratio = (float)gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)ai_Time)
                        / gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)&when);
                if (ratio > 1.0f)
                    ratio = 1.0f;
                d2_SetObjValue4(share, 0, 100, (int)(ratio * 100.0f));
            } else {
                memcpy(&when, ai_Time, sizeof(GAMECLOCK));
                gm_AddTime((struct GCLOCK *)&when, 0x18, 0, 0);
                when.hour = util_RandomU(2) + 0x15;
                when.min = 0;
                d2_SetObjValue4(share, 0, 100, 0);
            }
            d2_SetForm(form, 1);
            io_PrintText("$C");
            if (ev != 0) {
                if (ev->done == 0)
                    io_PrintText((char *)0x41f8, 0x41f4, ev->victim + 0x41fb, &when);
                else
                    io_PrintText((char *)0x41f9, 0x41f4);
            } else {
                value = d2_GetObjValue(victim);
                if (value < 4)
                    cls = 2;
                else
                    cls = (value >= 7) + 3;
                io_PrintText((char *)0x41f5, 0x41f4, cls, value + 0x41fb, &when);
            }
            d2_SetObjEnabled(ids[0], ev == 0);
            d2_SetObjEnabled(ids[1], ev == 0);
            d2_SetForm(form, 2);
            d2_EnableObject(share, ev != 0);
            d2_EnableObject(victim, ev == 0);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, cls) == 1) {
                    gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -cls);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xa0;
                    msg.flags = 2;
                    msg.to = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId;
                    msg.fromId = ((CURSEBLD *)building)->id;
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
                    msg.arg[0] = value;
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_CurseTargetWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
