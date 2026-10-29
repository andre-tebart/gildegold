/* 0x53dad0, sets the state of the event bar panel */

void he_Unk53dad0(int state)
{
    if (U_I((unsigned)&full_data_62571c) != -1)
        d2_SetViewEnabled(U_I((unsigned)&full_data_6faa60), state);
}
