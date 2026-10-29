/* 0x5830f0, a contact entry's name line (90 B): the record's +0x2c text (the German +0x253 / the English +0x27b
   by the flag +0x38) with the two name parts +4 / +0x14 through 0x5176d0 ("%s %s %s %s" 0x623594), or just the
   two parts ("%s %s" 0x60c7b8) */

void ai_Unk5830f0(char *e, char *out)
{
    char *p;

    p = (char *)U_I(e + 0x2c);
    if (p) {
        if (U_I(e + 0x38) == 0)
            txt_Format(out, (char *)(unsigned)"%s %s %s", p + 0x253, e + 4, e + 0x14);
        else
            txt_Format(out, (char *)(unsigned)"%s %s %s", p + 0x27b, e + 4, e + 0x14);
        return;
    }
    txt_Format(out, (char *)(unsigned)"%s %s", e + 4, e + 0x14);
}
