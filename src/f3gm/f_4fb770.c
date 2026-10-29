/* 0x4fb770, the number of the first head icon (of the heads 0 .. 3) that exists for the head name (split at the last '_' as in 0x4fb620; "zigeunermeister" uses the form without the suffix), 0 when none. ROUGH */

int unk_Unk4fb770(char *name)
{
    char buf[128], suffix[128], icon[128];
    char *u;
    int h, i, r;

    strcpy(buf, name);
    u = strrchr(buf, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    r = 0xff;
    for (h = 0; h < 4; h++) {
        if (r != 0xff)
            return r;
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", buf, h, suffix);
        if (strstr(buf, "zigeunermeister") != 0)
            sprintf(icon, "%s_HEAD%i_ICON.bmp", buf, h);
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                r = i;
                break;
            }
        }
    }
    if (r != 0xff)
        return r;
    return 0;
}
