/* 0x548b70, the init of a timed event: the clock +0x52 = the start (+0x44); a random half (0x42ec00 (2) != 0) frees it at once */

void he_Unk548b70(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    if ((short)util_Random(2) != 0)
        he_Unk53e1f0((int *)ev);
}
