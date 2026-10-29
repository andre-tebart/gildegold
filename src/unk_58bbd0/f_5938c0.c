/* 0x5938c0, the mining contact listener (152 B): 0x52e190, then the pump 0x5309a0 (this, 0x67fff); while the
   flag 0x6f231c has 0x200, the contacts 0x52e1c0 ("contact_SUCHEN_ERZ", 0x14420d0, 0x16) /
   ("contact_ABBAUEN", 0x14420a8, 10) are asked; when one is the pending 0x6f22e4, 0x5923c0 (param_1) /
   0x592b90 (param_1) open */

int ai_Unk5923c0(int param_1);
int ai_Unk592b90(int param_1);

void ai_Unk5938c0(int param_1)
{
    int r;
    int a;
    int b;

    a = 0;
    b = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5938c0, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            a = sv_RegisterNamePlate("contact_SUCHEN_ERZ", U_I((unsigned)&full_data_14420d0), 0x16);
            b = sv_RegisterNamePlate("contact_ABBAUEN", U_I((unsigned)&full_data_14420a8), 10);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == a)
                ai_Unk5923c0(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == b)
                ai_Unk592b90(param_1);
        }
        r = sv_RunFrame(ai_Unk5938c0, 0x67fff);
    }
}
