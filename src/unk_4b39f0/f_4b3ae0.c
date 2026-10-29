/* 0x4b3ae0, the mission type type */

G13TYPE *ai_GetMissionType(int type)
{
    int i;

    for (i = 0; i < 48; i++) {
        if (ai_MissionTypes[i].type == type)
            return &ai_MissionTypes[i];
    }
    return 0;
}
