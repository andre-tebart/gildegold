/* 0x4c3f50, lowers cm_SpeedLevel */

void ai_Unk4c3f50(void)
{
    int buf[31];

    if (cm_SpeedLevel > 0) {
        buf[0] = cm_SpeedLevel - 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
