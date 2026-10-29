/* 0x592290, the book listener (143 B): 0x52e190, then the pump 0x5309a0 (this, 0x67fff); the two book contacts
   0x52e1c0 ("ob_MEISTERBRIEF", 0x143cbc8, 0xc) / ("ob_PERSONALBUCH", 0x143c9f0, 0xc) open 0x5c3200 (param_1)
   / 0x5aab80 (param_1, the name) when they are the pending 0x6f22e4 */

void ai_Unk592290(int param_1)
{
    int r;
    int a;
    int b;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk592290, 0x67fff);
    while (r != 0) {
        a = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
        b = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == a)
                ai_MasterLetterWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == b)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
        }
        r = sv_RunFrame(ai_Unk592290, 0x67fff);
    }
}
