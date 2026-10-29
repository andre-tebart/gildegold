/* 0x5d9a80, the gypsy contact listener (97 B): building the gypsy building. contact_BLUTRACHE
   0x62dce0 (ai_ContactTextBloodRevenge 0x144a884, 0x16) opens the blood revenge window 0x5d85e0
   (ai_BloodRevengeWindow), contact_EHRERBIETUNG 0x62dcc8 (ai_ContactTextHomage 0x144a9d8, 0x16) the
   homage window 0x5d95a0 (ai_HomageWindow), between sv_ReleaseNamePlates calls and the pump 0x5309a0
   (this function, 0x67fff). The clicked object (ai_ClickedObject) selects one of the plates. */

/* ---- the contact listeners are defined above the stub bodies they open (out of address order:
   0x5d9a80 / 0x5d9b20 / 0x5d9b90 belong after 0x5d9890). VC6 drops a call to a function whose
   body it has already seen when that body has no side effect: the ROUGH stub bodies of 0x5d85e0
   .. 0x5d9890 make the call vanish, the original's callers were compiled against the real
   bodies. Move the listeners back into address order once those have their bodies. ---- */

void ai_GypsyContact(int building)
{
    int blutrache = 0;
    int ehrerbietung = 0;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GypsyContact, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            blutrache = sv_RegisterNamePlate("contact_BLUTRACHE", ai_ContactTextBloodRevenge, 0x16);
            ehrerbietung = sv_RegisterNamePlate("contact_EHRERBIETUNG", ai_ContactTextHomage, 0x16);
        }
        clicked = ai_ClickedObject;
        if (clicked != 0) {
            if (clicked == blutrache)
                ai_BloodRevengeWindow(building);
            else if (clicked == ehrerbietung)
                ai_HomageWindow(building);
        }
    }
}
