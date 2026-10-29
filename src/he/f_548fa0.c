/* 0x548fa0, a repeating payment: every 0x18 units the player of the event (word +8) gets +0xac (0x4c47e0, from the bank -1) for +0xb0 days */

void he_Unk548fa0(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st >= -2) {
        if (st >= 0) {
            if (st != 0)
                return;
            cm_SendMoveGoods(-1, U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, *(int *)(ev + 0xac));
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
