/* 0x5ca560, the embezzlement privilege handler (764 B): param_1 is the message record (the AIMSG by word
   offsets: +0 the id, +2 the type 6, +4 the handle), param_2 the request record (dword +0: 3 (the message)
   or 4 (with +0x21c the owner), +0x214 the target id). The owner kind 0x4e6f80 (owner +0x166) and the
   target 0x505df0 / 0x4e6030: the kind dwords (the kind record +4 / the character record +4) must match.
   Types other than 6: "absetzen" 0x4c4540, 0x4e6600 (the character, the owner), the carrier 0x4c4a60
   (owner +4, 0x1c8, 4, 0x200000) and the line 0x4c4b00 (the handle, the character +4, -0x28, 1), return
   0x10. Type 6: the config (0x400, the globals 0x5eed68 / 0x5eed6c, 6, 3, +0x1c = the message id and the
   owner id) and the texts 0x5176d0 (0x1d98 / 0x1d99) go to the window 0x5bea50 (the kind record +8, the
   config, the two texts, 0); the returned character 0x4e6030 (+0x10 == 1) runs the same "absetzen" chain
   with -0x14 and then waits 0x4c3ae0 / 0x52fe00: the text 0x1d9b and 0x90, else the text 0x1d9a and 0x20.
   ROUGH: the record fields by offset. */

int amt_GetKind(); int amt_FindOffice(); int amt_SendOfficeCandidates(); int ai_Unk494d40();

int ai_EmbezzlementPrivilege(unsigned short *param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    char kinfo[0x18];
    int cfg[20];
    char text1[256];
    char text2[256];
    char text3[256];
    char chrrec[24];
    char buf[24];
    int owner;
    int kind;
    int i;
    int h;
    int chr;
    int v;

    if (*param_2 == 3) {
        owner = (int)param_1;
    } else {
        if (*param_2 != 4)
            return 0x60;
        owner = param_2[0x87];
        if (owner == 0)
            return 0x60;
    }
    kind = *(unsigned char *)(owner + 0x166);
    if (amt_GetKind(kind, kinfo) == 0)
        return 0x60;
    if (*(char *)((char *)param_1 + 2) != 6) {
        chr = gm_FindChar(param_2[0x85]);
        if (chr == 0)
            return 0x60;
        if (amt_FindOffice((void *)chr, buf, chrrec) == 0)
            return 0x60;
        if (*(int *)(kinfo + 4) != *(int *)(chrrec + 4))
            return 0x60;
        cm_BeginGroup("absetzen");
        amt_SendOfficeCandidates((void *)chr, (void *)owner);
        cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, 0x200000, 0);
        cm_SendRelations(*(int *)((char *)param_1 + 4), *(int *)(chr + 4), 0xffffffd8, 1);
        cm_EndGroup("absetzen");
        return 0x10;
    }
    for (i = 0; i < 0x14; i++)
        cfg[i] = 0;
    for (i = 7; i < 0xf; i++)
        cfg[i] = -1;
    *(short *)((char *)&cfg[7] + 0) = *(short *)param_1;
    if (param_2[0x87] == owner)
        *(short *)((char *)&cfg[7] + 2) = *(short *)owner;
    cfg[1] = 0x400;
    cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
    cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
    cfg[4] = 6;
    cfg[5] = 3;
    txt_Format(text1, (char *)0x1d98);
    txt_Format(text2, (char *)0x1d99);
    chr = ai_Unk5bea50(*(int *)(kinfo + 8), (int)cfg, (int)text1, (int)text2, 0);
    if (chr != 0) {
        if (amt_FindOffice((void *)chr, buf, chrrec) == 0)
            return 0x60;
        if (*(char *)(chrrec + 0x10) == 1) {
            cm_BeginGroup("absetzen");
            amt_SendOfficeCandidates((void *)chr, (void *)owner);
            h = cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, 0x200000, 0);
            cm_SendRelations(*(int *)((char *)param_1 + 4), *(int *)(chr + 4), 0xffffffec, 1);
            cm_EndGroup("absetzen");
            v = cm_GetSlotState(h);
            while (v == 0) {
                sv_PumpNet();
                v = cm_GetSlotState(h);
            }
            if (cm_GetSlotState(h) != 2) {
                while (cm_GetSlotState(h) == 0)
                    sv_PumpNet();
                txt_Format(text3, (char *)0x1d9b, *(unsigned short *)chr,
                             *(unsigned short *)chr);
                sv_MessageBox(text3, 0);
                return 0x90;
            }
        }
        txt_Format(text3, (char *)0x1d9a, *(unsigned short *)chr);
        sv_MessageBox(text3, 0);
    }
    return 0x20;
}
