/* 0x59e170, the opening hours of a building kind (112 B): the table 0x62afa0 (kind dword, the open byte +4,
   the close byte +5, terminated by 0) is looked up by the kind byte of the building record 0x7e3f30 + *param_1
   * 0x24d; returns 1 when the table is empty, the kind unknown or the clock hour 0x770ae4 inside the range,
   and puts the range into param_2 / param_3 (when not null) */

int ai_Unk59e170(unsigned char *param_1, unsigned int *param_2, unsigned int *param_3)
{
    int i;
    unsigned int kind;

    if (U_I((unsigned)&full_data_62afa0) == 0)
        return 1;
    kind = U_I((unsigned)&full_data_62afa0);
    i = 0;
    do {
        if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d) == kind) {
            if (param_2 != 0)
                *param_2 = *(unsigned char *)((unsigned)&full_data_62afa4 + i * 8);
            if (param_3 != 0)
                *param_3 = *(unsigned char *)((unsigned)&full_data_62afa5 + i * 8);
            if (*(unsigned char *)((unsigned)&full_data_62afa4 + i * 8) <= (unsigned short)U_I((unsigned)&ai_TimeHour) &&
                (unsigned short)U_I((unsigned)&ai_TimeHour) < *(unsigned char *)((unsigned)&full_data_62afa5 + i * 8))
                return 1;
            return 0;
        }
        kind = U_I((unsigned)&full_data_62afa8 + i * 8);
        i++;
    } while (kind != 0);
    return 1;
}
