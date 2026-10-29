/* 0x5014b0, sets the capacity bytes of the building b objects from its kind record (+0x23d / +0x23e / +0x23f): the object 0x2a gets +0x1c = the first (at least 2) and +0x1d = the second (at least 2), or just the third when the first two are 0; the object 0x116 gets the third in +0x1c in that case */

void gm_SetCapacities(unsigned char *b)
{
    char *k;
    int o;
    unsigned char c;

    k = ai_BTypes + *b * 0x24d;
    o = (int)gm_Query(*(int *)(b + 0x5d), 2, 0, 0x2a, 6);
    if (o != 0) {
        c = k[0x23d];
        if (c == 0 && k[0x23e] == 0 && k[0x23f] != 0) {
            *(char *)(o + 0x1c) = k[0x23f];
        } else {
            if (c < 2)
                c = 2;
            *(unsigned char *)(o + 0x1c) = c;
            c = k[0x23e];
            if (c < 2)
                c = 2;
            *(unsigned char *)(o + 0x1d) = c;
        }
    }
    o = (int)gm_Query(*(int *)(b + 0x5d), 2, 0, 0x116, 6);
    if (o != 0 && k[0x23d] == 0 && k[0x23e] == 0 && k[0x23f] != 0)
        *(char *)(o + 0x1c) = k[0x23f];
}
