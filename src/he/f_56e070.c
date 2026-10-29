/* 0x56e070, the scene action of the research event 0x98 (ROUGH, the selection array of the original is not recovered): only for a building of the kind 0x1e (the kind record 0x7e3f30 + kind * 0x24d); bools: a task running 0x24b / 0x256 / 0x266 with its +0x1c = 0, the building's +0x5c above 10, a running effect in the +0x65 record (six 8-byte entries: int at +0xc, float at +0x10 above 0.2); a random pick of five candidates (start random(4) + 1) drives the action: 1 a random job 0x4c3ba0 / 0x4c3c00 (1, 0x1c / 0x20) / 0x4c49a0 and 0x5a9d60; 2 a task 0x4c3d00 (1, 0x5c, random) and 0x5a9ca0; 3 0x56fbc0 (c, random(3) + 1) and 0x5a9d00; 4 an effect from the +0x65 record reduced by a random 0.05 .. 0.15 (0x4ecc30 / 0x5a9dd0); the pick goes to +0xac of ev */

void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldDelta(int size, int n, short field, void *v);
void cm_SendFieldDeltas(void);
int ai_Unk5a9d60();
int ai_Unk5a9ca0();
int ai_Unk5a9d00();
int ai_Unk5a9dd0();
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);

void he_Unk56e070(char *c, int ev, unsigned char *b, int task)
{
    int *rec;
    int h[3];
    int f1, f2, i, n, pick, sel;
    float fv;

    if (*(unsigned char *)((unsigned)&ai_BTypes + *b * 0x24d) != 0x1e)
        return;
    sel = 0;
    f1 = 0;
    f2 = 10 < (char)b[0x5c];
    rec = *(int **)(b + 0x65);
    if (task != 1) {
        for (i = 0; i < 6; i++) {
            if (*(int *)((char *)rec + 0xc + i * 8) != 0 && 0.2 < *(float *)((char *)rec + 0x10 + i * 8)) {
                f1 = 1;
                break;
            }
        }
    }
    h[0] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x24b);
    if (h[0] != 0 && *(int *)(h[0] + 0x1c) == 0)
        f2 = 1;
    h[1] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x256);
    if (h[1] != 0 && *(int *)(h[1] + 0x1c) == 0)
        f2 = 1;
    h[2] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x266);
    if (h[2] != 0 && *(int *)(h[2] + 0x1c) == 0)
        f2 = 1;
    pick = (util_Random(4) & 0xffff) + 1;
    for (i = 0; i < 5; i++) {
        n = i + 1;
        if (pick == 1 && f2)
            sel = n;
        else if (pick == 2 && f1)
            sel = n;
        else if (pick == 3 && 1)
            sel = n;
        else if (pick == 4 && f1)
            sel = n;
        else if (pick == 5)
            sel = n;
        if (sel)
            break;
        pick = pick % 5 + 1;
    }
    switch (sel) {
    case 1:
        {
            int t = (util_Random(3) & 0xffff);
            int v = (util_Random(4) & 0xffff) + 3;
            cm_BeginFieldBlock(h[0], *(int *)(h[0] + 2));
            cm_AddFieldDelta(4, 1, 0x1c, &t);
            cm_AddFieldDelta(4, 1, 0x20, &v);
            cm_SendFieldDeltas();
            ai_Unk5a9d60((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b, h[0]);
        }
        break;
    case 2:
        {
            int t = (util_Random(3) & 0xffff);
            int v = -4 - t;
            cm_BeginFieldBlock(b, *(int *)(b + 1));
            cm_AddFieldValues(1, 1, 0x5c, &v);
            cm_SendFieldDeltas();
            ai_Unk5a9ca0((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b);
        }
        break;
    case 3:
        he_Unk56fbc0(c, (util_Random(3) & 0xffff) + 1);
        ai_Unk5a9d00((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b);
        break;
    case 4:
        {
            int k = util_Random(6) & 0xffff;
            for (i = 0; i < 6; i++) {
                if (*(int *)((char *)rec + 0xc + k * 8) != 0 && 0.2 < *(float *)((char *)rec + 0x10 + k * 8)) {
                    fv = *(float *)((char *)rec + 0x10 + k * 8) - (util_RandomF() * 0.1f + 0.05f);
                    ahm_SendSetPlaceValue(c, *(int *)((char *)rec + 0xc + k * 8), fv);
                    ai_Unk5a9dd0((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b,
                                 *(int *)((char *)rec + 0xc + k * 8));
                    break;
                }
                k = (k + 1) % 6;
            }
        }
        break;
    }
    *(int *)(ev + 0xac) = sel;
}
