/* 0x4b60f0, the other result dialog of the game mode (5: 0x4b5f10, else 0x4b6040) */

void ai_MissionResultByMode(int id)
{
    if (ai_MissionMode == 5)
        ai_MissionResultDialog(id);
    else
        ai_MissionPartnerDialog(id);
}
