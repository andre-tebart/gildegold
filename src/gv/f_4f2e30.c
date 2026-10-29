/* 0x4f2e30, an accusation by the character a of the character b (obj) with the table record i and the value v: the record's rule (byte +0x18, by kind 1 .. 7) compares v with its limit (+0x14); when it holds and the school
   factor (0x4f2da0 of a) does not exceed a random float (0x42ec20) (and, with the skill 0x22a, a second one is above 0.5), the message 0x4c4e40 is sent (time, record byte, value, ids); -1 otherwise. ROUGH */

int unk_Unk4f2e30(int i, int v, int a, int b, int obj)
{
    int rec[9];
    char *ca, *cb;
    int tgt;
    float f;
    char m[0x40];
    int ok;

    if (gv_GetTableRecord(i, rec) == 0)
        return -1;
    ca = gm_FindChar(a);
    cb = gm_FindChar(b);
    gm_FindById(&tgt, 0, 0, obj);
    ok = 1;
    switch ((unsigned char)rec[4]) {
    case 1:
        if (rec[5] != v)
            return -1;
        break;
    case 2:
        if (rec[5] == v)
            return -1;
        break;
    case 3:
        if (rec[5] <= v)
            return -1;
        break;
    case 4:
        if (rec[5] < v)
            return -1;
        break;
    case 5:
        if (v <= rec[5])
            return -1;
        break;
    case 6:
        if (v < rec[5])
            return -1;
        break;
    case 7:
        if (rec[5] == v)
            return -1;
        if (rec[5] == 2)
            return -1;
        break;
    default:
        return -1;
    }
    f = unk_Unk4f2da0((short *)ca);
    if (f <= util_RandomF()) {
        if (ai_InventoryHas(ca, 0x22a) == 0 || util_RandomF() <= 0.5f) {
            *(int *)(m + 4) = *(int *)ai_Time;
            *(int *)(m + 8) = *(int *)(ai_Time + 4);
            *(short *)(m + 0x10) = *(short *)(ai_Time + 12);
            *(int *)(m + 0x1b) = v;
            m[0x1a] = (char)rec[0];
            *(int *)(m + 0x16) = (int)cb;
            *(short *)(m + 0x1e) = (short)rec[2];
            *(int *)(m + 0x2b) = 1;
            *(int *)(m + 0x27) = obj;
            return ((int (*)(void *))ai_Unk4c4e40)(m);
        }
    }
    return -1;
}
