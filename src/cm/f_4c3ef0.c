/* 0x4c3ef0, sets cm_SpeedLevel (0..4) through message 0x12 */

void cm_SetSpeedLevel(unsigned int v)
{
    int buf[31];

    if (v >= 4)
        v = 4;
    if (v != cm_SpeedLevel) {
        buf[0] = v;
        cm_SendExSysMessage(0x12, buf);
    }
}
