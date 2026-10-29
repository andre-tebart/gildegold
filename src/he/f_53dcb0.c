/* 0x53dcb0, makes the slot `sl` the active event window (0x6f262c), closing the previous one */

void he_Unk53dcb0(int *sl)
{
    char *r;

    if (U_I((unsigned)&full_data_62571c) != -1) {
        if (U_I((unsigned)&full_data_6f262c) != 0) {
            if (((int *)U_I((unsigned)&full_data_6f262c))[1] != -1)
                d2_SetObjValue(((int *)U_I((unsigned)&full_data_6f262c))[1], 0);
            d2_SetViewEnabled(((int *)U_I((unsigned)&full_data_6f262c))[2], 0);
            r = (char *)*(int *)U_I((unsigned)&full_data_6f262c);
            if (HE_JUMP(r))
                U_I((unsigned)&tb_Paused) = 0;
            U_I((unsigned)&full_data_6f262c) = 0;
        }
        if (sl != 0 && sl[2] != -1) {
            if (sl[1] != -1)
                d2_SetObjValue(sl[1], 1);
            d2_SetViewEnabled(sl[2], 1);
            d2_RaiseViewWindows(sl[2]);
            r = (char *)*sl;
            if (HE_JUMP(r))
                U_I((unsigned)&tb_Paused) = 1;
        }
        U_I((unsigned)&full_data_6f262c) = (int)sl;
    }
}
