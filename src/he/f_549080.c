/* 0x549080, like 0x548fa0 but the amount -+0xac goes to the player through 0x53eec0 */

void he_Unk549080(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st >= -2) {
        if (st >= 0) {
            if (st != 0)
                return;
            he_Unk53eec0(*(unsigned short *)(ev + 8), U_I((unsigned)&full_data_1442500), -*(int *)(ev + 0xac), 0);
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            if (*(int *)(ev + 0xb0) != 0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                return;
            }
        }
        he_Unk53e1f0((int *)ev);
    }
}
