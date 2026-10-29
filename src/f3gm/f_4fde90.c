/* 0x4fde90, loads the icon names: counts the 0x80-byte records of "<root>Character_Names.dat" (0x6f1b90 of them), allocates 0x30 bytes per name (0x6f1b94) and copies each name; resets the three flag bytes at 0x6f0454 */

void unk_Unk4fde90(void)
{
    char path[260], rec[260];
    char *s, *d;
    FILE *f;
    char ch;

    ai_Unk6f0454[1] = (char)0xff;
    ai_Unk6f0454[0] = 0;
    ai_Unk6f0454[2] = 0;
    ai_Unk6f1b90 = 0;
    sprintf(path, "%sCharacter_Names.dat", ai_Unkf8ef80);
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            ai_Unk6f1b90++;
        } while ((f->_flag & 0x10) == 0);
        fclose(f);
    }
    ai_Unk6f1b94 = (char *)mem_Alloc(ai_Unk6f1b90 * 0x30, &ai_Unk60c1ec);
    ai_Unk6f1b90 = 0;
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            s = rec;
            d = ai_Unk6f1b94 + ai_Unk6f1b90 * 0x30;
            ai_Unk6f1b90++;
            do {
                ch = *s++;
                *d++ = ch;
            } while (ch != 0);
        } while ((f->_flag & 0x10) == 0);
        fclose(f);
    }
}
