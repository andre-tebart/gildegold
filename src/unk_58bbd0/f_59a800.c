/* 0x59a800, the market stand listener (736 B): nine stand contacts (0x52e1c0, class 0x10) and, in Nuernberg
   (strcmp of the city name 0x7e3f40 + city * 0x2f4), the three "contact_MARKTFLECKEN_0x" (state 0x143c700); the
   clicked contact 0x6f22e4 leaves the market via 0x59a710 (param_1, 0x92 Tischler / Schmiede .. 0x2db Okkult);
   the two "SCHWARZES_BRETT" contacts and "ob_TRIBUENE" open 0x5b5690 / 0x5b5a20; a market fleck needs its
   object +0x1f8 -> +0x200 and leaves 0x59a710 (that, 0x95) */

void ai_Unk59a800(int param_1)
{
    char *fleck1 = 0;
    char *fleck2 = 0;
    char *fleck3 = 0;
    char *c;

    sv_RegisterNamePlate("ob_MARKTSTAND_TISCHLER_SCHMIEDE", U_I((unsigned)&full_data_143ccf0), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_STEINMETZ_WIRT", U_I((unsigned)&full_data_143ccf8), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_PARFUM_KRAEUTER", U_I((unsigned)&full_data_143cd00), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_ROHSTOFFE", U_I((unsigned)&full_data_143cd08), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_IMPORTEUR", U_I((unsigned)&full_data_143cd10), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_KIRCHE", U_I((unsigned)&full_data_143cd18), 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_OKKULT", U_I((unsigned)&full_data_143df38), 0x10);
    sv_RegisterNamePlate("ob_SCHWARZES_BRETT", U_I((unsigned)&full_data_143cd20), 0x10);
    sv_RegisterNamePlate("ob_TRIBUENE", U_I((unsigned)&full_data_143a3ec), 0x10);
    if (strcmp((char *)(unsigned)&ai_PlayerSlots + U_I((unsigned)&ai_OwnPlayer) * 0x2f4, "Nuernberg") == 0) {
        fleck1 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_01", U_I(((unsigned)&ai_BuildingNames + 0x1300)), 0x10);
        fleck2 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_02", U_I(((unsigned)&ai_BuildingNames + 0x1300)), 0x10);
        fleck3 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_03", U_I(((unsigned)&ai_BuildingNames + 0x1300)), 0x10);
    }
    c = (char *)U_I((unsigned)&ai_ClickedObject);
    if (c == 0)
        return;
    if (strcmp(c, "ob_MARKTSTAND_TISCHLER_SCHMIEDE") == 0) {
        ai_Unk59a710(param_1, 0x92);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_STEINMETZ_WIRT") == 0) {
        ai_Unk59a710(param_1, 0x93);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_PARFUM_KRAEUTER") == 0) {
        ai_Unk59a710(param_1, 0x94);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_ROHSTOFFE") == 0) {
        ai_Unk59a710(param_1, 0x95);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_IMPORTEUR") == 0) {
        ai_Unk59a710(param_1, 0x96);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_KIRCHE") == 0) {
        ai_Unk59a710(param_1, 0x97);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_OKKULT") == 0) {
        ai_Unk59a710(param_1, 0x2db);
        return;
    }
    if (strcmp(c, "contact_ob_SCHWARZES_BRETT") == 0) {
        ai_Unk5b5690();
        return;
    }
    if (strcmp(c, "ob_SCHWARZES_BRETT") == 0) {
        ai_Unk5b5690();
        return;
    }
    if (strcmp(c, "ob_TRIBUENE") == 0) {
        ai_Unk5b5a20();
        return;
    }
    if (c != fleck1 && c != fleck2 && c != fleck3)
        return;
    if (U_I(U_I(c + 0x1f8) + 0x200) == 0)
        return;
    ai_Unk59a710(U_I(U_I(c + 0x1f8) + 0x200), 0x95);
}
