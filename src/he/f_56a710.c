/* 0x56a710, the run handler of the debug event "HE_NEUE_VORKOMMEN_SUCHEN_MSG" (a bug text with the name of the good +0xac): state 0 opens the message panel
   "Misc\EventMessagebox" (0x53d6f0, slot at +0x74), shows the text and goes to state 1; state 1 waits until the panel (the window of the slot +8) is closed */

void he_Unk56a710(int ev)
{
    int id;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        id = *(int *)(ev + 0xac);
        he_Unk53d6f0(ev, 0, 0, (int)"Misc\\EventMessagebox", 0x5f2);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText("HE_NEUE_VORKOMMEN_SUCHEN_MSG: Ich bin ein Bug !!! %s gefunden!!", id * 2 + 0x9a8);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (U_I((unsigned)&d2_Unkf8aac4) == *(int *)(*(int *)(ev + 0x74) + 8) && U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_ClickedId) == 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
        }
        return;
    }
}
