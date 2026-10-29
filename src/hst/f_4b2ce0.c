/* 0x4b2ce0, leaked name (its message): writes text k (or a string above 0x4800) to out (0x5176d0) with every
   label "[text#name]" replaced by its text */

void hst_ParseText_Real(char *out, int k)
{
    char buf[0x17d0];
    char err[0x100];
    char *open, *hash, *p;

    open = 0;
    hash = 0;
    if (!txt_Loaded)
        return;
    AI_ZERO(buf, 0x17d0);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return;
        if (strlen(txt_Ptrs[k]) > 0x17c0)
            return;
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    p = buf;
    while (*p != 0) {
        if (*p == '[') {
            if (open != 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            open = p;
        } else if (*p == '#') {
            if (open == 0 || hash != 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            hash = p;
        } else if (*p == ']') {
            if (open == 0 || hash == 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            memmove(hash, p + 1, strlen(p + 1) + 1);
            memmove(open, open + 1, strlen(open + 1) + 1);
            p = hash - 1;
            open = 0;
            hash = 0;
            continue;
        }
        p++;
    }
    if (open != 0 || hash != 0)
        return;
    txt_FormatString(out, "%s", buf);
}
