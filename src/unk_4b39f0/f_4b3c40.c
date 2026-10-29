/* 0x4b3c40, a random mission type of kind (0xff: none) */

int ai_RandomMissionType(int kind)
{
    int i, j, r, n;

    n = ai_CountMissionTypes(kind);
    if (n > 0) {
        r = ai_MissionRandom(n);
        for (i = 0, j = 0; i < 48; i++) {
            if (ai_MissionTypes[i].kind == kind)
                j++;
            if (j > r)
                return ai_MissionTypes[i].type;
        }
    }
    return 0xff;
}
