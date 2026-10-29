/* 0x592320, the mercenary-book listener (160 B): 0x52e190, then the pump 0x5309a0 (this, 0x67fff); while the
   flag 0x6f231c has 0x200 the two contacts 0x52e1c0 ("ob_SOELDNERBUCH", 0x143a3e8, 0xc) /
   ("ub_STAENDER_WAFFEN", 0x1441fb8, 0xd) are fetched, and when one is the pending 0x6f22e4, 0x5ac110 (param_1,
   the name) / 0x5bb350 (param_1) open */

void ai_Unk592320(int param_1)
{
    int r;
    int a;
    int b;

    a = 0;
    b = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk592320, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            a = sv_RegisterNamePlate("ob_SOELDNERBUCH", U_I((unsigned)&full_data_143a3e8), 0xc);
            b = sv_RegisterNamePlate("ub_STAENDER_WAFFEN", U_I((unsigned)&full_data_1441fb8), 0xd);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == a)
                ai_Unk5ac110(param_1, "ob_SOELDNERBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == b)
                ai_Unk5bb350(param_1);
        }
        r = sv_RunFrame(ai_Unk592320, 0x67fff);
    }
}
