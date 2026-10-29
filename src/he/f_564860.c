/* 0x564860, the allocation handler of the fire event (ROUGH): the clocks +0x52 / +0x60 are now, +0xb4 = 0, the building +0xac (none: free), the nearest building (0x501e40, 0x458a20 positions) gives the distance scaled by +0xb0 (_ftol) as the burning time +0xb0 and +0x84 = -1 */

void he_Unk564860(int ev)
{
    int b;

    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x60) = ai_Time;
    *(unsigned char *)(ev + 0xb4) = 0;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    gm_NearestBuilding(b);
    *(int *)(ev + 0x84) = -1;
    *(int *)(ev + 0xb0) = (int)((float)*(int *)(ev + 0xb0) * 1.0f);
}
