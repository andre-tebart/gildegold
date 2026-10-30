/* 0x4ba0c0, message: one of c's buildings of kind 1 / 2 is picked, and when at least 2 of its kind-0 workers (up
   to k231 + k232 of the building type) all have a gm_Liking (c, worker, 1) of at least a 0x19..0x2c roll, an
   AIMSG type 0x4a names the building (id, fromId, n), c, the object gm_BuildingMainObject (0x501b50) gives and
   the news text id, to the local player ai_PlayerId; the receive side (cm.c 0x4cbb90) finds that object and tears
   it down (f3gm.c 0x5034b0). The three 0x400 guards share one exit: the goto none keeps that block last, as in
   the original.
   @deep behaviour reached (10 order hunks: 168/168 instructions, spell.py 'same instructions, other order') - byte needs the scheduler (the order of the b->id load, the type / flags stores and the ai_PlayerId lea). Raised here: the frame (0x128 -> 0x124) and the FP compare via the liking operand order (gm_Liking(...) < r: fild after the call, fcompp, no float spill), the shared 0x400 exit via goto none, msg.n = msg.fromId (one b->id load). Q1: no name evidence - the gate is the workers' liking, but the 0x4a receive side (0x4cbb90 -> 0x5034b0) tears the building's object down and the text 0x1315 is not in the exe, so the message's wording cannot be stated. Q4: the one left is the 16-bit text-id store over AIMSG.dst[0] (0x64); the textId union arm in ai.h was built and regressed ai_Unk483a60 byte -> 5 differing (its slots, the same failure ai.c's note records for 0x60), reverted - a shared-header decision. */

int ai_Unk4ba0c0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    AIBTYPE *t;
    AIOBJ *e;
    int n, k, i, r;
    void *a[8];
    AIMSG msg;

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(b->type);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        goto none;
    k = 0;
    r = util_Random(0x14) + 0x19;
    b = a[util_Random(n)];
    t = &ai_BTypes[b->type];
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].kind == 0 && ai_Chars[i].bld == b) {
            if (gm_Liking(c->id, i, 1) < r)
                goto none;
            if (++k >= t->k231 + t->k232)
                goto none;
        }
    }
    if (k >= 2) {
        e = gm_BuildingMainObject(b);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x4a;
        msg.fromId = b->id;
        msg.flags = 1;
        msg.n = msg.fromId;
        msg.to = ai_Chars[ai_PlayerId].netId;
        msg.time = ai_TimeT;
        /* the text id at 0x64 is the only 16-bit store of AIMSG (a word over dst[0]); a textId union arm in
           ai.h regresses ai_Unk483a60's slots (ai.c's note), so the shared-header view is match-deep's */
        *(unsigned short *)&msg.dst[0] = m->type + 0x12e7;
        msg.car = c->netId;
        msg.ch = e->objId;
        cm_SendLongMessage(&msg);
        return 0;
    }
none:               /* the guards share this one 0x400 exit, as in the original */
    return 0x400;
}
