/* 0x503c20, releases what hangs on the character c (not for a free slot): 0x524260 (0, 0, id, 0), 0x5538b0, 0x568aa0, 0x44ee40, 0x4e6d80 */

void gm_Unk503c20(short *c)
{
    if (c != 0 && *c != -1) {
        sv_MoveToGroup(0, 0, *c, 0);
        he_Unk5538b0(c);
        he_Unk568aa0(c);
        d3io_NoopCallback(c);
        amt_ClearOffices(c);
    }
}
