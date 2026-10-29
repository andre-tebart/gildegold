/* 0x55bd80, the run handler "GebAbreissen" (the demolition of a building, 4 states by the jump table 0x55bf40; ROUGH): state 0 sends the figures of the building away (0x4c5b90, 0x4c52e0 (.., "GebAbreissen")) and waits, the later state demolishes (0x4c5510) after the news for the kind 0xf (0x53dda0) and frees the record */

void he_Unk55bd80(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, c, k;
    char text[0x200];

    if (st < 0 || st > 3) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); c += 0x21c) {
            if (*(int *)(c + 0x168) == b && *(int *)(c + 0x180) != 0) {
                ai_Unk4c5b90(*(int *)c);
                ai_Unk4c52e0(*(int *)c, -1, -1, 0, 0, "GebAbreissen");
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = st + 1;
        return;
    default:
        k = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
        if (k == 0xf) {
            txt_Format(text, 0x1bbb, b);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
        }
        ai_Unk4c5510(b);
        he_Unk53e1f0((int *)ev);
        return;
    }
}
