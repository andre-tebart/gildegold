/* 0x59ad00, the wine cellar contact listener (96 B): the pump 0x5309a0 (0x59ad00, 0x67fff) looks up
   "contact_WEINSCHRANK" (0x1440aec, class 0x16) and opens the cellar window 0x59aae0 when it is clicked */

void ai_WineCellarContact(int building, int room)
{
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_WineCellarContact, 0x67fff) != 0) {
        c = sv_RegisterNamePlate("contact_WEINSCHRANK", ai_ContactTextWineCellar, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_WineCellarWindow();
    }
}
