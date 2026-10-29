/* 0x495ce0, executes the current action of character i and applies its need weights */

int ai_ExecuteAction(int i)
{
    G5CH *p = &G5_CHARS[i];
    G5ACT *act;
    unsigned char msg;
    int k;

    if (p->act != 0 && G5_ACTS[p->act].exec != 0 && G5_ACTS[p->act].exec(p, &p->arg1, &p->arg2) != 0) {
        ai_Unk4c3e70(p);
        for (k = 0; k < 4; k++) {
            act = &G5_ACTS[p->prev];
            if (act->w1[k].k < 14)
                ai_Unk4c3eb0(act->w1[k].k, act->w1[k].w * -0.5f);
        }
        for (k = 0; k < 4; k++) {
            act = &G5_ACTS[p->act];
            if (act->w1[k].k < 14)
                ai_Unk4c3eb0(act->w3[k].k, act->w3[k].w * -0.5f);
        }
        if (p->prev == p->act) {
            ai_Unk4c3eb0(13, 500.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
                msg = 2;
                cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        } else {
            ai_Unk4c3eb0(13, 250.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
                msg = 1;
                cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        }
        ai_Unk4c4a20();
        memcpy(G5_ACTS[p->act].w3, G5_ACTS[p->act].w1, 0x20);
        return 1;
    }
    if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
        msg = 3;
        cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
        cm_AddFieldValues(1, 1, 0x215, &msg);
        cm_SendFieldDeltas();
    }
    return 0;
}
