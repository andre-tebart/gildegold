/* 0x53a450, per-frame update of every player slot (0x770b80 .. 0x7d5f82, 0x21c each; ROUGH) */

void main_Unk53a450(void)
{
    int i = 0;
    char *p = (char *)((unsigned)&ai_CharsT + 0x2);
    int k;
    int fl;

    do {
        if (*(short *)(p - 2) != -1) {
            if (p[0x162] != 0 || p[0x163] != 0)
                main_Unk53a250(i);
            if (*p == 2 || *p == 5 || *p == 4) {
                if (gm_Unk50a6c0(p - 2, 0) != 0) {
                    k = (int)-(*(float *)(p + 0x1a) + 5.0f);
                    gm_AddHealth(*(short *)(p - 2), k);
                }
            }
            if (p[0x1af] != 0) {
                fl = 0xff000000;
                cm_BeginFieldBlock(p - 2, *(int *)(p + 2));
                cm_AddFieldValues(1, 1, (int)(p + 0x1af - U_I((unsigned)&cm_FieldObject)), (char *)&fl + 3);
                cm_SendFieldDeltas();
            }
        }
        p += 0x21c;
        i++;
    } while ((int)p < (unsigned)&gm_PlayerRecordBytes);
}
