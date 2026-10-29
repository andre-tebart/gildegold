/* 0x508d80, the income / cost summary (0x15 ints, out[3] wages, out[0x10] cost, out[0x11] income, out[0x14] profit) of the building b for the owner id; 0 when both are zero. ROUGH */

int gm_Unk508d80(unsigned char *b, unsigned id, int *out)
{
    char *own;
    char *bp;
    unsigned char *c;
    int i, k, w, in, t, r, u, n, found;
    int *pi;
    char *o;

    for (i = 0; i < 0x15; i++)
        out[i] = 0;
    w = 0;
    in = 0;
    bp = ai_BTypes + *b * 0x24d;
    t = 0;
    if (id == *(unsigned short *)(b + 0x27)) {
        own = ai_Chars[id];
        if (*bp == 0x1e) {
            n = w;
            if (ahm_OwnerChar(b) != 0) {
                n = ahm_ObjectValue(b);
                out[6] = n;
            }
            w = n;
        } else {
            c = (unsigned char *)ai_Chars[0];
            do {
                if (*(unsigned char **)(c + 0x16c) == b) {
                    found = 0;
                    k = 0;
                    pi = out + 6;
                    do {
                        if (bp[k + 0x223] != 0 && bp[k + 0x223] == (char)c[0x164] && (char)c[2] != 0x14) {
                            r = (int)gm_Unk508740(own, *b, k, -1);
                            found = 1;
                            in += r;
                            *pi += r;
                        }
                        k++;
                        pi++;
                    } while (k < 6);
                    k = 0;
                    pi = out + 4;
                    do {
                        if (bp[k + 0x22f] != 0 && bp[k + 0x22f] == (char)c[0x165] && (char)c[2] != 0x14) {
                            r = (int)gm_Unk508740(own, *b, -1, k);
                            found = 1;
                            in += r;
                            *pi += r;
                        }
                        k++;
                        pi++;
                    } while (k < 2);
                    if (!found && (char)c[2] != 0x14) {
                        r = (int)gm_Unk508740(own, *b, -1, 0);
                        in += r;
                        out[4] += r;
                    }
                }
                c += 0x21c;
                w = in;
            } while ((int)c < (unsigned)((unsigned)ai_Chars + 0x65400));
        }
        out[3] = w;
        out[0x12] = *(int *)(b + 0x4d);
        if (*bp == 5) {
            out[10] = *(int *)(b + 0x55);
            t = *(int *)(b + 0x55);
        }
        if (*bp == 7) {
            o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0xe8);
            if (o) {
                r = gm_Unk509e30(*(void **)(o + 0x14), ai_OwnPlayer);
                u = out[0xc];
                out[0xc] = u + r;
                t = t + u + r;
            }
            o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0xf0);
            if (o) {
                r = gm_Unk509e30(*(void **)(o + 0x14), ai_OwnPlayer);
                u = out[0xf];
                out[0xf] = u + r;
                t = t + u + r;
            }
            k = *(int *)(b + 0x65);
            r = out[0xd];
            u = out[0xe];
            out[0xd] = r + k;
            i = *(int *)(b + 0x69);
            out[0xe] = u + i;
            t = t + r + k + u + i;
        } else if (*bp == 0x16) {
            t += *(int *)(b + 0x6d);
            out[0x13] = *(int *)(b + 0x6d);
        }
    }
    out[0x10] = w;
    out[0x11] = t;
    out[0x14] = t - w;
    if (w == 0 && t == 0)
        return 0;
    return 1;
}
