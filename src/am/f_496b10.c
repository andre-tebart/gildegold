/* 0x496b10, evaluate: the needs an action serves, or a companion near the job's partners */

int ai_Unk496b10(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;
    AIACT *act;
    AIJOBX *job;
    AIPER *c[3];
    float dist[3], best;
    int x, y, j, bi;

    if (f) {
        *a = -1.0e30f;
        *b = -1.0e30f;
        return 0;
    }
    act = &ai_Unkf83ec0[i];
    *a = 0.0f;
    *b = 0.0f;
    if (!p->unk168) {
        if (p->unk166 == 0xd)
            goto fallback;
        x = ai_JudgeCatholic ? ai_JudgeCatholic->netId : -1;
        y = ai_JudgeProtestant ? ai_JudgeProtestant->netId : -1;
        for (job = AI_53E400_3(1, 0, 0x2c); job; job = (AIJOBX *)he_NextEvent())
            if (job->unkb0 == p->netId)
                break;
        if (!job || job->unkb4 == x || job->unkb4 == y)
            goto fallback;
        dist[0] = 100.0f;
        dist[1] = 100.0f;
        dist[2] = 100.0f;
        best = 100.0f;
        bi = -1;
        c[0] = AI_505DF0(job->unkb4);
        c[1] = AI_505DF0(job->unkb8);
        c[2] = AI_505DF0(job->unkbc);
        if (c[0] && c[0] != AI_6F0468)
            dist[0] = gm_Liking(p->id, c[0]->id, 1);
        if (c[1] && c[1] != AI_6F0468)
            dist[1] = gm_Liking(p->id, c[1]->id, 1);
        if (c[2] && c[2] != AI_6F0468)
            dist[2] = gm_Liking(p->id, c[2]->id, 1);
        for (j = 0; j < 3; j++) {
            if (dist[j] < best) {
                bi = j;
                best = dist[j];
            }
        }
        if (bi == -1)
            goto fallback;
        o1->kind = 7;
        o1->unk04 = c[bi]->netId;
        o1->unk08 = 1;
    }
    for (j = 0; j < 4; j++) {
        if (act->w1[j].w > 0.0f) {
            if (act->w1[j].k == p->sel)
                *a += p->mood[act->w1[j].k][2] * act->w1[j].w * 0.02f;
            else
                *a += p->mood[act->w1[j].k][2] * act->w1[j].w * 0.01f;
        }
        if (act->w2[j].w > 0.0f) {
            if (act->w2[j].k == p->sel)
                *b += p->mood[act->w2[j].k][2] * act->w2[j].w * 0.02f;
            else
                *b += p->mood[act->w2[j].k][2] * act->w2[j].w * 0.01f;
        }
    }
    return 1;
fallback:
    gv_GetTableRecord(16, &d);
    if (d.cur)
        return ai_Unk4a4c10(a, b, i, p, &d);
    return ai_Unk4a49a0(a, b, i, p, 0, o1, o2);
}
