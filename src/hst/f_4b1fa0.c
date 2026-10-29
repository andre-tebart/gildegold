/* 0x4b1fa0, leaked name: copies text k (a text number, or a string above 0x4800) to out with every label
   "[name#text]" replaced by its text; the error messages are written to a local buffer only */

extern int txt_Loaded;                  /* 0xfe7c20 */
extern char *txt_Ptrs[0x4800];          /* 0x143a1c0 */

int hst_ParseText_Fake_1st_Pass(char *out, int k)
{
    char buf[0x17d0];
    char err[0x100];
    char *open, *hash, *p;

    open = 0;
    hash = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Label %i is too long", k);
            return 0;
        }
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
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            open = p;
        } else if (*p == '#') {
            if (open == 0 || hash != 0) {
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            hash = p;
        } else if (*p == ']') {
            if (open == 0 || hash == 0) {
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            memmove(p, p + 1, strlen(p + 1) + 1);
            memmove(open, hash + 1, strlen(hash + 1) + 1);
            p += open - hash - 1;
            open = 0;
            hash = 0;
            continue;
        }
        p++;
    }
    if (open != 0 || hash != 0) {
        sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
        return 0;
    }
    sprintf(out, "%s", buf);
    return 1;
}
