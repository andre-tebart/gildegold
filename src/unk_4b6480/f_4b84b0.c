/* 0x4b84b0, message: as 0x4b82c0, the worker chosen from more than 2; 0x4fca40 of p by -(p's 0x1c float + 1) (not
   with item 0x290)
   @deep behaviour draft (5 -> 1 close after the sibling idiom): 10 differing instructions in one hunk. The 0x230/0x20d ternary spelling of its sibling ai_Unk4b82c0 is applied and took 54 -> 11; the rest is the while (util_Random(k) == s) test: the original pushes k (push edi) before loading p = a[s] and merges the cleanup of the previous util_Random argument into add esp,0x8 after the test call, ours does add esp,4 / and esi / mov ebp / push edi / call. Read: the loop, s, p and the two calls are the same statements (p = a[s] before the loop matches the original's load order), no meaning error. spell.py: no hunk matches the menu ('no menu rule; meaning check first'), the fill hunk is only the tail. Pure argument-push scheduling/instruction order - match-deep's scheduling work. */

int ai_Unk4b84b0(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, s;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_Random(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k > 2) {
        s = util_Random(k);
        p = a[s];
        while (util_Random(k) == s)
            ;
    }
    if (b == 0 || p == 0)
        return 0x400;
    if (util_Random(2)) {
        txt_Format(buf, m->type + 0x12e7, b, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title,
                     p->id, p->id);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        gm_AddHealthS(p->id, (int)-(*(float *)((char *)p + 0x1c) + 1.0f));
    }
    return 0;
}
