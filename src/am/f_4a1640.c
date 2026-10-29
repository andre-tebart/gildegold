/* 0x4a1640, execute callback 0x3b: message 0x5f about the offer s2 (kind 0xe) for the child s1 (kind 7), to
   character 0; sets flag 0x10000 of its field 0x1c8 */

int ai_Unk4a1640(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AICHAR *c;

    if (s1->kind != 7 || s2->kind != 0xe)
        return 0;
    c = gm_FindChar(0);
    if (c == 0)
        return 0;
    m.unk36 = 0x12;
    m.type = 0x5f;
    m.to = ((G5CH *)c)->netId;
    ((unsigned char *)&m)[0x64] = (unsigned char)s2->unk04;
    *(int *)&m.unk58 = p->netId;
    m.unk0c = -1;
    ((unsigned char *)&m)[0x65] = (unsigned char)s2->unk10;
    m.unk60 = s1->unk10;
    cm_SendLongMessage((AIMSG *)&m);
    cm_SendSetField(((G5CH *)c)->netId, 0x1c8, 4, 0x10000, 0);
    return 0x3b;
}
