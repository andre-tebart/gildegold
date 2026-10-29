/* 0x50acf0, the same for the stall list of the object l */

int gm_Unk50acf0(char *l, int id, int n)
{
    int cap, in, cnt, t;

    cap = gm_ListStock((short *)l, id);
    in = (int)gm_Query(*(void **)(l + 0x14), 1, 0, id);
    if (in == 0) {
        cnt = 0;
        t = (int)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (t != 0) {
            cnt++;
            t = (int)gm_QueryNext();
        }
        if (*(unsigned char *)(l + 0x1c) <= cnt)
            return 0;
        if (cap < n)
            n = cap;
    } else {
        cap = cap - *(int *)(in + 0xe);
        if (cap <= n)
            return cap;
    }
    return n;
}
