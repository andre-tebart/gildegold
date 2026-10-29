/* 0x53d6f0, gives the event `ev` a slot (bit 8 of ev+0x78 must be set; 1 = no window wanted, 2 = no slot) with a message panel `b, c, d` and a window (0x4731d0); the slot is stored at ev+0x74 */

int he_Unk53d6f0(int ev, int b, int c, int d, int e)
{
    int i = 0;
    int *p;
    int *sl;

    if ((*(unsigned char *)(ev + 0x78) & 8) == 0) {
        *(int *)(ev + 0x74) = 0;
        return 1;
    }
    p = (int *)(unsigned)&full_data_6faa84;
    do {
        if (*p == -1) {
            if (i < 0x10)
                goto got;
            break;
        }
        p += 3;
        i++;
    } while ((int)p < ((unsigned)((unsigned)full_data_6faa84 + 0xbc) + 0x4));
    i = he_Unk53d630();
got:
    if (i == -1) {
        *(int *)(ev + 0x74) = 0;
        return 2;
    }
    if (U_I((unsigned)&full_data_625718) < i)
        U_I((unsigned)&full_data_625718) = i;
    d2_SetForm(U_I((unsigned)&full_data_6faa60), 0);
    *(int **)(ev + 0x74) = SLOT(i);
    SLOT(i)[0] = ev;
    sl = *(int **)(ev + 0x74);
    sl[1] = d2_WinOpenPic(i << 5, 0, e, U_I((unsigned)&d2_LastWindow));
    U_I(U_I((unsigned)&d2_Objs) + 0x48 + (*(int **)(ev + 0x74))[1] * 0x2ec) = 1;
    (*(int **)(ev + 0x74))[2] = d2_OpenForm(b, c, d);
    d2_PlaceMainWindow((*(int **)(ev + 0x74))[2], 3);
    d2_SetViewEnabled((*(int **)(ev + 0x74))[2], 0);
    if ((*(int **)(ev + 0x74))[1] != -1) {
        if ((*(int **)(ev + 0x74))[2] != -1)
            return 0;
        __asm int 3
    } else {
        __asm int 3
    }
}
