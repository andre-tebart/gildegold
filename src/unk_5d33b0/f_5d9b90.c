/* 0x5d9b90, the theatre / card game contact listener (127 B): building the building.
   contact_THEATERSTUECK 0x62dd20 (ai_ContactTextTheatre 0x144a98c, 0x16) opens the curse target picker
   0x5d90e0 (ai_CurseTargetWindow), contact_KARTENSPIEL 0x62dd0c (ai_ContactTextCardGame 0x144aa9c,
   0x16) the poker game 0x5da030 (ai_PokerGame), between sv_ReleaseNamePlates calls and the pump
   0x5309a0 (this function, 0x67fff). */

void ai_TheatreContact(int building)
{
    int theater = 0;
    int karten;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_TheatreContact, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0)
            theater = sv_RegisterNamePlate("contact_THEATERSTUECK", ai_ContactTextTheatre, 0x16);
        karten = sv_RegisterNamePlate("contact_KARTENSPIEL", ai_ContactTextCardGame, 0x16);
        clicked = ai_ClickedObject;
        if (clicked != 0) {
            if (clicked == theater)
                ai_CurseTargetWindow(building);
            else if (clicked == karten)
                ai_PokerGame();
        }
    }
}
