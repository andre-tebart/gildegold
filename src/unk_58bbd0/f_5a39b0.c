/* 0x5a39b0, the church's top-5 / indulgence listener (141 B): the pump 0x5309a0 (this, 0x67fff) looks up
   "contact_TOP5" (0x1440768) and "contact_ABLASS" (0x144073c, both 0x16) and opens 0x5a28f0 / 0x5a1f70 */

void ai_ChurchContact(int building, int arg)
{
    int top5;
    int ablass;
    int click;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ChurchContact, 0x67fff) != 0) {
        top5 = sv_RegisterNamePlate("contact_TOP5", ai_ContactTextTop5, 0x16);
        ablass = sv_RegisterNamePlate("contact_ABLASS", ai_ContactTextIndulgence, 0x16);
        click = ai_ClickedObject;
        if (click != 0) {
            if (click == top5)
                ai_Top5Window();
            else if (click == ablass)
                ai_IndulgenceWindow(building, arg);
        }
    }
}
