/* 0x5327e0, the player count dialog (the form "Menu\CHOOSE_PLAYERCOUNT"): a count slider (at least 2, started from ai_NumPlayers), the heading text 0x1ab5 and the "%s$A%s" texts 0x7d / 0x7e; runs the
   frame loop 0x5309a0 until a button is pressed (button 0 = ok: stores the slider value in ai_NumPlayers and returns 1, right mouse button / button 1 = cancel); returns 1 for ok, 0 for cancel @deep body complete, 87 = 87 instructions, readable (d2 / dm_L / sv_PopRequest typed, slider / form names, the 4-argument d2_SetObjValue4 call, the right mouse button dm_L.rup cancels). One hunk left (14 ins at 0x53280d): the original clamps the player count as 'mov al,[ai_NumPlayers]; cmp al,2; mov byte [ai_NumPlayers],2; jb L; mov [ai_NumPlayers],al; L: mov eax,[ai_NumPlayers]; and eax,0xff; push eax' (immediate pre-store, then a reload of the global as dword + mask for the d2_SetObjValue argument); ours keeps the value in al / cl ('jae; mov al,2; mov [x],al; xor ecx,ecx; mov cl,al'). Tried: if (n < 2) n = 2; the ternary assignment; cnt local copy with ai_NumPlayers = 2 stored first and re-stored when cnt >= 2 (89 ins, worse); a char bitfield view of ai_NumPlayers (the dword + and 0xff read does not appear); & 0xff on the argument */

int sv_RunFrame(int cb, unsigned mask);

int main_PlayerCountDialog(void)
{
    int ok, form, slider, run;

    ok = 0;
    form = d2_OpenForm(0, 0, "Menu\CHOOSE_PLAYERCOUNT");
    d2_CentreViewWindows(form);
    slider = d2_GetWinObj(form, 0, 0);
    if (ai_NumPlayers < 2)
        ai_NumPlayers = 2;
    d2_SetObjValue4(slider, 2, 8, ai_NumPlayers);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 1);
    io_PrintText(0x1ab5);
    d2_SetForm(form, 2);
    io_PrintText("%s$A%s", 0x7d, 0x7e);
    run = sv_RunFrame((int)main_PlayerCountDialog, 0x24087);
    while (run != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 0) {
            ai_NumPlayers = d2_GetObjValue(slider);
            d2_SetViewEnabled(form, 0);
            ok = 1;
            sv_PopRequest = 1;
        }
        run = sv_RunFrame((int)main_PlayerCountDialog, 0x24087);
    }
    if (form != -1)
        d2_CloseView(form);
    return ok;
}
