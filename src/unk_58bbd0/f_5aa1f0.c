/* 0x5aa1f0, the info panel of a building (700 B): the kind record 0x7e3f30 + the byte of the record at +0x16c of
   param_1 * 0x24d; the title text +0x18c (0x4731d0), a text of the record 0x770b80 + 0x6f25b8 * 0x21c (0x5143d0)
   into the +5 control, the slider 0x46a960 at +4, the record param_2 (x, y, the character at +2, the window +0x16):
   the byte +0x164 of param_1 suppresses the status text, the int +0x174 is compared with the religion / race bytes
   +0x22f / +0x230 of the kind record (the texts 0x1a39 / 0x1a3a / 0x1a3b / 0x1a3c); with param_3 two lines
   0x52eaa0 (the bytes +0x83 / +0x82) and the slider 0x475cb0 = the opinion 0x50c520 in percent */

void ai_Unk5aa1f0(int param_1, int *param_2, int param_3)
{
    char text[128];
    int kind;
    int ch;
    int v;
    int id;

    kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)U_I(param_1 + 0x16c) * 0x24d;
    d2_SetForm(param_2[0x16], 3);
    id = d2_WinOpenPic((short)U_W(param_1) + 2, (short)param_2[1], U_I(param_1 + 0x18c), U_I((unsigned)&d2_LastWindow));
    param_2[7] = id;
    io_CharacterName(U_W(param_1), 1, 1, text);
    d2_SetObjText(param_2[5], text);
    U_I((unsigned)&d2_Objs + 0x5c + param_2[5] * 0x2ec) = 1;
    U_W((unsigned)&d2_Objs + 0x14 + param_2[5] * 0x2ec) = 0xa0;
    id = d2_WinOpenSlider(param_2[0] - 8, param_2[1] + 0x32, 0x40, 0, 100, 0x75, 0x42, U_I((unsigned)&d2_LastWindow));
    param_2[4] = id;
    U_I((unsigned)&d2_Objs + 0x3c + id * 0x2ec) = 1;
    *(unsigned char *)((char *)param_2 + 0x20) = 1;
    U_I((unsigned)&d2_Objs + 0x2e8 + param_2[3] * 0x2ec) = param_1;
    v = U_I(param_1 + 0x174);
    ch = param_2[2];
    if (*(unsigned char *)(param_1 + 0x164) == 0) {
        if (v == 0) {
            if (*(unsigned char *)(ch + 0x165) == *(unsigned char *)(kind + 0x230))
                txt_Format(text, "%s", 0x1a3b);
            else if (*(unsigned char *)(ch + 0x165) == *(unsigned char *)(kind + 0x22f))
                txt_Format(text, "%s", 0x1a3c);
        } else if (*(unsigned char *)(ch + 0x165) == *(unsigned char *)(kind + 0x230)) {
            txt_Format(text, (char *)(unsigned)"%i %s", v, 0x1a39);
        } else if (*(unsigned char *)(ch + 0x165) == *(unsigned char *)(kind + 0x22f)) {
            txt_Format(text, (char *)(unsigned)"%i %s", v, 0x1a3a);
        }
        d2_SetObjText(param_2[6], text);
        U_I((unsigned)&d2_Objs + 0x5c + param_2[6] * 0x2ec) = 1;
        U_W((unsigned)&d2_Objs + 0x14 + param_2[6] * 0x2ec) = 0xa0;
    }
    if (param_3 != 0) {
        sv_Unk52eaa0(param_2[0], param_2[1] + 100, *(unsigned char *)(param_1 + 0x83), 0x4df);
        sv_Unk52eaa0(param_2[0], param_2[1] + 0x74, *(unsigned char *)(param_1 + 0x82), 0x4df);
    }
    if (param_2[4] != -1) {
        v = gm_Opinion(U_W((unsigned)&ai_PlayerId), *(unsigned short *)param_2[2]);
        d2_SetObjValue(param_2[4], 0, 100, (int)((v + 128.0) * 100.0 * (1.0 / 255.0)));
    }
}
