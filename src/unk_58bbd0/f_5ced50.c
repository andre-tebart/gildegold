/* 0x5ced50, an AIMSG type 0x45 "the interrogation was refused" message (144 B): the record is zeroed
   (0x3e dwords, 0xf8 bytes), +4 = 0x45 the type, +8 = param_1 + 4 (the character), +0xc = -1, +0x28 ..
   +0x34 the clock 0x770ae0 .., +0x36 = 2, +0x58 = param_1 + 4 (again), +0x5c = param_2 + 4 (the
   partner), +0x60 = param_3; sent by 0x4c4b70 (its handle is the return value). */

int ai_Unk5ced50(int param_1, int param_2, int param_3)
{
    int rec[0x3e];
    int i;

    for (i = 0; i < 0x3e; i++)
        rec[i] = 0;
    *(char *)((char *)rec + 4) = 0x45;
    rec[2] = *(int *)(param_1 + 4);
    rec[3] = -1;
    rec[10] = U_I((unsigned)&ai_Time);
    rec[11] = U_I((unsigned)&ai_TimeHour);
    rec[12] = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)((char *)rec + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
    *(char *)((char *)rec + 0x36) = 2;
    rec[0x16] = *(int *)(param_1 + 4);
    rec[0x17] = *(int *)(param_2 + 4);
    rec[0x18] = param_3;
    return cm_SendLongMessage(rec);
}
