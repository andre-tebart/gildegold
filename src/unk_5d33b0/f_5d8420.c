/* 0x5d8420, the contact listener of the production buildings (429 B): building the building record,
   arg the caller's argument. The contacts 0x52e1c0 are created on the flags 0x6f231c: bit 0x200
   -> contact_PRODUKTION_SCHNEIDER 0x62dc48 (0x144aa84, 0x13), contact_ROHSTOFFE_SUCHEN_SCHNEIDER
   0x62dc24 (0x14401b4, 0x16) and contact_PLAUSCH 0x62dc14 (0x144aa58, 0xc); bit 0x400 ->
   contact_LAGER, contact_TRANSPORT, ob_PERSONALBUCH, ob_MEISTERBRIEF. The click 0x6f22e4 dispatches:
   LAGER -> 0x5b26b0 (param_1, the dword +0x5d), SCHNEIDER -> 0x58bd60 (param_1), TRANSPORT ->
   0x5af1a0 (param_1), PERSONALBUCH -> 0x5aab80 (param_1, "ob_PERSONALBUCH"), MEISTERBRIEF ->
   0x5c3200 (param_1), ROHSTOFFE -> 0x595480 (param_1, param_2), PLAUSCH -> 0x5d8360. Then
   0x58f510 (param_1). The pump 0x5309a0 (0x5d8420, 0x67fff) runs between 0x52e190 calls. */

/* ai.h's AIBLD (0xa9 bytes, packed) as the contact listener reads it: the head of the building's object
   list. */
#pragma pack(push, 1)
typedef struct AIBLDVIEW {
    char pad0[0x5d];
    int objList;                        /* +0x5d, AIBLD.objList: the objects of the building */
} AIBLDVIEW;
#pragma pack(pop)

void ai_ProductionContact(int building, int arg)
{
    int lager, schn, trans, personal, meister, roh, plausch;
    int pump;
    int click;

    lager = schn = trans = personal = meister = roh = plausch = 0;
    sv_ReleaseNamePlates();
    pump = sv_RunFrame(ai_ProductionContact, 0x67fff);
    while (pump != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            schn = sv_RegisterNamePlate("contact_PRODUKTION_SCHNEIDER", ai_ContactTextProductionTailor, 0x13);
            roh = sv_RegisterNamePlate("contact_ROHSTOFFE_SUCHEN_SCHNEIDER", ai_ContactTextRawMaterialSearch, 0x16);
            plausch = sv_RegisterNamePlate("contact_PLAUSCH", ai_ContactTextChat, 0xc);
        }
        if ((ai_BuildingPerms & 0x400) != 0) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            trans = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
        }
        if ((click = ai_ClickedObject) != 0) {
            if (click == lager) {
                ai_StorageWindow(building, ((AIBLDVIEW *)building)->objList);
            } else if (click == schn) {
                ai_ProductionWindow((unsigned char *)building);
            } else if (click == trans) {
                ai_OpenTransport(building);
            } else if (click == personal) {
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            } else if (click == meister) {
                ai_MasterLetterWindow((unsigned char *)building);
            } else if (click == roh) {
                ai_TradeSearchWindow((unsigned char *)building, arg);
            } else if (click == plausch) {
                ai_TailorContact(building, arg);
            }
        }
        ai_MarketContact((struct AIBLD *)building);
        pump = sv_RunFrame(ai_ProductionContact, 0x67fff);
    }
    sv_ReleaseNamePlates();
    return 0;
}
