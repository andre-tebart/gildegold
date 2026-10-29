/* 0x4f3780, the slot for the pair (a, b) in the table 0x91b6e0 (2048 pairs): -2 when it exists already, else the first free one or -1 */

int unk_Unk4f3780(int a, int b)
{
    int ok, r, i;
    int *t;

    ok = 0;
    r = -1;
    i = 0;
    t = (int *)unk_Tab91b6e0;
    while (1) {
        if (!ok && t[i * 2] == -1 && t[i * 2 + 1] == -1) {
            r = i;
            ok = 1;
        }
        if (t[i * 2] == a && t[i * 2 + 1] == b)
            break;
        i++;
        if (i >= 0x800) {
            if (ok)
                return r;
            return -1;
        }
    }
    return -2;
}
