/* 0x5cf8a0, the use / target object of a window entry (1744 B): param_1 the user record (+0x1ec the
   allowed class mask, +4 the window holder, +0x170 the owner record, +2 the type), param_2 the entry
   (param_2[0] the object record, param_2[3] / [4] out). Errors: -1 param_1, -2 param_2, -3 no record,
   -4 0x5ba620 failed, -5 the class mask (the object +0x10) against param_1 +0x1ec. param_2[3] = the
   object word is not 0x175 or 0x42ec20 () >= 0.66f; the callback of the object +0x14 runs (0 -> return
   0); 0x576040 (0x2d, 0, 0, the word); the mask 0x4c4a60 (+0x1ec, 4, the object +0x10); an object +4:
   the object 0x4c3ba0 and the fields +0x1c (the +4 dword), +0x20 / +0x21 (1), +0x2d the clock, +0x31
   the word 0x770ae4, +0x33 / +0x37 the clock, finished by 0x4c49a0; no +4: the carrier message 0x4c4880
   (-1, the holder, the word, 1, U_B (0x6f1bc5)); the object +8 != -1: 0x4f2e30 (the id, 1, the holder,
   -1, the owner +1). Type 6 with param_2[4] == 0: the record 0x5ba660 and, when param_2[3], the text
   0x5176d0 (0xfe8, the record +0x18 + 1) through the panel 0x5cf820. ROUGH: the 0x4c3c00 field block
   is transcribed from Ghidra (the CONCAT22 sums are the object's +0x1c .. +0x37 fields). */

int ai_Unk5cf8a0(int param_1, int *param_2)
{
    char buf[0x800];
    int out;
    int kind;
    int one;
    char byteone;
    int rec;
    int target;
    int h;
    int ret;
    int i;

    if (param_1 == 0)
        return -1;
    if (param_2 == 0)
        return -2;
    param_2[4] = 0;
    if (*param_2 == 0)
        return -3;
    rec = ai_Unk5ba620(*(unsigned short *)*param_2);
    if (rec == 0)
        return -4;
    if ((*(unsigned int *)(rec + 0x10) & *(unsigned int *)(param_1 + 0x1ec)) != 0)
        return -5;
    if (*(unsigned short *)*param_2 != 0x175 || util_RandomF() >= 0.66f)
        i = 1;
    else
        i = 0;
    param_2[3] = i;
    if (*(void **)(rec + 0x14) == 0) {
        ret = 1;
    } else {
        ret = ((int (*)(int, int *))*(void **)(rec + 0x14))(param_1, param_2);
        if (ret == 0)
            return 0;
    }
    ai_Unk576040(0x2d, 0, 0, *(unsigned short *)*param_2);
    if (*(int *)(rec + 0x10) != 0)
        cm_SendSetField(*(int *)(param_1 + 4), 0x1ec, 4, *(int *)(rec + 0x10), 0);
    if (*(int *)(rec + 4) != 0) {
        kind = *(int *)(rec + 4);
        one = 1;
        byteone = 1;
        cm_BeginFieldBlock((char *)*param_2, *(int *)(*param_2 + 2));
        cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x1c, &kind);
        cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x21, &one);
        cm_AddFieldDelta(1, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x20, &byteone);
        cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x2d, (void *)(unsigned)&ai_Time);
        cm_AddFieldDelta(2, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x31, (void *)(unsigned)&ai_TimeHour);
        cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x33, (void *)((unsigned)&ai_Time + 0x6));
        cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x37, (void *)((unsigned)&ai_Time + 0xa));
        cm_SendFieldDeltas();
    }
    if (*(int *)(rec + 4) == 0)
        cm_SendExSellObject(-1, *(int *)(param_1 + 4), *(unsigned short *)*param_2, 1,
                     *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
    if (*(int *)(rec + 8) != -1) {
        if (*(int *)(param_1 + 0x170) == 0)
            target = -1;
        else
            target = *(int *)(*(int *)(param_1 + 0x170) + 1);
        unk_Unk4f2e30(*(int *)(rec + 8), 1, *(int *)(param_1 + 4), -1, target);
    }
    if (*(char *)(param_1 + 2) == 6 && param_2[4] == 0) {
        h = ai_Unk5ba660(*param_2, &out);
        if (param_2[3] == 1)
            txt_Format(buf, (char *)0xfe8, *(unsigned short *)(h + 0x18) + 1);
        ai_Unk5cf820((int)buf);
    }
    return ret;
}
