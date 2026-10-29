/* 0x5524e0, the allocation handler of an event without the flag 4 of +0x78: the due clock (+0x52) is a copy of the start clock (+0x44) and the event is posted (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5524e0(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
