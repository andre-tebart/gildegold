/* 0x56dd20, the alloc of the research event 0x98 (not while paused, flags +0x78 & 4): the clock +0x52 = now, the hour set to a random 9 .. 16 (or 13 on the next day from 20 on), +0xac = 0, +0xb0 / +0xb4 = -1, and the handler scheduled (0x4c4c60) into +0x84 */

void he_Unk56dd20(int ev)
{
    int h;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (*(unsigned short *)(ev + 0x56) < 0x14) {
        if (*(unsigned short *)(ev + 0x56) < 9) {
            h = util_Random(8);
            *(unsigned short *)(ev + 0x56) = h + 9;
        }
    } else {
        *(unsigned short *)(ev + 0x56) = 0xd;
        *(int *)(ev + 0x52) += 1;
    }
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = -1;
    *(int *)(ev + 0xb4) = -1;
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
