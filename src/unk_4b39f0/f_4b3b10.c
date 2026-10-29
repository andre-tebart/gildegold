/* 0x4b3b10, 1 if owner has a mission */

int ai_HasMission(int owner)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type && ai_Missions[i].owner == owner)
            return 1;
    }
    return 0;
}
