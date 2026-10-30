/* 0x522040, the image number of the panel for the player's group (0x6f3668, or when 0: the kind number 0x504ad0 of the first player character): 0x534 (1, 2, 10), 0x538 (3, 4), 0x53c (0xb, 0xc, 6), 0x540 (5, 7, 8, 9); 0x534 else */

int sv_PanelImage(void)
{
    int i, k;
    char *p;
    int is9;

    if (ai_GamePanel.byte[0] == 0) {
        i = 0;
        for (p = ai_Chars[0] + 2; (int)p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            if (*p == 6 || *p == 7)
                break;
            i++;
        }
        k = gm_TitleGroup((unsigned char)ai_Chars[i][0x164]);
        if (k == 1 || k == 2 || k == 10)
            return 0x534;
        if (k == 3 || k == 4)
            return 0x538;
        if (k == 0xb || k == 0xc || k == 6)
            return 0x53c;
        if (k == 5)
            return 0x540;
        if (k == 7)
            return 0x540;
        if (k == 8)
            return 0x540;
        is9 = k == 9;
    } else {
        if (ai_GamePanel.byte[0] == 1)
            return 0x534;
        if (ai_GamePanel.byte[0] == 2)
            return 0x538;
        if (ai_GamePanel.byte[0] == 3)
            return 0x53c;
        is9 = ai_GamePanel.byte[0] == 4;
    }
    if (is9)
        return 0x540;
    return 0x534;
}
