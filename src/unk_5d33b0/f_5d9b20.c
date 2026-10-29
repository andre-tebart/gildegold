/* 0x5d9b20, the "Fluch wirken" (cast curse) contact listener (76 B): buildingArg the building.
   contact_FLUCH_WIRKEN 0x62dcf4 (ai_ContactTextCurse 0x144a8c0, 0x16) opens the curse window 0x5d8c90
   (ai_CurseWindow) when its plate is the clicked object (ai_ClickedObject), between
   sv_ReleaseNamePlates calls and the pump 0x5309a0 (this function, 0x67fff). */

void ai_CurseContact(int buildingArg)
{
    int fluch;
    int building;
    int clicked;

    /* the original keeps the building in a callee-saved register across the plate call */
    building = buildingArg;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_CurseContact, 0x67fff) != 0) {
        fluch = sv_RegisterNamePlate("contact_FLUCH_WIRKEN", ai_ContactTextCurse, 0x16);
        clicked = ai_ClickedObject;
        if (clicked != 0 && clicked == fluch)
            ai_CurseWindow(building);
    }
    return 0;
}
