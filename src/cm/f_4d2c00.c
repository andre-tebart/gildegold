/* 0x4d2c00, the row of 6 floats of ai_Unk9300a0 for the side (kind 0 .. 5 by the fight flags +0x7c bit 0 / 1 / 2 and the side
   being the attacker ai_Unk6f0cec or the defender ai_Unk6f0ce8, else 6) and the strength level 0 .. 4 (the ratio of the two
   sides' strengths 0x4cdcf0 against the limits 5ebc1c / 5ecebc / 5eb94c / 5eb7ac), lowered by the number of "sp_CONQUER"
   flags the attacker owns. Rough draft. */

float *ai_Unk4d2c00(int side)
{
    unsigned char fl = *(unsigned char *)(ai_Unk6f0d04 + 0x7c);
    int kind = 6;
    int lvl, cnt = 0, i;
    float a = 0.0f, b = 0.0f, r;
    char *o;

    if ((fl & 1) && ai_Unk6f0cec == side)
        kind = 0;
    else if ((fl & 1) && ai_Unk6f0ce8 == side)
        kind = 1;
    else if ((fl & 2) && ai_Unk6f0cec == side)
        kind = 2;
    else if ((fl & 2) && ai_Unk6f0ce8 == side)
        kind = 3;
    else if ((fl & 4) && ai_Unk6f0cec == side)
        kind = 4;
    else if ((fl & 4) && ai_Unk6f0ce8 == side)
        kind = 5;
    for (i = 0x80; i < 0xc0; i += 4) {
        if (*(int *)(ai_Unk6f0d04 + i) != -1) {
            o = ai_Unk4ce130(*(int *)(ai_Unk6f0d04 + i));
            if (o[8] != 0 && o[8] != 4)
                a = ai_Unk4cdcf0(o) + a;
        }
    }
    for (i = 0xc0; i < 0x100; i += 4) {
        if (*(int *)(ai_Unk6f0d04 + i) != -1) {
            o = ai_Unk4ce130(*(int *)(ai_Unk6f0d04 + i));
            if (o[8] != 0 && o[8] != 4)
                b = ai_Unk4cdcf0(o) + b;
        }
    }
    for (i = 0; i < ai_Unk6f0d58; i++) {
        if (strstr(ai_Unk935980[i], "sp_CONQUER") != 0) {
            float v[4];

            d3_PointToWorld(ai_Unk935980[i], ai_Unk935980[i] + 0x4c, v);
            o = *(char **)(ai_Unk935980[i] + 0x200);
            if (o != 0 && *(int *)(o + 0x16c) == ai_Unk6f0cec)
                cnt++;
        }
    }
    if (side == ai_Unk6f0cec)
        r = b / a;
    else
        r = a / b;
    if (!(r < ai_Unk5ebc1c))
        lvl = 4;
    else if (!(r < ai_Unk5ecebc))
        lvl = 3;
    else if (!(r < ai_Unk5eb94c))
        lvl = 2;
    else if (!(r < 0.5f))
        lvl = 1;
    else
        lvl = 0;
    if (side == ai_Unk6f0ce8) {
        if (cnt == 1)
            lvl = cnt;
        else if (cnt == 2 || cnt == 3)
            lvl = 0;
    }
    return &ai_Unk9300a0[(kind * 5 + lvl) * 6];
}
