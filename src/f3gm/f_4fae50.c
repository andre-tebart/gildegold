/* 0x4fae50, a leader (the record b, its character at +0x27) takes a share from the other characters: with the chance 0.25 * the other character's byte +0x1b0 * (1 + 0.0125 * the 0x5092e0 skill of b) (times 0.8 / 0.7 / 0.6 when the groups differ, byte +0xc against the record 2's rate), the other pays 0x4c4830 (-1, .., about the 0x50a640 value * 160 / the best value): the loyalty float at +0x1d0 moves by 0.05 (same group) or -0.05 (0x4c4ac0), the master gets 0x4c4b00. out[0] = the number of payers, out[1] = the sum; out[2] = an object of the leader. Returns 1, or 0 when b has no character. ROUGH, the amount (the lost __ftol argument) is a guess */

int unk_Unk4fae50(char *b, int *out)
{
    int n, sum, i, a, v, top;
    short id;
    int r[9];
    char *c;
    char *u;
    float f, p, lo;
    unsigned uid;

    sum = 0;
    n = 0;
    gm_FindRichest(&id, &top);
    if (*(unsigned short *)(b + 0x27) == 0xffff)
        return 0;
    uid = *(unsigned short *)(b + 0x27);
    gm_GetSkill(ai_Chars[uid], 4);
    gv_GetTableRecord(2, r);
    f = (float)gm_BuildingSkill(b, 9, 1) * 0.0125f + 1.0f;
    u = ai_Chars[uid];
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[2] != 6 && (unsigned char)c[2] != 7 && (unsigned char)c[2] != 8 && (unsigned char)c[2] < 10 && *(int *)(c + 4) != *(int *)(u + 4)) {
            if (c[0xc] == u[0xc]) {
                p = (float)(unsigned char)c[0x1b0] * 0.25f * f;
                lo = 0.05f;
            } else {
                p = (float)(unsigned char)c[0x1b0] * 0.25f * f;
                if (r[6] == 2)
                    p = (float)(p * 0.8);
                else if ((unsigned char)u[0xc] == r[6])
                    p = (float)(p * 0.7);
                else
                    p = (float)(p * 0.6);
                lo = -0.05f;
            }
            {
                float x = util_RandomF2();
                if (x + x <= p) {
                    a = I50A640(*(short *)c);
                    if (0.0 < ((float)a / (float)top) * 160.0)
                        a = I50A640(*(short *)c);
                    v = (int)(((float)a / (float)top) * 160.0);
                    sum += v;
                    cm_SendMoveGoodsUnchecked(-1, *(int *)(c + 4), ai_OwnPlayer, v);
                    n++;
                    if (*(float *)(c + 0x1d0) < 0.95 && 0.05 < *(float *)(c + 0x1d0))
                        ai_Unk4c4ac0(*(int *)(c + 4), 0x1d0, lo);
                    if (out[2] != 0) {
                        gm_Mix(u, c, 4);
                        cm_SendRelations(*(int *)(out[2] + 4), *(int *)(c + 4), (int)lo, 1);
                    }
                }
            }
        }
    }
    out[0] = n;
    out[1] = sum;
    return 1;
}
