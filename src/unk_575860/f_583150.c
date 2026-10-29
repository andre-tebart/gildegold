/* 0x583150, a contact entry's second line (108 B): the texts +0x30 / +0x34 (the German +0x114 / the English
   +0x11e and +0x130 / +399 by the flag +0x38): both ("%s - %s" 0x62a570), one ("%s" 0x5f8318), or empty */

void ai_Unk583150(char *e, char *out)
{
    char *a;
    char *b;

    a = (char *)U_I(e + 0x30);
    if (a) {
        b = (char *)U_I(e + 0x34);
        if (b) {
            if (U_I(e + 0x38) == 0)
                txt_Format(out, (char *)(unsigned)&full_data_62a570, a + 0x114, b + 0x130);
            else
                txt_Format(out, (char *)(unsigned)&full_data_62a570, a + 0x11e, b + 399);
            return;
        }
        if (U_I(e + 0x38) == 0) {
            txt_Format(out, "%s", a + 0x114);
            return;
        }
        txt_Format(out, "%s", a + 0x11e);
        return;
    }
    b = (char *)U_I(e + 0x34);
    if (!b) {
        *out = 0;
        return;
    }
    if (U_I(e + 0x38) == 0) {
        txt_Format(out, "%s", b + 0x130);
        return;
    }
    txt_Format(out, "%s", b + 399);
}
