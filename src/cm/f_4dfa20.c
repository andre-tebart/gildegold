/* 0x4dfa20, after a duel (the message m: +0x34 / +0x38 the duellists): when one of them is down (life 0x4fc8e0 <= 0) the owner of the
   other gets the text 0x143e8f8 (0x53dda0, kind 0x5ec); when both stand each side with the kind 6 / 7 gets "0x11ca" + the text 0x143e8f4
   (0x4e48b0) and the relation is changed (0x4f2e30); a caught person (+0x1b1) gets the text 0x143e8fc. Returns 0 */

int ai_Unk4dfa20(char *m)
{
    char buf[0x200];
    char *a, *b;
    int flag;

    flag = 0;
    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return 0;
    if (b[0x1b1] == 0 && a[0x1b1] == 0) {
        if (unk_Unk4fc8e0(a) <= 0.0f) {
            if (b[2] != 6 && b[2] != 7)
                return 0;
            he_ShowNews(ai_CharInts[*(unsigned short *)b][0], -1, ai_Unk143e8f8, 0, 0x5ec, 0);
            return 0;
        }
        if (unk_Unk4fc8e0(b) <= 0.0f) {
            if (a[2] != 6 && a[2] != 7)
                return 0;
            he_ShowNews(ai_CharInts[*(unsigned short *)a][0], -1, ai_Unk143e8f8, 0, 0x5ec, 0);
            return 0;
        }
        if (b[2] == 6 || b[2] == 7) {
            txt_Format(buf, 0x11ca, *(unsigned short *)b);
            strcat(buf + 12, ai_Unk143e8f4);
            ai_Unk4e48b0(a, *(int *)(m + 0x10), buf + 12, &ch_EmptyName);
            flag = 1;
        }
        if (a[2] == 6 || a[2] == 7) {
            txt_Format(buf, 0x11ca, *(unsigned short *)a);
            strcat(buf + 12, ai_Unk143e8f4);
            ai_Unk4e48b0(b, *(int *)(m + 0x10), buf + 12, &ch_EmptyName);
            flag = 1;
        }
        if (flag != 0) {
            int ra = -1;

            if (*(int *)(a + 0x170) != 0)
                ra = *(int *)(*(int *)(a + 0x170) + 1);
            else if (*(int *)(b + 0x170) != 0)
                ra = *(int *)(*(int *)(b + 0x170) + 1);
            unk_Unk4f2e30(0x14, 1, *(int *)(a + 4), *(int *)(b + 4), ra);
        }
        return flag;
    }
    if (b[2] == 6 || b[2] == 7)
        he_ShowNews(ai_CharInts[*(unsigned short *)b][0], -1, ai_Unk143e8fc, 0, 0x5ec, 0);
    if (a[2] == 6 || a[2] == 7)
        he_ShowNews(ai_CharInts[*(unsigned short *)a][0], -1, ai_Unk143e8fc, 0, 0x5ec, 0);
    return 0;
}
