/* 0x546c40, a Ghidra miss (disassembled): the outcome of a fight between the figures +0xac (a) and +0xb0 (b): the clock is set, the state 2; unless a or b is gone or marked (+0x1c8 flags 0x100 / 0x200) the event ends. With +0xb4 set (a won) the next clock is in 0x18 units and b hears the news 0x1e18; else (a lost: code -2) in 0x60 units, the news 0x1d5b, the relation change 0x4f2e30 (0x16 ..) and 0x34, up to 3 of the 0x4f4080 records are given to a (0x4c4f10), a of kind 6 gets 0x4b44e0 (0xb); both lose the flags and 0x4c5f60 closes the fight with the code (-4 won, -2 lost) (ROUGH) */

void he_Unk546c40(int ev)
{
    char text[0x100];
    int sel[16];
    int arr[32];
    int a, b, code, x, n, i, r;

    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x70) = 2;
    if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
        return;
    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    if (a == 0 || (*(unsigned *)(a + 0x1c8) & 0x100) != 0 || b == 0 || (*(unsigned *)(b + 0x1c8) & 0x200) != 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(int *)(ev + 0xb4) != 0) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(int *)(ev + 0x58) = 0;
        if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
            txt_Format(text, 0x1e18, *(unsigned short *)a);
            he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, (char *)((unsigned)&ai_Unk625820 + 0x1284));
        }
        code = -4;
    } else {
        gm_AddTime(ev + 0x52, 0x60, 0, 0);
        *(int *)(ev + 0x58) = 0;
        code = -2;
        if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
            txt_Format(text, 0x1d5b, *(unsigned short *)a, *(unsigned short *)a);
            he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, (char *)((unsigned)&ai_Unk625820 + 0x1270));
        }
        x = -1;
        r = *(int *)(a + 0x170);
        if (r == 0)
            r = *(int *)(b + 0x170);
        if (r != 0)
            x = *(int *)(r + 1);
        unk_Unk4f2e30(0x16, 1, *(int *)(a + 4), *(int *)(b + 4), x);
        cm_SendRelations(*(int *)(a + 4), *(int *)(b + 4), 0x34, 1);
        n = unk_Unk4f4080(*(int *)(b + 4), arr, *(unsigned short *)a);
        gm_Shuffled(n < 0x10 ? n : 0x10, sel);
        i = 0;
        while (n != 0) {
            ai_Unk4c4f10(arr[sel[i]], *(int *)(a + 4), 0);
            i++;
            n--;
            if (i >= 3)
                break;
        }
        if (*(char *)(a + 2) == 6)
            ai_Unk4b44e0(a, 0xb);
    }
    cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0x100, 0);
    cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0x200, 0);
    cm_SendMoneyDelta(*(int *)(a + 4), code);
}
