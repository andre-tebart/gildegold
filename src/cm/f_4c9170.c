/* 0x4c9170, handler of type 0x26, "cm_ExSendCutInfo": the 0x35c bytes of the joined message go to the cutscene
   slot of player data[0] */

int ai_Unk4c9170(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    char *rec;
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == G17_I(*m, 4)) {
            rec = ai_Unk92cae0[i];
            memcpy(rec, ai_Unka68d40, 0x35c);
            *(int *)(rec + 0xc) = 0;
            sprintf(buf, "cm_ExSendCutInfo: PlayerId:%i Valid:%i  Ready:%i  Time:%i", *(int *)(rec + 0x10),
                    *(int *)(rec + 8), *(int *)(rec + 4), d3_Time);
        }
    }
    if (i >= 16) {
        if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == G17_I(*m, 4))
            sprintf(buf, "cm_ExSendCutInfo(): io-buffer has not been written, because player not found (player id %i)",
                    G17_I(*m, 0));
        return 1;
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = (int)ai_Unk92cae0[i];
    }
    return 0;
}
