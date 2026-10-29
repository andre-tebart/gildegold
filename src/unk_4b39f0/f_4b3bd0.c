/* 0x4b3bd0, sets mission 0 */

char ai_MissionMode = -2;  /* 0x624580, the mission kind chosen (5: one mission only, -1: none) */

void ai_Unk4b3bd0(int owner, int type)
{
    ai_Missions[0].type = type;
    ai_Missions[0].owner = owner;
    ai_Missions[0].time.year = 0;
    gm_SetTime(&ai_Missions[0].time, 0, 0, 0);
    ai_Missions[0].unk20 = 0;
}
