/* 0x4a58d0: picks a partner chain of a (< 4) characters for an action with the flags flags[i]: the leader
   ai_Partner first when 0x5c33c0 accepts it, then for each further slot the next ring entry (0x4a3a90) whose
   rating lies in [b, f] and that 0x5c33c0 accepts with the chain so far; ids into id[] */

int ai_Unk4a58d0(AIPER *p, int a, int *flags, int b, float f, unsigned short *id)
{
    unsigned int q[0x14];
    G6RING *e;
    unsigned int i, n;
    int found, ret;

    ret = 0;
    if ((unsigned int)a < 4) {
        memset(q, 0, sizeof(q));
        i = 0;
        memset(&q[7], 0xff, 8 * 4);
        q[5] = 2;
        if (ai_Partner != 0 && !(q[1] & 0x10000000)) {
            q[16] = 0;
            q[1] = flags[0];
            if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
                i = 1;
                id[0] = *(unsigned short *)ai_Partner;
            }
        }
        for (; i < (unsigned int)a; i++) {
            q[1] = flags[i];
            if (i == 0)
                *(unsigned short *)&q[7] = *(unsigned short *)p;
            else
                ((unsigned short *)&q[7])[i] = id[i - 1];
            found = 0;
            e = ai_Unk4a3a90();
            for (n = ai_Unk6f070c; !found; n--, e = ai_Unk4a3a90()) {
                if (n == 0) {
                    if (!found)
                        return 0;
                    break;
                }
                if (e->who == 0)
                    return 0;
                if (*(float *)&b <= e->dist && e->dist <= f
                    && ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)e->who, q, 0) != 0) {
                    found = 1;
                    id[i] = *(unsigned short *)e->who;
                }
            }
        }
        ret = 1;
    }
    return ret;
}
