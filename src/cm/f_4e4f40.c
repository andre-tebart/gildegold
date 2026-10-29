/* 0x4e4f40, the pending party with the smallest id that has the flag 1 (used, id not -1): its record (0x4e50a0), 0 without */

int ai_Unk4e4f40(void)
{
    int i;
    unsigned int best;

    best = (unsigned int)-1;
    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].id != -1 && (ai_Unk925520[i].flags & 1) != 0 && best >= (unsigned int)ai_Unk925520[i].id)
            best = ai_Unk925520[i].id;
    }
    if (best != (unsigned int)-1)
        return cm_PartyById(best);
    return 0;
}
