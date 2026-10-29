/* 0x4c3f90, message 0x13 with the player's id */

int cm_SendPlayerId(void)
{
    int buf[31];

    buf[0] = ai_CharInts[ai_PlayerId][0];
    cm_SendExSysMessage(0x13, buf);
}
