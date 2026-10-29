/* 0x5894d0, the key-triggered slot selection (151 B): with U_B (0x1468410) set and U_B (0x14681ea) clear, the
   slots 0x6f5ec4 .. 0x6f5f48 whose byte +0 is that key (0x589450 first): the first valid one (+0 not -1):
   0x502020 (its +0) and 0x4fe980 (its +4) -> 0x521960 (the first, the second's +2 or -1, 0x631090) */

int sv_SelectPanelObject();

void ai_Unk5894d0(void)
{
    char *p;
    int id;
    char *e;

    if (*(unsigned char *)(unsigned)&kb_Last == 0)
        return;
    if (*(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
        return;
    for (p = (char *)(unsigned)&full_data_6f5ec4; p < (char *)(unsigned)((unsigned)full_data_6f5ec4 + 0x84); p += 0xc) {
        if (*(unsigned char *)(p - 4) != *(unsigned char *)(unsigned)&kb_Last)
            continue;
        if (*(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
            continue;
        ai_Unk589450();
        if (*(int *)p != -1)
            break;
    }
    if (p >= (char *)(unsigned)((unsigned)full_data_6f5ec4 + 0x84))
        return;
    id = gm_FindBuildingById(*(int *)p);
    e = (char *)gm_FindObjectInst(*(int *)(p + 4));
    if (e != 0)
        sv_SelectPanelObject(id, *(int *)(e + 2), (char *)(unsigned)&ch_EmptyName);
    else
        sv_SelectPanelObject(id, -1, (char *)(unsigned)&ch_EmptyName);
}
