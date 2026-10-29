/* 0x537db0, frees the preloaded sound banks (0x426ba0) of the table 0x75df20 */

void main_FreeSoundBanks(void)
{
    int *p;

    p = (int *)(unsigned)&full_data_75df20;
    do {
        if (*p != 0)
            sb_UnloadSampleBank(*p);
        p++;
    } while ((int)p < (int)((unsigned)&full_data_75df20 + 0x200));
}
