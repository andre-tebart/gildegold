/* 0x4c9110, handler of type 0x25: clears the entry (data[4], data[0]) of ai_Unk91b6e0 */

int ai_Unk4c9110(G17MSG *m, G17STAT *s)
{
    int i;

    for (i = 0; i < 0x800; i++) {
        if (ai_Unk91b6e0[i][0] == G17_I(*m, 4) && ai_Unk91b6e0[i][1] == G17_I(*m, 0)) {
            ai_Unk91b6e0[i][0] = -1;
            ai_Unk91b6e0[i][1] = -1;
            if (s != 0)
                s->state = 1;
            return 0;
        }
    }
    return 1;
}
