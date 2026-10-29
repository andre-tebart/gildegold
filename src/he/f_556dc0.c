/* 0x556dc0, the allocation handler of the delivery event (ROUGH): the clock +0xc0 = the start clock + (+0xb8 * +0xb0) units (the duration), the due clock (+0x52) is the start clock + 1, the person +0xac (0x4fea50) is freed from an older job (flag 0x20 of +0x13 set: the record is freed) or gets the message 0x13 / 0x20, +0x60 = now, +0xb4 = min(+0xb0 + the person's byte +0x12, 100) */

int gm_MinutesBetween();

void he_Unk556dc0(int ev)
{
    int o = 0, n;

    *(HETIME *)(ev + 0xc0) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0xc0, 0, *(int *)(ev + 0xb8) * *(int *)(ev + 0xb0), 0);
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 0, 1);
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o != 0) {
        if ((*(unsigned char *)(o + 0x13) & 0x20) != 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0x20, 0);
    }
    *(HETIME *)(ev + 0x60) = ai_Time;
    n = *(unsigned char *)(o + 0x12) + *(int *)(ev + 0xb0);
    if (n >= 0x64)
        n = 0x64;
    *(int *)(ev + 0xb4) = n;
}
