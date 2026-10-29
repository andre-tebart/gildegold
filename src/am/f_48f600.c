/* 0x48f600, decide of action 0x48 */

int ai_Unk48f600(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIRECX rec[15];
    int st[3];
    int r;

    if (b)
        goto fail;
    if (a != 0x48 && a != 0)
        goto fail;
    memset(rec, 0, sizeof(rec));
    rec[0].item = 0x2b8;
    rec[1].item = 0x2bb;
    rec[2].item = 0x2bc;
    rec[3].item = 0x2bf;
    rec[4].item = 0x2c2;
    rec[5].item = 0x2c3;
    rec[6].item = 0x2bd;
    rec[7].item = 0x2c1;
    rec[8].item = 0x2c4;
    rec[9].item = 0x2c5;
    rec[10].item = 0x2c6;
    rec[11].item = 0x2ba;
    rec[12].item = 0x2c0;
    rec[13].item = 0x2be;
    rec[14].item = 0x2b9;
    ai_Unk4a5760(p, 15, rec, st);
    if (st[0] == 6 && !st[2])
        goto fail;
    if (st[1] == 15)
        goto fail;
    if (st[0] < 6) {
        if (!st[2]) {
            if (ai_Time.year < 0x14)
                goto fail;
            if (p->unk00d <= 4)
                goto fail;
            if (ai_Time.year % 8 != (p->netId & 7))
                goto fail;
            if (p->flags1cc & 0x40000000)
                goto fail;
            r = util_Random(15);
            while (rec[r].unk04)
                r = (r + 1) % 15;
            o1->kind = 2;
            o1->unk08 = 1;
            o1->unk04 = rec[r].item;
            o1->unk14 = 0.05f;
            o1->unk10 = 1;
            o2->kind = 0;
            return 0x48;
        }
    } else if (!st[2])
        goto fail;
    r = util_Random(15);
    while (rec[r].unk04 < 1)
        r = (r + 1) % 15;
    o1->kind = 1;
    o1->unk08 = 1;
    o1->unk04 = rec[r].unk08;
    o1->unk14 = 0.05f;
    o1->unk10 = 3;
    o2->kind = 0;
    o2->unk04 = -1;
    o2->unk08 = -1;
    if (r < 6) {
        unsigned short id;
        int flags;

        flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
        if (!ai_Unk4a58d0((AIPER *)p, 1, &flags, 0, util_Random(0x20) + 10.0f, &id))
            goto fail;
        o2->kind = 7;
        o2->unk04 = AIC[id].netId;
        o2->unk08 = -1;
        o2->unk10 = 0x48;
        return 0x48;
    }
    if (r == 6) {
        int out;

        out = -1;
        if (ai_Partner) {
            AI_4A63F0_7(ai_Partner, 4, 0xa0744380, 1, 0, 100.0f, &out);
            if (out != -1)
                goto done;
        }
        if (!AI_4A63F0_7(p, 6, 0xa0744380, 1, 0, util_Random(0x20) + 16.0f, &out))
            goto fail;
    done:
        o2->kind = 4;
        o2->unk04 = out;
        o2->unk08 = -1;
        o2->unk10 = 0x48;
        return 0x48;
    }
    if (r == 7 && p->unk194 < 3)
        goto fail;
    o2->unk10 = 0x48;
    return 0x48;
fail:
    return 0;
}
