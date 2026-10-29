/* 0x4ed170, picks a free avatar record (byte +0xf2 = 0) starting at a random one of the 32 (cyclic), marks it used (+0xf2 = 1) after setting the 14 floats at +0x30, +0x3c .. to the floats before them (+0x2c ..)
   times 0.001, and returns it; 0 when all are in use */

char *gv_AllocAvatar(void)
{
    unsigned i;
    int n;
    int j;
    float *f;

    i = util_Random(0x20) & 0xffff;
    n = 0x20;
    do {
        if (unk_Avatars[i][0xf2] == 0) {
            j = 0xe;
            f = (float *)&unk_Avatars[i][0x30];
            do {
                *f = f[-1] * 0.001;
                f += 3;
                j--;
            } while (j != 0);
            unk_Avatars[i][0xf2] = 1;
            return unk_Avatars[i];
        }
        n--;
        i = (i + 1) & 0x1f;
    } while (n != 0);
    return 0;
}
