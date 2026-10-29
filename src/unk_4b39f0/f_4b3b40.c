/* 0x4b3b40, the number of mission types of kind (0..5, else -1) */

int ai_CountMissionTypes(int kind)
{
    int i, n;

    if (kind < 0 || kind > 5)
        return -1;
    n = 0;
    for (i = 0; i < 48; i++) {
        if (ai_MissionTypes[i].kind == kind)
            n++;
    }
    return n;
}
