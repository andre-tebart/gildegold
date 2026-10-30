/* 0x4ed3f0, callback of an effect slot (p: 3 entries of 4 ints; the entry p[0] says is the one at hand: +8 an object id (0x501190 (1, 1, id)), +0x10 a mode, +0x14 a float): when the object's
   character (+0x27) is a player (kind 6), calls 0x5a9e40 (character, object, 1 or 2 when mode 1 and the float > 25, else 0) */

int unk_Unk4ed3f0(int *p)
{
    int *q;
    int i;
    char *o;
    unsigned short c;
    int m;

    i = 0;
    q = p;
    do {
        if (*p == i) {
            o = (char *)gm_BuildQuery(1, 1, q[2]);
            m = 0;
            if (o != 0) {
                c = *(unsigned short *)(o + 0x27);
                if (c != 0xffff) {
                    if (q[4] == 1) {
                        m = 2;
                        if (*(float *)&q[5] <= 25.0f)
                            m = 1;
                    }
                    if (ai_Chars[c][2] == 6)
                        ai_Unk5a9e40K(ai_Chars[c], o, m);
                }
            }
        }
        i++;
        q += 4;
    } while (i < 3);
    return 0;
}
