/* 0x593e80, the information listener (186 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); the contacts
   INFORMATION (0x143cb38, 0xc) -> 0x5a45c0, MEISTERBRIEF (0x143cbc8, 0xc) -> 0x5c3200, PERSONALBUCH
   (0x143c9f0, 0xc) -> 0x5aab80 (param_1, the name), each when it is the pending 0x6f22e4 */

int ai_Unk5b9db0(); int ai_Unk5b9730(); void ai_Unk5a45c0(); 
int ai_Unk5b2180();

void ai_Unk593e80(int param_1)
{
    int r;
    int inf;
    int meister;
    int personal;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk593e80, 0x67fff);
    while (r != 0) {
        inf = sv_RegisterNamePlate("contact_INFORMATION", U_I((unsigned)&full_data_143cb38), 0xc);
        meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
        personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == inf)
                ai_Unk5a45c0(param_1);
        }
        r = sv_RunFrame(ai_Unk593e80, 0x67fff);
    }
}
