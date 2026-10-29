/* 0x4b5310, shows the missions of owner again (0x4b5360) */

void ai_MissionInfoDialog(int type, int id);

void ai_ShowMissions(int owner)
{
    int i, id;

    id = sv_FrameMask | 0x80;
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == owner)
            ai_MissionInfoDialog(ai_Missions[i].type, id | 0x80);
    }
}
