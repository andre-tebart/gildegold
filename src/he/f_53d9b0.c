/* 0x53d9b0, closes every used slot and the event bar panel */

void he_Unk53d9b0(void)
{
    int *p = (int *)(unsigned)&full_data_6faa80;

    do {
        if (p[1] != -1)
            he_Unk53d800(*p);
        p += 3;
    } while ((int)p < (int)((unsigned)&full_data_6faa84 + 0xbc));
    d2_CloseView(U_I((unsigned)&full_data_6faa60));
}
