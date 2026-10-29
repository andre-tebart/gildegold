/* 0x547ee0, a Ghidra miss (disassembled): the init of the "RndGeselle" event: the figure +0xb0 (0x4fea50) must exist (else the event is freed), +0xbc = 0x4c55d0 (figure, 0, the byte at +0x22f of its building kind record (0x24d bytes each from 0x7e3f30), 0), the clock is 6 units ahead (ROUGH) */

void he_Unk547ee0(int ev)
{
    int o = 0;
    int c;

    gm_FindById(&o, 0, 0, *(int *)(ev + 0xb0));
    if (o == 0)
        he_Unk53e1f0((int *)ev);
    c = *(unsigned char *)o;
    *(int *)(ev + 0xbc) = cm_SendMessage3d(o, 0, *(unsigned char *)(U_I((unsigned)&ai_BTypes) + c * 0x24d + 0x22f), 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 6, 0);
}
