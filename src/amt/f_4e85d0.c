/* 0x4e85d0, brings the 30 offices and the characters in line after an election (main.c runs it when amt_CheckOfficeColumns decided nothing, cm.c at the
   end of the council session): an office whose owner is gone (or not alive: health +8 zero) is announced free (amt_SendOfficeFree) unless it is open
   (state 3) and the owner's officeKind (+0x166) is cleared; an office not in the state 1 is announced to its living owner (amt_SendOfficeMessage); then every
   application (appliedOffice +0x168) is cleared. The tail walks the records ai_Chars + 0x168 (the appliedOffice byte, 0x21c apart), ch - 0x168 is the record */

void amt_UpdateOffices(void)
{
    int r, v, i;
    AMTCHARVIEW *c;
    char *ch;

    for (i = 0; i < 30; i++) {
        c = (AMTCHARVIEW *)gm_FindChar(amt_Table[i].owner);
        r = -1;
        if (c == 0 || c->health == 0) {
            if (amt_Table[i].state != 3)
                r = amt_SendOfficeFree(amt_Table[i].idx);
            if (c != 0 && c->officeKind != 0) {
                v = 0;
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldDelta(1, 1, 0x166, &v);
                cm_SendFieldDeltas();
            }
        } else if (amt_Table[i].state != 1)
            r = amt_SendOfficeMessage((char)amt_Table[i].idx, (char *)c, 0, 1, 0xff);
        while (r != -1 && cm_GetSlotState(r) == 0)
            sv_PumpNet();
    }
    ch = (char *)ai_Chars + 0x168;
    do {
        if (ch[0] != 0) {
            v = 0;
            cm_BeginFieldBlock(ch - 0x168, ((AMTCHARVIEW *)(ch - 0x168))->netId);
            cm_AddFieldDelta(1, 1, 0x168, &v);
            cm_SendFieldDeltas();
        }
        ch += 0x21c;
    } while ((int)ch < (int)((char *)ai_Chars + 0x168 + 768 * 0x21c));
}
