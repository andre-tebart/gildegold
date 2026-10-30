/* 0x4c3560, as 0x4c3480 without the start */

void ai_ResetMessages(void)
{
    int i;
    G17STAT *s;

    for (i = 0; i < 0x2000; i++) {
        ai_Unk9367e0[i].type = 0;
        if (i == 0) {
            ai_Unk9367e0[0].prev = 0;
            ai_Unk9367e0[0].next = &ai_Unk9367e0[1];
        } else {
            ai_Unk9367e0[i].prev = &ai_Unk9367e0[i - 1];
            if (i < 0x1fff)
                ai_Unk9367e0[i].next = &ai_Unk9367e0[i + 1];
            else
                ai_Unk9367e0[i].next = 0;
        }
    }
    ai_Unka68c00 = ai_Unk9367e0;
    ai_Unka68d24 = 0;
    ai_Unkf81180.type = 0;
    ai_Unkf81168U = 0;
    cm_SendQueue = 0;
    for (s = ai_Unkf31160; s < ai_Unkf31160 + 0x8000; s++) {
        s->idx = -1;
        s->state = 1;
    }
}
