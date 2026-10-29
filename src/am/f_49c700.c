/* 0x49c700, execute callback 0x25: sends the "spionage" message */

int ai_Unk49c700(G5CH *p, AIARG *s1)
{
    G8MSG m;
    unsigned char *h;

    if (s1->kind != 7)
        return 0;
    m.type = 0x18;
    m.unk36 = 2;
    m.to = p->netId;
    m.unk0c = -1;
    if ((h = *(unsigned char **)((char *)p + 0x170)) != 0)
        *(int *)&m.unk58 = *(int *)(h + 1);
    else
        *(int *)&m.unk58 = -1;
    *(int *)((char *)&m + 0x70) = s1->unk04;
    *(int *)((char *)&m + 0x90) = s1->unk10;
    cm_BeginGroup("spionage");
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("spionage");
    return 0x25;
}
