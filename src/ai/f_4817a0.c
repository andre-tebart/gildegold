/* 0x4817a0, ai_CalcAngriff: spy on a building, then attack it @deep B3 far draft (frame 0x518 = the original's; diffclass 749 code / 12 regs / 4 slots): the body was read against build/ghidra/4817a0.c and tools/asmdiff - the statements, constants (the kinds 0x13 / 4 / 0x10, the job types 0x40 / 0x3f / 0x49, the roll util_Random(0x32) + 0x32 <= unk37), callees and field offsets match; the machine diff is the register allocation (ours ch=edi, original ch=ebx) and the slot rotation (0x20 / 0x24 / 0x2c) it causes - the same pattern as the other AI drafts - plus the two float product spellings, which are fpu-identical in diffclass (no lead). Readability advanced: ai_Unk5090b0 -> gm_BuildingRank, ai_Unk509c80 -> gm_OwnerWorth (both with their f3gm.c definitions). Q2 gaps left: ord_GetGridCell (ord.c's ord_GetGridCell; the unit has no name, @named_by guess, so no prefix can be chosen here), he_Unk553cc0 (role read in he.c: 1 unless an event 0x62 with +0xb0 = the id and +0xb8 == 1 runs; no name), AIOBJ.unk2a / unk37 (an object's building id and a roll threshold; no source names them), AIJOB.unkac (a union id: the building id in the espionage jobs, a netId in the am.c jobs - no single role name, and the union already aliases it as id). */

/* the master's free people (no job, at home) into msg.ids, -1 for the rest */
#define AI_FILL_IDS(n)                                                                                      \
    k = 0;                                                                                                  \
    for (c = 0; k < (n); c++) {                                                                             \
        if (c >= 768)                                                                                       \
            break;                                                                                          \
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title                    \
            && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)  \
            msg.ids[k++] = ai_CharsT[c].netId;                                                              \
    }                                                                                                       \
    for (; k < 8; k++)                                                                                      \
        msg.ids[k] = -1;

int ai_CalcAngriff(AICHAR *ch, int n)
{
    AIOBJ *o;
    AIBLD *bb, *best;
    AIJOB *job;
    AIBTYPE *bt;
    int i, j, k, c, r, t;
    float max, v, w, bestw;
    AIMSG msg;
    char buf[1024];

    sprintf(buf, "ai_CalcAngriff(): Meister %s m\xf6" "chte angreifen", ch->name);
    if (ch->target != -1) {
        best = gm_FindBuildingById(ch->target);
        if (!best)
            ch->target = -1;
        else if (best->owner == ch->bld->owner)
            ch->target = -1;
    }
    if (ch->target == -1) {
        max = 0.0f;
        for (j = 0; j < 8; j++) {
            for (i = 0; i < 8; i++) {
                v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                if (!(max > v))
                    max = v;
            }
        }
        if (max == 0.0f)
            max = 1.0f;
        best = 0;
        bb = ai_BldsT;
        for (t = 0; t < 256; t++, bb++) {
            if (bb->type) {
                bt = &ai_BTypesT[bb->type];
                if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                    && bb->owner != *ai_PlayerChar
                    && (bt->kind == 0x13 || bt->kind == 4 || bt->kind == 0x10)
                    && he_Unk553cc0(bb)) {
                    if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                        w = 0.0f;
                    else
                        w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                    r = gm_BuildingRank(bb);
                    w = (8.0f - r) * w * 0.5 * util_Random(100) * 0.01;
                    if (best) {
                        if (!((float)gm_OwnerWorth(bb, 0) * w > (float)gm_OwnerWorth(best, 0) * bestw))
                            continue;
                    }
                    bestw = w;
                    best = bb;
                }
            }
        }
        ch->target = best->id;
    }
    if (!best || (ai_BTypesT[best->type].kind != 0x13 && ai_BTypesT[best->type].kind != 4
                  && ai_BTypesT[best->type].kind != 0x10))
        return 0;
    for (o = gm_Query(ch->bld->objList, 1, 0, 0xca); o; o = gm_QueryNext()) {
        if (o->unk2a == best->id) {
            if ((unsigned)util_Random(0x32) + 0x32 <= o->unk37) {
                if (n < 3)
                    return 0;
                for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent()) {
                    if (job->unkac == best->id) {
                        sprintf(buf, "ai_CalcAngriff(): Meister %s hat aber noch eine Spionage bei '%s' nicht beendet",
                                ch->name, best->name);
                        return 1;
                    }
                }
                for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent()) {
                    if (job->unkac == best->id) {
                        sprintf(buf, "ai_CalcAngriff(): Meister %s hat aber noch eine Spionage bei '%s' nicht beendet",
                                ch->name, best->name);
                        return 1;
                    }
                }
                for (job = he_FindEvent(2, 0, 0x49, 3, ch->bld->id); job; job = he_NextEvent()) {
                    if (job->unkac == best->id) {
                        sprintf(buf, "ai_CalcAngriff(): Meister %s hat aber noch eine Spionage bei '%s' nicht beendet",
                                ch->name, best->name);
                        return 1;
                    }
                }
                sprintf(buf, "ai_CalcAngriff(): Meister %s greift an '%s' ", ch->name, best->name);
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x49;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.time = ai_TimeT;
                msg.flags = 1;
                msg.arg[0] = best->id;
                msg.arg[1] = ch->bld->id;
                if (n > 7)
                    n = 8;
                AI_FILL_IDS(n)
                if (msg.ids[0] == -1)
                    return 1;
                cm_SendLongMessage(&msg);
                ch->target = -1;
                return 1;
            }
            break;
        }
    }
    for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent()) {
        if (job->unkac == best->id) {
            sprintf(buf, "ai_CalcAngriff(): Meister %s hat schon eine Spionage bei '%s' laufen", ch->name,
                    best->name);
            return 1;
        }
    }
    for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent()) {
        if (job->unkac == best->id) {
            sprintf(buf, "ai_CalcAngriff(): Meister %s hat schon eine Spionage bei '%s' laufen", ch->name,
                    best->name);
            return 1;
        }
    }
    for (job = he_FindEvent(2, 0, 0x49, 3, ch->bld->id); job; job = he_NextEvent()) {
        if (job->unkac == best->id) {
            sprintf(buf, "ai_CalcAngriff(): Meister %s hat schon eine Spionage bei '%s' laufen", ch->name,
                    best->name);
            return 1;
        }
    }
    sprintf(buf, "ai_CalcAngriff(): Meister %s spioniert bei '%s'", ch->name, best->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x40;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_TimeT;
    msg.flags = 1;
    msg.arg[0] = best->id;
    msg.arg[1] = ch->bld->id;
    if (n > 7)
        n = 8;
    AI_FILL_IDS(n)
    cm_SendLongMessage(&msg);
    return 1;
}
