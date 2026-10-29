/* 0x596080, the aquarium contact listener (151 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with 0x200 the
   contacts 0x52e1c0 FLIEGENGLAS (0x143cf58, 0x16), TERRARIUM (0x143cf78, 0x16) and TEICH (0x143cf50, 0x16);
   when TEICH is the pending 0x6f22e4, the trade search 0x595a00 (param_1, param_2) opens */

void ai_AquariumContact(int building, int room)
{
    int r;
    int teich;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_AquariumContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            sv_RegisterNamePlate("ob_FLIEGENGLAS", ai_ContactTextFlyGlass, 0x16);
            sv_RegisterNamePlate("ob_TERRARIUM", ai_ContactTextTerrarium, 0x16);
            teich = sv_RegisterNamePlate("ob_TEICH", ai_ContactTextPond, 0x16);
            if (ai_ClickedObject == teich)
                ai_AquariumSearchWindow(building, room);
        }
        r = sv_RunFrame(ai_AquariumContact, 0x67fff);
    }
}
