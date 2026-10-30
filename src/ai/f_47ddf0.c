/* 0x47ddf0, looks for a partner (a character that suits this one) and sends the messages about it; returns
   the partner when the two have met enough, else 0
   @deep behaviour: 420/455 instructions differ (diffclass 582 code / 14 regs / 6 slots hunks), all read against tools/disasm.py 0x47ddf0 0x570: no missing / extra statement, constant or callee; the original keeps bestf [esp+0x14], best [esp+0x18], f [esp+0x1c] and reuses the dead parameter slot [esp+0x28] for i*0x300 (frame 0x14), ours holds best in edi, f at [esp+0x20] (frame 0x1c), and the original's return-0 tails are out of line (47de32, 47df5b, 47e2b0, 47e307) where ours are inline. spell.py: 47ddf0 / 47e175 / 47e263 dup-tail - tried the polarity spelling 'if (!c->officeKind && !c->secondOffice && c->kind != 5)' (byte-identical, reverted), the other prescriptions need the whole tail's block layout; 47e111 reload - the netId load did not move. Left: allocation / scheduling of best / bestf and the return-tail placement = match-deep. readability: Q6 done, Q2 renames done (ai_Unk505df0 -> gm_FindChar 602 refs, ai_Unk4c3c00 -> cm_AddFieldDelta 174 refs); Q1 no evidenced name (the only strings are the three pair-message texts 0x132f / 0x1339 / 0x1343); Q2 AICHAR 0x214..0x218 have no evidenced role anywhere (0x218 has no reader), the pair-message callees 0x5a8ee0 / 0x5a8f90 / 0x5a9050 differ only by text id; Q3 0x7d5f82 loop bound = ai_Chars + 768 * 0x21c + 2. */

AICHAR *ai_FindPartner(AICHAR *c)
{
    AICHAR *o, *p;
    int i, best, d, k, k2;
    float f, bestf;
    unsigned char a, z;
    unsigned short id;
    int m1;

    if (c->partner == -1) {
        best = -1;
        bestf = 128.0f;
        if (!(c->officeKind || c->secondOffice || c->kind == 5))
            return 0;
        i = 0;
        o = ai_CharsT;
        do {
            id = c->id;
            if (i != id && o->kind == 6 || o->kind == 7 || ai_WorksForMaster(o) && (o->jobId || o->prevOffice)) {
                d = gm_Unk505e60S(i, 0);
                d = d - gm_Unk505e60S(id, 0);
                d = abs(d);
                f = (10.0f - (float)d * 0.5f) * 0.1f;
                if (ai_RelationMemory[i][c->id] * f < bestf && gm_Liking(i, c->id, 1) < 34.0f) {
                    bestf = ai_RelationMemory[i][c->id] * f;
                    best = i;
                }
            }
            i++;
            o++;
        } while ((int)&o->kind < (unsigned)&gm_PlayerRecordBytes);
        if (best == -1)
            return 0;
        if (ai_Relations[best][c->id] / 2 + ai_RelationMemory[best][c->id] < -0x32 - util_Random(0x20)) {
            a = 0;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(1, 1, 0x215, &a);
            d = abs((int)bestf);
            a = (unsigned char)(d / 3 + 1);
            k = ai_Difficulty * 2 + 0x23;
            k2 = ai_Difficulty * 2 + 0xf;
            a = max(min(a, k), k2);
            cm_AddFieldDelta(1, 1, 0x216, &a);
            cm_AddFieldDelta(4, 1, 0x210, &ai_CharsT[best].netId);
            cm_AddFieldDelta(1, 1, 0x214, &ai_RelationMemory[best][c->id]);
            cm_SendFieldDeltas();
            cm_SendRelations(ai_CharsT[best].netId, c->netId, 0, 2);
            ai_Unk5a8f90(c, &ai_CharsT[best]);
        }
        return 0;
    }
    p = gm_FindChar(c->partner);
    if ((c->officeKind || c->secondOffice || c->kind == 5) && p &&
        (p->secondOffice || p->officeKind || p->kind == 6 || p->kind == 7 || p->kind == 5)) {
        if (c->unk215 <= c->unk216 + 5) {
            if (c->unk216 <= c->unk215) {
                a = 1;
                cm_BeginFieldBlock(c, c->netId);
                cm_AddFieldValues(1, 1, 0x215, &a);
                a = 1;
                cm_AddFieldDelta(1, 1, 0x218, &a);
                cm_SendFieldDeltas();
                return 0;
            }
            if (ai_RelationMemory[p->id][c->id] + c->unk214 > 0) {
                a = c->unk216 + 1;
                cm_BeginFieldBlock(c, c->netId);
                cm_AddFieldDelta(1, 1, 0x215, &a);
                a = 1;
                cm_AddFieldDelta(1, 1, 0x218, &a);
                cm_SendFieldDeltas();
                return 0;
            }
            if (c->unk215 > 3)
                return p;
            a = util_Random(2) + 1;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldValues(1, 1, 0x215, &a);
            if (c->unk215 + a > 3)
                cm_AddFieldDelta(1, 1, 0x218, &c->unk217);
            cm_SendFieldDeltas();
            if (c->unk215 + a < 4)
                return 0;
            ai_Unk5a9050(c, p);
            return p;
        }
    }
    a = 0;
    z = 0;
    m1 = -1;
    cm_BeginFieldBlock(c, c->netId);
    cm_AddFieldDelta(1, 1, 0x215, &a);
    cm_AddFieldDelta(1, 1, 0x216, &a);
    cm_AddFieldDelta(4, 1, 0x210, &m1);
    cm_AddFieldDelta(1, 1, 0x214, &z);
    a = 1;
    cm_AddFieldDelta(1, 1, 0x218, &a);
    cm_SendFieldDeltas();
    if (p) {
        cm_SendRelations(c->partner, c->netId, 0x3c, 2);
        ai_Unk5a8ee0(c, p);
    }
    return 0;
}
