/* 0x4e0580, announces a wedding (the message m: +0x34 / +0x38 the two, of different sex): the text 0x11c3 for the first (0x4e48b0), the text
   0x11c4 (both names) to every character of the kinds 6 / 7 who is neither of them (0x53dda0); 1 when the first is a person of the kinds
   6 / 7, else the two get the state 0x1c8 (0x4c4a60) */

int ai_Unk4e0580(char *m)
{
    char buf[0x200];
    char *a, *b;
    int i;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a != 0) {
        if (b != 0 && a[9] != b[9]) {
            if (a[2] == 6 || a[2] == 7) {
                txt_Format(buf, 0x11c3, *(unsigned short *)b);
                ai_Unk4e48b0(a, m, buf, "_PA_WERBUNG_EVENT_ZUSAGE_M1_00");
            }
            if (a[2] == 6 || a[2] == 7 || a[2] == 5) {
                txt_Format(buf, 0x11c4, *(unsigned short *)a, *(unsigned short *)b);
                for (i = 0; i < 768; i++) {
                    if ((ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) && ai_CharInts[i][0] != *(int *)(a + 4) &&
                        ai_CharInts[i][0] != *(int *)(b + 4))
                        he_ShowNews(ai_CharInts[i][0], -1, buf, 0, 0x5fe, "_PA_WERBUNG_EVENT_NACHRICHT_HOCHZEIT_M1_00");
                }
            }
            if (a[2] == 6 || a[2] == 7)
                return 1;
        }
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000);
    }
    if (b != 0)
        cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x40000);
    return 0;
}
