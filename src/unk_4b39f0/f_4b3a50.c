/* 0x4b3a50, the running mission of m's owner */

G13MIS *ai_FindOwnerMission(G13MIS *m)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type && ai_Missions[i].owner == m->owner)
            return &ai_Missions[i];
    }
    return 0;
}
