/* 0x4b39f0, clears the running missions */
int ai_ClearMissions(void)
{
    memset(ai_Missions, 0, sizeof(ai_Missions));
    return 1;
}
