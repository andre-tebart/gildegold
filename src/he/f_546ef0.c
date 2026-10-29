/* 0x546ef0, a Ghidra miss (disassembled): the end of the fight event (see 0x546c40): in the state 2 both figures lose the flags 0x100 / 0x200 (+0x1c8) and are told the result by +0xb4 (1 / 2 = the news 0x1e19 / 0x1e20 for b, else the relation -0x68 and the news 0x1d5d / 0x1d5c); a free (the state -1 / -2) only clears the flags (ROUGH) */

void he_Unk546ef0(int ev)
{
    char text[0xf0];
    int a, b, st, r;

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            a = gm_FindChar(*(int *)(ev + 0xac));
            if (a != 0)
                cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x100);
            b = gm_FindChar(*(int *)(ev + 0xb0));
            if (b != 0)
                cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x200);
            he_Unk53e1f0((int *)ev);
        }
        return;
    }
    if (st != 2)
        return;
    if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
        return;
    a = gm_FindChar(*(int *)(ev + 0xac));
    if (a != 0)
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x100);
    b = gm_FindChar(*(int *)(ev + 0xb0));
    if (b != 0)
        cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x200);
    if (a != 0 && b != 0) {
        r = *(int *)(ev + 0xb4);
        if (r == 1) {
            if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
                txt_Format(text, 0x1e19, *(unsigned short *)a);
                he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, (char *)0);
            }
        } else if (r == 2) {
            if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
                txt_Format(text, 0x1e20, *(unsigned short *)a);
                he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, (char *)0);
            }
        } else {
            cm_SendRelations(*(int *)(a + 4), *(int *)(b + 4), -0x68, 1);
            if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
                txt_Format(text, 0x1d5d, *(unsigned short *)a);
                he_ShowNews(*(int *)(b + 4), *(int *)(b + 4), text, 0, 0x5fb, (char *)0);
            }
            if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 7) {
                txt_Format(text, 0x1d5c, *(unsigned short *)b);
                he_ShowNews(*(int *)(b + 4), *(int *)(b + 4), text, 0, 0x5fb, (char *)0);
            }
        }
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
