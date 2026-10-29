/* 0x552270, the run handler of the two-stage event of 0x552200 (ROUGH): state 0 posts a message record of 0xf8 bytes (type 0x7e, the figure +0xac, the clocks) through 0x4fe140 and 0x4c4b70 and the event 8 later (state 1); state 1: with +0xb0 set the figure +8 pays (0x504ec0 (.., 3) clamped to 0.1 scales the amount, 0x50a640 / 0x4fc820 give the base, 0x4fca40 (id, -amount) and 0x4c4830 (+0xac, -1, player, amount)) and the player characters read the news 0x10ba, without it the news 0x10bb; then the record is freed */

void he_Unk552270(int ev)
{
    int st = *(int *)(ev + 0x70);
    char text[0x100];
    unsigned char m[0xf8];
    int ch, x, v, pay;
    float f, t;

    if (st < 0 || st > 1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        memset(m, 0, sizeof(m));
        *(int *)(m + 0x10) = *(int *)(ev + 0xac);
        *(int *)(m + 0x60) = *(int *)(ev + 0xac);
        m[0x18] = 0x7e;
        *(int *)(m + 0x20) = -1;
        *(int *)(m + 0x30) = *(int *)(unsigned)&ai_Time;
        *(int *)(m + 0x68) = *(int *)(unsigned)&ai_Time;
        *(int *)(m + 0x40) = *(int *)(unsigned)&ai_TimeHour;
        *(int *)(m + 0x44) = *(int *)((unsigned)&ai_Time + 0x8);
        *(unsigned short *)(m + 0x48) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        m[0x4a] = 9;
        *(int *)(m + 0x70) = *(int *)(ev + 4);
        *(int *)(m + 0x78) = *(int *)(unsigned)&ai_TimeHour;
        *(int *)(m + 0x7c) = *(int *)((unsigned)&ai_Time + 0x8);
        *(unsigned short *)(m + 0x80) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        gm_AddTime(m + 0x6c, 2, 0, 0);
        cm_SendLongMessage(m + 0x14);
        *(int *)(ev + 0x70) = 1;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 8, 0, 0);
        return;
    }
    if (*(int *)(ev + 0xb0) != 0) {
        ch = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
        if (gm_GetSkill(ch, 3) > 0.1f)
            f = gm_GetSkill(ch, 3);
        else
            f = 0.1f;
        x = gm_GetWorth(*(unsigned short *)ch);
        v = (int)gm_CharacterHealth(ch);
        t = (float)v * (*(float *)(unsigned)&ai_Unk5eb780 - f) * 0.5f;
        gm_AddHealth(*(unsigned short *)ch, -(int)t);
        pay = (int)((float)x * f * (0.0599999987f));
        cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, pay);
        txt_Format(text, 0x10ba, pay);
        he_ShowNews(*(int *)(ev + 0xac), -1, text, 0x20, 0x5fd, (unsigned)&ch_EmptyName);
    } else {
        txt_Format(text, 0x10bb);
        he_ShowNews(*(int *)(ev + 0xac), -1, text, 0x20, 0x5fd, (unsigned)&ch_EmptyName);
    }
    he_Unk53e1f0((int *)ev);
}
