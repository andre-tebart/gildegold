/* 0x53d800, releases the slot of the event `ev`: closes its window and panel, packs the slots below it up by one (0x46cda0 re-places the windows) and sets 0x625718 to the last used slot */

void he_Unk53d800(int ev)
{
    int *sl;
    int *q;
    int *pr;
    int i, k;

    if (*(int *)(ev + 0x74) != 0) {
        sl = *(int **)(ev + 0x74);
        if (sl[1] == -1) {
            __asm int 3
            return;
        }
        d2_SetForm(U_I((unsigned)&full_data_6faa60), 0);
        d2_RemoveObj(sl[1]);
        sl[1] = -1;
        i = sl[2];
        if (i != -1) {
            d2_CloseView(i);
            sl[2] = -1;
        }
        if (U_I((unsigned)&full_data_6f262c) == (int)sl)
            U_I((unsigned)&full_data_6f262c) = 0;
        *(int *)(ev + 0x74) = 0;
        i = 0;
        q = (int *)(unsigned)&full_data_6faa84;
        do {
            if (*q == -1) {
                if (i < 0x10) {
                    q = (int *)((unsigned)&full_data_6faa90 + i * 12);
                    do {
                        if ((int)q < ((unsigned)((unsigned)full_data_6faa90 + 0xb0) + 0x4) && *q != -1) {
                            q[-3] = *q;
                            pr = q - 4;
                            q[-2] = q[1];
                            *pr = q[-1];
                            *(int **)(q[-1] + 0x74) = pr;
                            *q = -1;
                            q[-1] = 0;
                            q[1] = -1;
                            if (q[-3] == -1 && *pr != 0) {
                                __asm int 3
                                return;
                            }
                        }
                        k = q[-3];
                        if (k != -1)
                            d2_MoveObj((((k * 0xbb) & 0xffff0000) | (unsigned short)(*(short *)(U_I((unsigned)&d2_Objs) + k * 0x2ec + 0x10) - 0x20)),
                                         ((((unsigned)k >> 16) << 16) | (unsigned short)*(short *)(U_I((unsigned)&d2_Objs) + 0x12 + k * 0x2ec)), k);
                        q += 3;
                    } while ((int)q < ((unsigned)((unsigned)full_data_6faa90 + 0xb0) + 0x10));
                }
                break;
            }
            q += 3;
            i++;
        } while ((int)q < ((unsigned)((unsigned)full_data_6faa90 + 0xb0) + 0x4));
        i = 0;
        q = (int *)(unsigned)&full_data_6faa84;
        do {
            if (*q != -1)
                U_I((unsigned)&full_data_625718) = i;
            q += 3;
            i++;
        } while ((int)q < ((unsigned)((unsigned)full_data_6faa84 + 0xbc) + 0x4));
    }
}
