/* 0x506180, the ranking of the players: collects the ids of up to 8 characters of the kinds 5 / 6 / 7 into out (records of 0x24 bytes, the id first; *count of them), and fills per record: +0x18 the sum of the five skill bytes (+0x80) / 1260, +0xc the byte +0xd / 9, +0x10 the two 0x4e6f80 levels (jobs +0x166 / +0x169) / 13, +0x14 the mean opinion (0x50c550 over the other players and townsmen) / 100, +4 the 0x50a640 value (at least 1; the maximum in 0x506180's local), +8 = that / the maximum, +0x1c the weighted sum (0.25 * +0x18 + 0.3 * +8 + 0.15 * (+0xc + +0x10 + +0x14)), +0x20 its truncation; the line is formatted with 0x61ac40. ROUGH, the lost float arguments are guessed */

float gm_Liking();                     /* 0x50c550 */
void ai_RestartMission(struct G13MIS *m);                     /* 0x4b3a90 */
void he_Unk53ed40();                     /* 0x53ed40 */
char ai_Unk61ac40[] = "player %i: sum:%f points:%i verm\366gen:%i vermoegen_trend:%f ansehen:%f talente:%f amt:%f";  /* 0x61ac40, a log format */

void gm_Unk506180(int *count, unsigned short *out)
{
    int n, i, k, mx, sum, cnt, j, v;
    char *c;
    unsigned short *r;
    float *f;
    float acc;
    int tmp[4];
    unsigned u1;
    char buf[256];
    unsigned short idx;

    n = 0;
    mx = 0;
    idx = 0;
    r = out;
    c = ai_Chars[0] + 2;
    do {
        if (7 < n)
            break;
        if (*c == 6 || *c == 7 || *c == 5) {
            *r = idx;
            n++;
            r += 0x12;
        }
        c += 0x21c;
        idx++;
    } while ((int)c < (unsigned)((unsigned)ai_Chars + 0x65402));
    *count = n;
    if (n != 0) {
        r = out;
        for (i = 0; i < *count; i++, r += 0x12) {
            c = ai_Chars[*r];
            sum = 0;
            for (k = 0; k < 5; k++)
                sum += (unsigned char)c[0x80 + k];
            *(float *)(r + 0xc) = (float)sum * 0.0007936508f;
            *(float *)(r + 6) = (float)(unsigned char)c[0xd] * 0.11111111f;
            amt_GetKind(c[0x166], tmp);
            u1 = tmp[2] & 0xff;
            amt_GetKind(c[0x169], tmp);
            *(float *)(r + 8) = ((float)(tmp[2] & 0xff) + (float)u1) * 0.07692308f;
            cnt = 0;
            acc = 0.0f;
            for (j = 0; j < 0x300; j++) {
                char *o = ai_Chars[j];
                if ((unsigned)j != *r && *(short *)o != -1 && (o[2] == 6 || o[2] == 7 || o[2] == 5 || o[2] == 4)) {
                    acc = (float)(gm_Liking(*r, j) + acc);
                    cnt++;
                }
            }
            *(float *)(r + 10) = (float)((acc * 0.01) / (float)cnt);
            v = (int)gm_GetWorth(*r);
            if (v < 2)
                v = 1;
            else
                v = (int)gm_GetWorth(*r);
            *(int *)(r + 2) = v;
            if (mx < v)
                mx = v;
        }
        f = (float *)(out + 6);
        for (i = 0; i < *count; i++, f += 9) {
            float t = f[-2];
            float q = (float)(int)t / (float)mx;
            f[-1] = q;
            f[4] = f[3] * 0.25f + q * 0.3f + (f[0] + f[1] + f[2]) * 0.15f;
            f[5] = (float)(int)f[4];
            sprintf(buf, ai_Unk61ac40, i, (double)f[5], f[4], t, (double)q, (double)f[2], (double)f[3], (double)f[1]);
        }
    }
}
