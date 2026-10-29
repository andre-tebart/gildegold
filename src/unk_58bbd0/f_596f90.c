/* 0x596f90, the mistress listener (95 B): the pump 0x5309a0 (this, 0x67fff); the contact GELIEBTE
   (0x1440af8, 0x16) opens the mistress window 0x596ba0 (param_1) when it is the pending 0x6f22e4 */

void ai_MistressContact(int building, int room)
{
    int r;
    int geliebte;

    r = sv_RunFrame(ai_MistressContact, 0x67fff);
    while (r != 0) {
        geliebte = sv_RegisterNamePlate("contact_GELIEBTE", ai_ContactTextMistress, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == geliebte)
            ai_MistressWindow(building);
        r = sv_RunFrame(ai_MistressContact, 0x67fff);
    }
}
