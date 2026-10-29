/* 0x4ac2f0, applies record k to character p (message to the player d): kind 0 (ids 0..4) raises skill id by
   8..15, at most to 252; kind 1 sends a message from every other character whose class (0x504ad0) is id */

void hmt_ApplyRecord(AICHAR *p, int d, unsigned short k)
{
    G11REC *e;
    int i;
    unsigned char v;

    cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, d);
    cm_SendSetField(p->netId, 0x1c8, 4, 0x20000, 0);
    e = hmt_GetRecord(k);
    if (e != 0) {
        if (e->kind == 0 && e->id <= 4) {
            v = util_Random(8) + 8;
            v = (unsigned char)min((float)(char)v, 252.0f - ((unsigned char *)p)[e->id + 0x80]);
            cm_BeginFieldBlock((AIPER *)p, p->netId);
            CM_AddFieldValuesShort(1, 1, e->id + 0x80, &v);
            cm_SendFieldDeltas();
        } else if (e->kind == 1) {
            for (i = 0; i < 768; i++) {
                if (ai_Chars[i].netId != p->netId && gm_TitleGroup(ai_Chars[i].jobId) == e->id)
                    cm_SendRelations(p->netId, ai_Chars[i].netId, util_Random(4) + 3, 0);
            }
        }
    }
}
