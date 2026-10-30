/* 0x4b9cd0, message 0x2b: one worker p (kind 0, title set) of one of c's category-1 buildings b is picked, and
   when a 0..49 roll beats gm_Liking(c, p, 1) (the worker does not like c, and a building's AIMSG type 0x31
   stops whoever it names) c is told (text 0x12e7 + 0x2b) and p is told to stop working: an AIMSG type 0x31
   (fromId b->id, to c, arg[0] p, the receive side cm.c 0x4c9c10 runs ch_StopActions on "sp_<p>", ch.c's stop of
   the action). Skipped when c holds the item 0x290. As 0x4b84b0 (which hurts the worker instead).
   @deep behaviour draft (42 -> 34 code hunks before the revert): the whole body reads the same as the original;
   the one differing region is the AIMSG fill after he_ShowNews (instruction 150 on), i.e. the tail of
   memset / fromId / to / time / type / flags / arg. Tried: (1) hoisting the fromId load into a local before the
   memset - the original loads [ebp+1] before the rep stos; it matched that one instruction (150 -> 155 equal)
   but grew the bytes (1414 -> 1447 differing, WORSE) and the rest stayed as a different schedule, so it was
   reverted; (2) spell.py's width lead (16-bit vs 32-bit read of the same slot) - both sides read `mov ax` and
   store a WORD (msg.time's last word, 0x34), so the widths already agree; the 16-bit store's disp differs by 4
   only because the original sinks it past `push &msg`. Nothing left in base's means: it is the store / load
   schedule and the register choice of the time copy (original 0x30 before 0x28, ours ascending). */

int ai_MsgWorkerStop(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i;
    float f;
    void *a[8];
    AIMSG msg;
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(b->type) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_Random(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind == 0 && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k > 2)
        p = a[util_Random(k)];
    if (b == 0 || p == 0)
        return 0x400;
    f = gm_Liking(c->id, p->id, 1);
    if (util_Random(0x32) > f) {
        txt_Format(buf, m->type + 0x12e7, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title, p->id, b);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        memset(&msg, 0, sizeof(msg));
        msg.fromId = b->id;
        msg.to = c->netId;
        msg.time = ai_TimeT;
        msg.type = 0x31;
        msg.flags = 1;
        msg.arg[0] = p->netId;
        cm_SendLongMessage(&msg);
        return 0;
    }
    return 0x400;
}
