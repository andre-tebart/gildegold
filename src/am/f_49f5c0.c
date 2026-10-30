/* 0x49f5c0, decide callback 0x34: a kind-5 character (0x194 >= 4, money / 32 >= 5000, no job 0x24 of its own)
   builds a building of a random class (0x5043d0) it has fewer than 1..4 of and none of in its list */

int ai_Unk49f5c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int cnt[15];
    int own[15];
    unsigned char *q, *l;
    AIJOB *j;
    AIBLD *b;
    int money, k, n, r, c, x, y;

    q = (unsigned char *)gm_PlayerRecord((AICHAR *)p);
    if (((unsigned char *)p)[2] != 5 || *(int *)((unsigned char *)p + 0x194) < 4 || q == 0)
        return 0;
    money = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
    if (money / 32 < 5000)
        return 0;
    for (j = he_FindEvent(1, 0, 0x24); j != 0; j = he_NextEvent()) {
        if (j->unkac == (int)p->netId)
            return 0;
    }
    gm_TitlesLeft(((unsigned char *)p)[0x164]);
    memset(cnt, 0, sizeof(cnt));
    l = q + 0x75;
    for (k = 0; k < 15; k++, l++)
        own[k] = gm_TitlesLeft(*l);
    for (b = ai_BldsT, k = 0; k < 0x100; k++, b++) {
        if (b->type != 0)
            cnt[gm_BuildingJob(ai_BTypesT[b->type].kind) - 1]++;
    }
    n = util_Random(4) + 1;
    r = util_Random(0x10);
    for (c = 0x10; c != 0; c--) {
        if (r != 0 && own[r - 1] == 0 && cnt[r - 1] < n)
            break;
        r = (r + 1) % 0x10;
    }
    if (c == 0 || r == -1)
        return 0;
    x = gm_TitleNumber(r, 1);
    y = gm_FirstTitleNumber(x);
    if (y == 0)
        return 0;
    s1.unk04 = y;
    s1.kind = 5;
    s1.unk08 = 1;
    s2.kind = 0;
    s2.unk04 = 0;
    s2.unk08 = 0;
    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
        return 0;
    if (money < gm_BuildingKindValue(s1.unk04))
        return 0;
    *o1 = s1;
    o1->unk10 = x;
    *o2 = s2;
    return 0x34;
}
