/* 0x55b720, the allocation handler of a building event: both clocks now, the building +0xac (none: free), the due delay is the table value +0x243 of its kind (else 0x1e units), +0x10 = its id; a building with the flag 0x40 in +0x5a frees the record, else the flag is set (message 0x5a / 2 / 0x40) */

void he_Unk55b720(int ev)
{
    int b, kind, d;

    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    d = *(int *)(kind + 0x243);
    gm_AddTime(ev + 0x52, 0, d != 0 ? d : 0x1e, 0);
    *(int *)(ev + 0x10) = *(int *)(b + 1);
    if ((*(unsigned short *)(b + 0x5a) & 0x40) != 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(unsigned short *)(b + 0x5a) = *(unsigned short *)(b + 0x5a) | 0x40;
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x40, 0);
}
