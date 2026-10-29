/* 0x4fdfd0, the character statistics (0x91b500, 0x68 bytes): [0] the number of active persons of kind < 10, a histogram of 0x505e30 (i) / 10 (clamped, [1 ..]), the float [9] the share of children below their lifetime (0x4fd7e0, in percent at the end), the sums of the healths 0x4fc820 / 0x4fc890 per kind ([10 ..] / [18 ..]) turned into averages. ROUGH */

void unk_Unk4fdfd0(void)
{
    int *s = (int *)gm_CharacterStatistics;
    float *f = (float *)gm_CharacterStatistics;
    float v;
    int i, k;
    char *c;

    memset(gm_CharacterStatistics, 0, 0x68);
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0 && (unsigned char)c[2] < 10) {
            s[0]++;
            v = gm_CharAge(i) * 0.1f;
            if (v >= 7.0f)
                v = 6.0f;
            if (v < 0.0f)
                v = 0.0f;
            k = (int)v;
            s[1 + k]++;
            if (unk_Unk4fd7e0(i) != 0)
                f[9] += 1.0f;
            f[10 + (unsigned char)c[2]] = gm_CharacterHealth((short *)c) + f[10 + (unsigned char)c[2]];
            f[18 + (unsigned char)c[2]] = unk_Unk4fc890((short *)c) + f[18 + (unsigned char)c[2]];
        }
    }
    for (i = 18; i < 26; i++) {
        f[i - 8] = (f[i - 8] / f[i]) * 100.0f;
        f[i] = f[i] / (float)s[i - 17];
    }
    f[9] = (f[9] / (float)s[0]) * 100.0f;
}
