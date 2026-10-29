/* 0x4a5a30: one partner for an action with the flags fl and the chain ids[n]: the leader ai_Partner when
   0x5c33c0 accepts it, else the first ring entry (0x4a3a90) rated in [lo, hi] that it accepts; into *id */

int ai_Unk4a5a30(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    G6RING *e;
    unsigned int len;
    int k;

    memset(q, 0, sizeof(q));
    len = n * 2;
    if (len > 0x20)
        len = 0x20;
    memcpy(&q[7], ids, len);
    q[5] = 2;
    if (ai_Partner != 0 && !(q[1] & 0x10000000)) {
        q[16] = 0;
        q[1] = fl;
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            *id = *(unsigned short *)ai_Partner;
            return 1;
        }
    }
    q[16] = 0;
    for (k = ai_Unk6f070c; ; k--) {
        e = ai_Unk4a3a90();
        if (k == 0 || e->who == 0)
            break;
        if (lo <= e->dist && e->dist <= hi
            && ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)e->who, q, 0) != 0) {
            *id = *(unsigned short *)e->who;
            return 1;
        }
    }
    return 0;
}
