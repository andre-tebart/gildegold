/* 0x50cdc0, the standing of the building b with the persons ids[0 .. n - 1]: the mean of their 0x4fc8e0 weights (+0.3 for those whose title matches the building's), added to the owner's factor. ROUGH */

float gm_Unk50cdc0(unsigned char *b, int n, int *ids)
{
    char *p, *c;
    int i, cnt, id;
    float a, s, m, f, t;

    p = ai_BTypes + *b * 0x24d;
    i = 0;
    a = 0.0f;
    cnt = 0;
    s = 0.0f;
    m = 0.0f;
    gm_BuildingSkill(b, 1, 1);
    c = gm_FindCharacterByMaster((int)b);
    if (c == 0)
        c = ai_Chars[*(unsigned short *)(b + 0x27)];
    if (c[2] == 6 || c[2] == 7)
        f = (unsigned char)c[0x81] * 0.000992063492063492;
    else
        f = 0.0f;
    if (0 < n) {
        do {
            id = ids[i];
            if (id != -1 && (c = gm_FindChar(id)) != 0) {
                s = (float)(unk_Unk4fc8e0((short *)c) + s);
                if (p[0x22f] == c[0x165])
                    m = m + 0.3;
                cnt++;
            }
            i++;
        } while (i < n);
        if (cnt != 0) {
            a = s * (1.0f / (float)cnt);
            m = (1.0f / (float)cnt) * m;
        }
    }
    t = gm_Unk50c980(*(unsigned short *)(b + 0x27), n, ids);
    f = (float)(f + 0);
    if ((b[0x5b] & 0x10) != 0)
        f = f + 0.1;
    return (m + a) * ((t - 0.5) * 0.25 + f + (*(int *)(b + 0x41) - 2) * 0.1);
}
