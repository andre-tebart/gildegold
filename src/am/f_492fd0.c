/* 0x492fd0, makes a random character in ai_Unk6f0478 from kind k, returns its stats | k */

/* ---- 0x492fd0..0x494020 (was unk_490830.c): a random character generator, a dice duel between two
   characters (struct AGDUEL), two helpers that pick a character for a message ---- */
/* 0x5ecf18, the pairs (a, b) the duel accepts for a side (9 records) */
static const int ag_Pairs[9][2] = {
    {0, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}
};

/* 0x5ecf60, five rows of cumulative chances */
static const float ag_Chance[5][6] = {
    {0.2f, 0.3787f, 0.5444f, 0.711f, 0.8999f, 1.0f},
    {0.19f, 0.3565f, 0.5222f, 0.6898f, 0.8666f, 1.0f},
    {0.1678f, 0.3343f, 0.5f, 0.6676f, 0.8444f, 1.0f},
    {0.1456f, 0.311f, 0.4888f, 0.6442f, 0.8222f, 1.0f},
    {0.1234f, 0.2998f, 0.4666f, 0.622f, 0.8111f, 1.0f}
};
const AIREC10 ai_Unk5ecfd8[7] = { { 0, 10, 20.0f, 40.0f }, { 1, 13, 40.0f, 60.0f }, { 2, 16, 60.0f, 80.0f }, { 3, 19, 80.0f, 100.0f }, { 4, 23, 100.0f, 120.0f }, { 5, 27, 120.0f, 140.0f }, { 6, 32, 140.0f, 160.0f } };  /* 0x5ecfd8 */
const int ai_Unk5ed100[16] = { 1, 5, 7, 11, 13, 17, 19, 23, 745, 749, 751, 755, 757, 761, 763, 767 };  /* 0x5ed100, random steps */
const int ai_Unk5ed140[16] = { 1, 3, 5, 7, 11, 13, 17, 19, 237, 239, 243, 245, 249, 251, 253, 255 };  /* 0x5ed140, random steps */
const int ai_Unk5ed180[] = { 1, 3, 5, 7, 11, 13, 17, 19, 23, 29 };  /* 0x5ed180, ring steps */
const int ai_Unk5ed1a8 = 31;  /* 0x5ed1a8, the last of the ring sizes before it */
const G9BALL ai_Unk5edfe0[3] = { { 2000.0f, 3000.0f, 0.0599999987f }, { 2450.0f, 3850.0f, 0.0649999976f }, { 3600.0f, 5950.0f, 0.075000003f } };  /* 0x5edfe0 */
const float ai_Unk5eee08 = 7874.0f;  /* 0x5eee08, 7874 */

unsigned int ai_Unk492fd0(unsigned int k)
{
    float t[15] = {0.2f, 0.55f, 0.85f, 0.95f, 1.0f, 0.1f, 0.4f, 0.7f, 0.9f, 1.0f, 0.05f, 0.25f, 0.55f, 0.8f, 1.0f};
    float r;
    int idx, i, base, mul;
    unsigned int v, ret;
    char c;

    r = util_RandomF();
    if (k == 1)
        idx = 0;
    else
        idx = (k != 2) + 1;
    for (i = 0; i < 5; i++) {
        if (r <= t[idx * 5 + i]) {
            k = i + 1;
            break;
        }
    }
    if (idx == 1)
        v = util_Random(3);
    else {
        v = util_Random(2);
        if (idx == 0)
            v = v & 0xffff;
        else
            v = idx - (v & 0xffff);
    }
    r = util_RandomF();
    ret = k;
    for (i = 0; i < 5; i++) {
        if (r <= t[v * 5 + i]) {
            ret = (i + 1) * 0x100;
            break;
        }
    }
    memset(&ai_Unk6f0478, 0, sizeof(ai_Unk6f0478));
    ai_Unk6f0478.unk009 = (unsigned char)util_Random(2);
    if (ai_Unk6f0478.unk009 == 0)
        strncpy(ai_Unk6f0478.name, ai_Unk143ac5c[util_Random(0xbf)], 15);
    else
        strncpy(ai_Unk6f0478.name, ai_Unk143af70[util_Random(0x70)], 15);
    switch (k) {
    case 1:
        base = 0x15;
        mul = 0x2a;
        break;
    case 2:
        base = 0x15;
        mul = 0x3f;
        break;
    case 3:
        base = 0x3f;
        mul = 0x3f;
        break;
    case 4:
        base = 0x69;
        mul = 0x3f;
        break;
    case 5:
        base = 0xa8;
        mul = 0x54;
        break;
    default:
        base = k;
        mul = k;
    }
    for (i = 0; i < 5; i++) {
        c = (char)util_Random(mul);
        ai_Unk6f0478.stat[i] = c + (char)base + 1;
    }
    ai_Unk6f0478.kind = 10;
    return ret | k;
}
