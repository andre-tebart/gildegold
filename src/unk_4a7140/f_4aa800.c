/* 0x4aa800, decide callback: p (unk194 >= 3) picks a character within 33..48 units and a second one 50..100 units
   from both, by chance after 0x504e20, into m (kind 7) */

int ai_Unk4aa800(AIPER *p, int c, unsigned char *m)
{
    unsigned short ids[4];
    unsigned short id, id2;
    float f;

    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    if (p->unk194 < 3)
        return 0;
    ids[0] = p->id;
    if (ai_Unk4a5a30(p, ((unsigned)&ai_Unka69140 + 0x4973c0), 4, ids, 0.0f, (float)util_RandomU(0x10) + 33.0f, &id) == 0)
        return 0;
    ids[1] = id;
    if (ai_Unk4a5a30(p, ((unsigned)&ai_Unka69140 + 0x4973c0), 4, ids, 50.0f, 100.0f, &id2) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    f = gm_Mix(p, &((AIPER *)ai_CharsT)[id], 4);
    if (util_RandomF() + f < 1.0f)
        return 0;
    *(int *)(m + 8) = ((AIPER *)ai_CharsT)[id2].netId;
    return 1;
}
