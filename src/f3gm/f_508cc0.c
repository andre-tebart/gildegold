/* 0x508cc0, the objects a building b (kind b[0]) lists after the object word *proto in its object words (+0x23): counts them up to the first kind 2 / 6 object (kind 0x21 not counted), stores their ids at out + 4 * n */

int gm_Unk508cc0(unsigned char *b, unsigned short *proto, int *out)
{
    char *bp;
    int n, i, cnt;
    unsigned short *w;
    unsigned id;
    char k;

    bp = ai_BTypes + *b * 0x24d;
    n = 0;
    i = 0;
    cnt = (unsigned char)bp[0x22];
    if (cnt != 0) {
        w = (unsigned short *)(bp + 0x23);
        do {
            if ((*w & 0x7fff) == *proto)
                break;
            i++;
            w++;
        } while (i < cnt);
    }
    if (cnt <= i)
        return 0;
    i++;
    if (i < 0x40) {
        w = (unsigned short *)(bp + 0x23 + i * 2);
        while (*w != 0) {
            id = *w & 0x7fff;
            k = ai_Goods[id * 0x41];
            if (k == 2)
                return n;
            if (k == 6)
                return n;
            if (k != 0x21)
                n++;
            if (out)
                out[n] = id;
            i++;
            w++;
            if (i > 0x3f)
                return n;
        }
    }
    return n;
}
