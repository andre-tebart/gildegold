/* 0x5965f0, the rhetoric contact listener (300 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); DROHBRIEF
   (0x143ff1c, 0xc) -> 0x5b4fb0 (param_1) + 0x52d7a0; PAMPHLET (0x14405f0, 0xc) -> 0x5b5610 (param_1);
   RHETORIK (0x143fc78, 0xc) -> 0x5b3130 (param_1, param_2, 4); while 0x6f2448 is set: the state 0x1468410
   0x19 -> 0x5b5610 and ' ' -> 0x5b4fb0 */

void ai_Unk5965f0(int param_1, int param_2)
{
    int r;
    int drohbrief;
    int pamphlet;
    int rhetorik;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5965f0, 0x67fff);
    while (r != 0) {
        drohbrief = sv_RegisterNamePlate("contact_DROHBRIEF", U_I((unsigned)&full_data_143ff1c), 0xc);
        pamphlet = sv_RegisterNamePlate("contact_PAMPHLET", U_I((unsigned)&full_data_14405f0), 0xc);
        rhetorik = sv_RegisterNamePlate("contact_RHETORIK", U_I((unsigned)&full_data_143fc78), 0xc);
        if (U_I((unsigned)&ai_Unk6f2448) != 0) {
            if (U_I((unsigned)&kb_Last) == 0x19)
                ai_Unk5b5610(param_1);
            if (U_I((unsigned)&kb_Last) == ' ')
                ai_Unk5b4fb0(param_1);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == drohbrief) {
                ai_Unk5b4fb0(param_1);
                sv_CameraFollow();
            } else if (U_I((unsigned)&ai_ClickedObject) == pamphlet) {
                ai_Unk5b5610(param_1);
            } else if (U_I((unsigned)&ai_ClickedObject) == rhetorik) {
                ai_Unk5b3130(param_1, param_2, 4);
            }
        }
        r = sv_RunFrame(ai_Unk5965f0, 0x67fff);
    }
}
