/* 0x53dee0, sends the message 0x11 with a quick jump (name `jump`, at most 0x30 characters: else "he_MessageBoxQuickjump(): .." 0x626700, the function's own name) and the clock values a5 / a6 / a7 (+0x58 / +0x5c / +0x60) (ROUGH) */

void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2)
{
    char msg[0xf8];
    char buf[0x1f90];
    int n;
    unsigned char b;

    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x11;
    HM_I(msg, 8) = a1;
    HM_I(msg, 0xc) = a2;
    HM_B(msg, 0x36) = 9;
    if (jump != 0) {
        if (strlen(jump) + 1 > 0x30)
            err_Message("he_MessageBoxQuickjump(): contactname is too long!");
        else
            strcpy(msg + 0x68, jump);
    }
    HM_I(msg, 0x58) = a5;
    b = (unsigned char)fl | 2;
    HM_I(msg, 0x5c) = a6;
    HM_B(msg, 0x9c) = b;
    HM_I(msg, 0x60) = a7;
    strcpy(buf, text);
    n = strlen(text) + 1;
    if (text2 == 0 || *text2 == 0) {
        ai_Unk4c4bc0(msg, n, buf);
        return;
    }
    memcpy(buf + n, text2, strlen(text2) + 1);
    b |= 0x10;
    HM_B(msg, 0xa0) = b;
    ai_Unk4c4bc0(msg, strlen(text2) + n + 1, buf);
}
