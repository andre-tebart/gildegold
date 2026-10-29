/* 0x4b2ed0, leaked name (its message): text k is a date "DD.MM.YYYY..." (DD / MM 00: unknown); *year = YYYY -
   1400; mode 4: name = the text, mode 2: name = "<town>, d <month> y" (as far as known); mode 1: the first 10
   characters become the key hst_Key
   @deep read the original in full: fixed strlen(buf) != 10 (was < 10; original cmp 0xa/je, longer texts take the error path) and moved *year after the d/m normalization (187 -> 183 asm). Types tried: short y 194, m+0x4f 191, both 196, block-scoped d/m worse; all 120 declaration orders and s/t at function scope leave the slots unchanged. Remaining (allocation): frame 0x17f4 vs 0x17f8 because d/m sit in dead slots ([ebp+0xb]/[ebp-0x4]) where the original gives them -0x28/-0x27, shifting known/tmp by 4 and rotating the G12 temp/name slots */

/* G12_COPY2: G12_COPY with destination and source in variables */
#define G12_COPY2(dst, src, size) {                                 \
    int _cn = (size); void *_cs = (src); void *_cp = (dst);         \
    __asm mov edi, _cp                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

int hst_ParseDate(int k, unsigned char mode, int *year, char *name)
{
    char buf[0x17d0];
    char tmp[16];
    char d, m;
    unsigned short y;
    int known;

    known = 0;
    if (!txt_Loaded || hst_TextFirst == -1)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(tmp, 16);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0)
            return 0;
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *t = buf;

        while ((*t++ = *s++) != 0)
            ;
    }
    if (strlen(buf) != 10) {
        sprintf(buf, "hst_ParseDate() failed: Label %i is not a <<DATE>> ", k - hst_TextFirst);
        return 0;
    }
    G12_COPY2(tmp, buf, 2);
    d = (char)atoi(tmp);
    G12_COPY2(tmp, buf + 3, 2);
    m = (char)atoi(tmp);
    G12_COPY2(tmp, buf + 6, 4);
    y = (unsigned short)atoi(tmp);
    if (d == 0) {
        d = 1;
        known = 1;
    }
    if (m == 0) {
        m = 1;
        known = 2;
    }
    *year = atoi(tmp) - 1400;
    if (mode & 4) {
        G12_COPY2(name, buf, strlen(buf));
    } else if (mode & 2) {
        switch (known) {
        case 0:
            txt_FormatString(name, "%s, %i %s %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, d, (unsigned char)m + 0x4f, y);
            break;
        case 1:
            txt_FormatString(name, "%s, %s %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, (unsigned char)m + 0x4f, y);
            break;
        case 2:
            txt_FormatString(name, "%s, %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, y);
            break;
        }
    }
    if (mode & 1) {
        G12_ZERO_G(hst_Key, 16);
        G12_COPY2(hst_Key, buf, 10);
    }
    return 1;
}
