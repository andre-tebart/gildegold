/* 0x519010, the language number of the name s (case-insensitive, one of the five of the table 0x5ee978), 0xffff when unknown; kept in 0x62364c */

int ls_LanguageOf(char *s)
{
    char buf[0x40];
    int i;

    strcpy(buf, s);
    _strupr(buf);
    ls_Language = 0xffff;
    for (i = 0; i < 5; i++) {
        if (strcmp(ls_LanguageNames[i], buf) == 0) {
            ls_Language = i;
            return i;
        }
    }
    return 0xffff;
}
