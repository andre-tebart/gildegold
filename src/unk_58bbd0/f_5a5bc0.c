/* 0x5a5bc0, the kidnapping contact listener (694 B, room 0x65 of the building driver 0x59e420): the victim
   0x505df0 (param_1 + 0x65): without a cell record 0x4fbb80 (the victim, 0x633444, 0, 0, 0, "dummy_EINGANG_ZELLE")
   when +0x184 is clear and the kind +2 is not 6 / 7, or 0x4c51b0 (the victim, the owner, param_2 + 2, the name)
   when the cell 0x184 belongs to another record; 0x52e190, the pump 0x5309a0 (this, 0x67fff) fetches
   contact_ENTFUEHREN (0x144089c, 0x16) / contact_LOESEGELD (0x14408cc, 0x16) under the mask 0x200 and
   contact_AUSBRECHEN (0x144a17c, 0x16) for an own victim with the byte +0x1b1 set; the pending click 0x6f22e4
   opens 0x5a53e0 / 0x5a5530 / 0x5a5910; the cell 0x4028a0 (the record) and +0x184 = 0 on the exit */

int unk_Unk4fbb80(); void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s); int ch_Delete();

void ai_Unk5a5bc0(int param_1, int param_2)
{
    int entfuehren = 0;
    int loesegeld = 0;
    int ausbrechen = 0;
    int ch = 0;
    int cell = 0;
    int r;

    if (U_I(param_1 + 0x65) != -1) {
        ch = gm_FindChar(U_I(param_1 + 0x65));
        if (ch != 0) {
            if (U_I(ch + 0x184) == 0 && *(char *)(ch + 2) != 6 && *(char *)(ch + 2) != 7)
                cell = unk_Unk4fbb80(ch, U_I((unsigned)&d3_CurrentScene), 0, 0, 0, "dummy_EINGANG_ZELLE");
            if (U_I(ch + 0x184) != 0 && U_I(U_I(ch + 0x184) + 0x30) != U_I(param_2 + 2))
                cm_RequestChrMoveToUniverse(U_I(ch + 4), U_I(param_1 + 1), U_I(param_2 + 2), "dummy_EINGANG_ZELLE");
        }
    }
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5a5bc0, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            entfuehren = sv_RegisterNamePlate("contact_ENTFUEHREN", U_I((unsigned)&full_data_144089c), 0x16);
            loesegeld = sv_RegisterNamePlate("contact_LOESEGELD", U_I((unsigned)&full_data_14408cc), 0x16);
        }
        if (ch != 0 && U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c) == U_I(ch + 4) &&
            *(char *)(ch + 0x1b1) != 0)
            ausbrechen = sv_RegisterNamePlate("contact_AUSBRECHEN", U_I((unsigned)&full_data_144a17c), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == entfuehren)
                ai_Unk5a53e0(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == loesegeld)
                ai_Unk5a5530(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ausbrechen)
                ai_Unk5a5910(param_1);
        }
        r = sv_RunFrame(ai_Unk5a5bc0, 0x67fff);
    }
    if (cell != 0 && ch != 0) {
        ch_Delete(cell);
        U_I(ch + 0x184) = 0;
    }
}
