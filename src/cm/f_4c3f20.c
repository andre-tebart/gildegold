/* 0x4c3f20, raises cm_SpeedLevel */

void ai_Unk4c3f20(void)
{
    int buf[31];

    if (cm_SpeedLevel < 4) {
        buf[0] = cm_SpeedLevel + 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
