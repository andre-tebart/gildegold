/* 0x4cbbd0, handler of type 0x4b: a chat text (data[0x20]) is appended to ai_Unk75fba0 (reset after 5 lines) when
   it is for us (the player ids data[0..0x1f]) or sent by ourselves (s != 0) */

int ai_Unk4cbbd0(G17MSG *m, G17STAT *s)
{
    char *p;
    int n = 0;
    int found = 0;
    int i;

    p = strstr(ai_Unk75fba0, "$A");
    if (p != 0) {
        do {
            p += 2;
            n++;
            p = strstr(p, "$A");
        } while (p != 0);
        if (n > 5)
            ai_Unk75fba0[0] = 0;
    }
    for (i = 0; i < 8; i++) {
        if (*(int *)&m->data[i * 4] == ai_CharInts[ai_PlayerId][0]) {
            found = 1;
            break;
        }
    }
    if (s == 0 && !found)
        return 0;
    strcat(ai_Unk75fba0, (char *)&m->data[0x20]);
    ai_Unk623ec4 = d3_Time;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
