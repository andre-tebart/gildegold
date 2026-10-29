/* 0x5d7430, the quicksave: in a normal game (ai_GameFlags bit 2 clear, bit 7 clear) it shows the
   "Quicksave" status text (sv_SetStatusText 0x52e330), pumps one frame (sv_RunFrame 0x5309a0 (0,
   0x657c7)), takes the screenshot (ai_Screenshot 0x5d7160), sets the save slot (ls_SaveSlot = 1),
   writes the save (ls_SaveGame 0x5190b0: "Gamedata\Saves\Quicksave.SAV" 0x62db54, "Quicksave"
   0x62db74, 1) and clears the status text (ch_EmptyName 0x631090). In a network game (bit 2): without
   bit 4 it shows ai_QuickSaveDisabledText; when ai_HasQuicksavePlayer finds no saveable player it
   shows ai_QuickSaveNoPlayerText; otherwise it sends the 0xf request (CMQUICKSAVE: slot 1, name
   "QUICKSAVE" 0x624528) through cm_SendExSysMessage, then waits for the message slot to change
   (cm_GetSlotState) while pumping the net (sv_PumpNet 0x52fe00). Void; the callers ignore the result. */

void ai_Quicksave(void)
{
    CMQUICKSAVE rec;
    char flags;
    int msg;
    int r;

    flags = ai_GameFlags;
    if ((flags & 4) != 0) {
        if ((flags & 0x10) == 0) {
            sv_SetStatusText(ai_QuickSaveDisabledText);
            return;
        }
        if (ai_HasQuicksavePlayer() == 0) {
            sv_SetStatusText(ai_QuickSaveNoPlayerText);
            return;
        }
        memset(&rec, 0, sizeof(rec));
        rec.slot = 1;
        strcpy(rec.name, "QUICKSAVE");
        /* @original cm.h declares cm_SendExSysMessage void, but the original caller reads the call's
           eax as the message handle (0x4c4db0 returns 0x4c39c0's int); the call goes through a
           prototype that returns int and the handle is passed to cm_GetSlotState */
        msg = ((int (__cdecl *)(unsigned char, void *))cm_SendExSysMessage)(0xf, &rec);
        r = cm_GetSlotState(msg);
        if (r == 0) {
            do {
                sv_PumpNet();
                r = cm_GetSlotState(msg);
            } while (r == 0);
        }
        return;
    }
    if ((flags & 0x80) != 0)
        return;
    sv_SetStatusText("Quicksave");
    sv_RunFrame(0, 0x657c7);
    ai_Screenshot();
    ls_SaveSlot = 1;
    ls_SaveGame("Gamedata\\Saves\\Quicksave.SAV", "Quicksave", 1);
    sv_SetStatusText(ch_EmptyName);
    return;
}
