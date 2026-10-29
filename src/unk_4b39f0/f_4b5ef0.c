/* 0x4b5ef0, the mission complete dialog of the game mode (5: 0x4b5970, else 0x4b5c20) */

void ai_MissionCompleteByMode(int id)
{
    if (ai_MissionMode == 5)
        ai_MissionCompleteDialog(id);
    else
        ai_MissionNextDialog(id);
}
