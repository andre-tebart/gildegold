/* 0x4cf000, the per-tick update of the characters (ai_Unk7603a0[]): the counter byte +8 of each counts down (when
   the clock of the player record ai_Unk6f0d74 matches, every 0x15e ticks), the record 0x172's characters are
   shaded by their distance to the others (rest 255), characters at a cutscene slot turn towards their partner;
   ends with the floating numbers (0x4cee20). Rough draft. */

void ai_Unk4cf000()
{
    char **pp, **qq;
    char *c, *d, *r, *mdl, *e, *o, *g;
    int *rr;
    int fl, found, n, v, t;
    unsigned int col;
    float best, dist, ang;
    unsigned char b;

    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && d3_Time % 0x15e == 0) {
        for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            b = c[8];
            if (b > 4 || b <= 0)
                continue;
            b--;
            c[8] = b;
            if (b == 1) {
                c[8] = 100;
                continue;
            }
            r = ai_Unk4cd9d0(0x16e);
            n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
            v = (int)((double)(unsigned int)(n + (unsigned char)r[5]) * *(float *)(c + 0x1c) * 0.01);
            ai_Unk4c5d00(*(int *)(c + 4), v, (unsigned char)c[8]);
        }
    }
    for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        c = *pp;
        if (c == 0)
            continue;
        rr = (int *)ai_Unk4cda50(c);
        if (rr != 0 && *rr == 0x172) {
            fl = 0;
            if (ai_Unk6f0d74 != 0)
                fl = ai_Unk4ce170(*(unsigned short *)ai_Unk6f0d74, c);
            if (fl & 0x800) {
                col = 0x80;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                d3if_ShowTree(mdl, 1);
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                mdl[0x212] &= 0xfd;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                ((void (*)(void *, void *, int))d3_SetObjectAlpha)(mdl, (void *)(mdl + 0x1cc), col);
            } else {
                best = 999999.0f;
                found = 0;
                for (qq = (char **)ai_Unk7603a0V; (int)qq < (int)ai_Unk760420V; qq++) {
                    d = *qq;
                    if (d != 0 && ai_Unk4ce270(c, d) != 0 && d[8] != 0) {
                        dist = ai_Unk4ce1b0(c, *(char **)(*(char **)(d + 0x184) + 0x34));
                        if (dist < 100.0f) {
                            found = 1;
                            if (dist < best)
                                best = dist;
                        }
                    }
                }
                d3if_ShowTree(*(char **)(*(char **)(c + 0x184) + 0x34), found);
                if (found == 0)
                    goto next;
                col = 0;
                t = (int)(255.0f - best * 0.01f * 255.0f);
                col = (unsigned char)t;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                mdl[0x212] &= 0xfd;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                ((void (*)(void *, void *, int))d3_SetObjectAlpha)(mdl, (void *)(mdl + 0x1cc), col);
            }
            mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
            mdl[0x212] |= 2;
        }
next:
        rr = (int *)ai_Unk4cdf80(c);
        if (rr != 0 && rr[1] == 2 && *((char *)rr + 0x1d) != 0) {
            o = ai_Unk4ce130(rr[4]);
            if (o != 0) {
                if (d3_VecEqual((float *)(*(char **)(*(char **)(o + 0x184) + 0x34) + 0x4c),
                                 (float *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c), 10.0f) == 0) {
                    ang = d3_YawTo(*(char **)(*(char **)(c + 0x184) + 0x34), *(char **)(*(char **)(o + 0x184) + 0x34) + 0x4c);
                    g = *(char **)(c + 0x184);
                    if (*(char **)(g + 0x128) == 0 || ((*(char **)(g + 0x128))[9] != 0x2d && (*(char **)(g + 0x128))[9] != 0x3a)) {
                        if (fabs(ang) <= 0.08) {
                            mdl = *(char **)(g + 0x34);
                            d3_SetObjectRotXYZ(mdl, *(int *)(mdl + 0x84), *(float *)(mdl + 0x88) + ang * 0.25, *(int *)(mdl + 0x8c));
                        }
                    }
                }
            }
        }
    }
    ai_Unk4cee20();
}
