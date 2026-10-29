/* 0x4fb620, fills the four head numbers (+0x34 .. +0x40) of a head table record whose first is still 0xff: from the name (+4) split at the last '_' into name and suffix, the icon "%s_HEAD%i_%s_ICON.bmp" (or "%s_HEAD%i_ICON.bmp") is looked up in the icon names 0x6f1b94 (0x30 bytes each, 0x6f1b90 of them) for head 0 .. 3 */

void unk_Unk4fb620(unsigned *rec)
{
    char name[128], suffix[128], icon[128];
    char *u;
    int h, i, *out;

    out = (int *)(rec + 0xd);
    if (*out != 0xff)
        return;
    strcpy(name, (char *)(rec + 1));
    u = strrchr(name, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    for (h = 0; h < 4; h++, out++) {
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
        *out = 0xff;
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                *out = i;
                goto next;
            }
        }
        sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
        *out = 0xff;
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                *out = i;
                break;
            }
        }
    next:;
    }
}
