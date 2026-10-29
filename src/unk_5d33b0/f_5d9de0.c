/* 0x5d9de0, opens the poker table window "Locations\zigeuner\poker" 0x62de10 (273 B): returns 0 when a window is open
   already (ai_PokerUi.window != -1) or the form cannot be opened, else 1. The pointer sprite is cleared, the view
   centred, and the panels are initialised from the windows the forms 1, 2 and 3 made (each takes d2_LastWindow and clears its
   five slots to -1; the form 3 panel, first in memory, is marked shown); the form 6 row of five buttons (the texts 0x144aae8 ..
   0x144aaf8, ids into ai_PokerUi.buttons) gets its third button disabled. The poker family 0x5d9de0 ..
   0x5db110 (the data block 0x62dda0 .. 0x62de10 is one POKERUI); the meaning of `active` (0x62ddb8) is not derived. @deep Body read whole against the disassembly (open guard, form, three panels from the windows of the forms 1 / 2 / 3, button row of five on the form 6, third button disabled; data block 0x62dda0 reconstructed as POKERUI). 91 vs 93 instructions, no meaning difference left: the original loads the -1 once more into eax (mov eax,edi) and stores it through eax for the slots of the first two panels (and the shown flag 0 / 1 interleaved: slot0, slot1, win, slot2, shown, slot3, slot4) while ours stores through the hoisted -1 register directly; the compare constant sits in edi in the original (esi in ours); the five button texts are loaded in a different order for the push. asmdiff: hunks at 0x5d9e37 (80 ins) and 0x5d9f19 (30 ins) are pure scheduling / allocation of the constant stores. Tried: chained assignment of the slots, a named local int none = -1 for the compare and stores, natural per-slot order. */

int ai_OpenPokerWindow(void)
{
    if (ai_PokerUi.window != -1)
        return 0;
    ai_PokerUi.window = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker");
    if (ai_PokerUi.window == -1)
        return 0;
    ai_PokerUi.active = -1;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(ai_PokerUi.window);
    d2_SetForm(ai_PokerUi.window, 1);
    ai_PokerUi.panel[1].win = d2_LastWindow;
    ai_PokerUi.panel[1].slot[0] = -1;
    ai_PokerUi.panel[1].slot[1] = -1;
    ai_PokerUi.panel[1].slot[2] = -1;
    ai_PokerUi.panel[1].slot[3] = -1;
    ai_PokerUi.panel[1].slot[4] = -1;
    ai_PokerUi.panel[1].shown = 0;
    d2_SetForm(ai_PokerUi.window, 2);
    ai_PokerUi.panel[2].win = d2_LastWindow;
    ai_PokerUi.panel[2].slot[0] = -1;
    ai_PokerUi.panel[2].slot[1] = -1;
    ai_PokerUi.panel[2].slot[2] = -1;
    ai_PokerUi.panel[2].slot[3] = -1;
    ai_PokerUi.panel[2].slot[4] = -1;
    ai_PokerUi.panel[2].shown = 0;
    d2_SetForm(ai_PokerUi.window, 3);
    ai_PokerUi.panel[0].win = d2_LastWindow;
    ai_PokerUi.panel[0].slot[0] = -1;
    ai_PokerUi.panel[0].slot[1] = -1;
    ai_PokerUi.panel[0].slot[2] = -1;
    ai_PokerUi.panel[0].slot[3] = -1;
    ai_PokerUi.panel[0].slot[4] = -1;
    ai_PokerUi.panel[0].shown = 1;
    d2_SetForm(ai_PokerUi.window, 6);
    sv_ButtonRow(d2_LastWindow, 0, ai_PokerUi.buttons, 5, txt_Ptrs[0x424a], txt_Ptrs[0x424b], txt_Ptrs[0x424c], txt_Ptrs[0x424d], txt_Ptrs[0x424e]);
    d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
    return 1;
}
