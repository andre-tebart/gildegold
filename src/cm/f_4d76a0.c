/* 0x4d76a0, the fight commands of one fighter record: 16 slots of 0x2c bytes at rec + 0x94 (kind, state, request, ...); each kind
   (1 walk, 2 attack, 3 .., 4 follow, 5 stand, 6 flee / goods, 7, 8) sends its messages. rough draft from the disassembly */

#define G21_BC0 ((int (*)(const char *, int, int, const char *))ai_Unk4c5bc0)
#define G21_C40 ((int (*)(int, void *))ai_Unk4c5c40)
#define G21_CA0 ((int (*)(int, void *))ai_Unk4c5ca0)
#define G21_DC0 ((int (*)(char *, int, void *))ai_Unk4c5dc0)
#define G21_CH(o) (*(char **)((o) + 0x184))
#define G21_POS(o) (*(char **)(G21_CH(o) + 0x34) + 0x4c)

void ai_Unk4d76a0(int *rec)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int m[11];
    char buf[48];
    char *c, *k, *x, *other, *r2, *nm;
    int *p;
    int slot, i, n, res, hit, dmg, ix, iz, id, b;
    float f, q[4];
    unsigned short r;
    int tmp[3];

    for (slot = 0; slot < 16; slot++) {
        if (ai_Unk6f0cf4 != 0)
            return;
        p = rec + slot * 11 + 0x25;
        memcpy(m, p, 0x2c);
        if (p[1] != 0 && p[0] != -1) {
            if (p[1] == 5)
                c = gm_FindChar(0);
            else
                c = ai_Unk4ce130(p[0]);
            k = ai_Unk4cda50(c);
            if ((c == 0 || c[8] == 0) && k != 0) {
                p[0] = -1;
                p[1] = 0;
                return;
            }
            switch (p[1]) {
            case 1:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0) {
                        if (ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                            sprintf(buf, "cmb_%i", *(int *)(c + 4));
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_BC0(buf, ix, iz, 0);
                            goto done;
                        }
                    } else {
                        d3sm_GetVertex(h, p[4], p[5], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 30.0f) != 0) {
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            ai_Unk4c5dc0(c, 6, m);
                            goto send;
                        }
                    }
                }
                break;
            case 2:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    x = ai_Unk4ce130(p[4]);
                    if (x != 0 && (x[8] == 0 || *(int *)(*(char **)(G21_CH(x) + 0x34) + 0x218) == 1 || c == 0 || c[8] == 0)) {
                        m[0] = *(int *)(c + 4);
                        id = 6;
                        goto stop;
                    }
                    if ((*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) && (other = ai_Unk4cdac0(c)) != 0 && *(int *)(other + 0x20) < 1) {
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                    b = 1;
                    if (*(int *)k == 0x174) {
                        b = 0;
                        if (((char *)p)[0x1d] == 0 && (char)p[3] == 0) {
                            if (ai_Unk4ce040(p[5], p[6], &ix, &iz) != 0) {
                                p[5] = ix;
                                p[6] = iz;
                                ai_Unk4c5bc0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                            }
                            ((char *)p)[0xc] = 1;
                        } else if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[3] != 0) {
                            d3sm_GetVertex(h, p[5], p[6], q);
                            b = d3_VecEqual(q, (float *)G21_POS(c), 120.0f) != 0;
                        }
                    } else {
                        if (x == 0)
                            f = ai_Unk4ce200(c, p[5], p[6]);
                        else
                            f = ai_Unk4ce1b0(c, *(char **)(G21_CH(x) + 0x34));
                        if (*(int *)k == 0x176) {
                            b = f <= *(float *)(k + 0xc);
                            if ((char)p[3] == 0 && f < *(float *)(k + 0xc) * 0.1f)
                                b = 0;
                        } else if (!(f <= *(float *)(k + 0xc)))
                            b = 0;
                    }
                    if ((b != ((char *)p)[0x1d] || *(int *)(G21_CH(c) + 0x128) == 0) && (((char *)p)[0x1d] = b, b != 0)) {
                        if (*(int *)(k + 0x58) != 2) {
                            r = util_Random(0xff);
                            if (0xff - (int)((unsigned char)k[4] * 0.01 * (unsigned char)c[0x83]) < (int)r)
                                hit = 1;
                            else
                                hit = 0;
                        } else
                            hit = 1;
                        if (hit != 0) {
                            n = ai_Unk4e52f0((unsigned char)k[6] - (unsigned char)k[5]) + (unsigned char)k[5];
                            if (x != 0)
                                dmg = (int)((double)(unsigned int)n * *(float *)(x + 0x1c) * 0.01);
                            else
                                dmg = (int)((double)(unsigned int)n * 9.0);
                        } else
                            dmg = 0;
                        p[8] = hit;
                        p[9] = dmg;
                        if (hit != 0 && *(int *)(c + 0x16c) != 0 && ai_InventoryHas(*(int *)(c + 0x16c), 0x2b9) != 0)
                            p[9] = (int)(p[9] * 1.25f);
                        if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && *(int *)(k + 0x58) == 0 && x != 0) {
                            other = 0;
                            n = (unsigned char)ai_Unk6f0d04[0x30];
                            for (i = 0; i < n; i++) {
                                if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1 && (char *)rec != ai_Unk92cae0[i]) {
                                    other = ai_Unk92cae0[i];
                                    break;
                                }
                            }
                            r2 = (char *)ai_Unk4cdf00(other, x);
                            if ((*(int *)(r2 + 8) == -1 || cm_GetSlotState(*(int *)(r2 + 8)) != 0) &&
                                (*(int *)(r2 + 8) = -1, *(int *)(r2 + 4) == 0) &&
                                ai_Unk4cdac0(x) != 0 && *(int *)(ai_Unk4cdac0(x) + 0x20) > 0) {
                                cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                                *(int *)(r2 + 8) = ai_Unk4cffd0(other, x, c, 0, 0);
                            }
                        }
                    }
                    if (*(int *)(k + 0x58) == 0) {
                        if (((char *)p)[0x1d] == 0) {
                            if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[7] == 0) {
                                if (d3_GetHeightAt(h, G21_POS(x), &ix, &iz, tmp) == 0)
                                    break;
                                p[5] = ix;
                                p[6] = iz;
                                sprintf(buf, "cmb_%i", *(int *)(c + 4));
                                res = G21_BC0(buf, ix, iz, *(char **)(G21_CH(x) + 0x34));
                                p[2] = res;
                            }
                            goto test1d;
                        }
                        goto attack;
                    } else {
                        if (*(int *)(k + 0x58) == 1 || *(int *)k == 0x176) {
                            if (((char *)p)[0x1d] != 0)
                                goto attack;
                            if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[7] == 0) {
                                if (d3_GetHeightAt(h, G21_POS(c), &q[2], &q[3], tmp) == 0 ||
                                    (x != 0 && d3_GetHeightAt(h, G21_POS(x), p + 5, p + 6, tmp) == 0))
                                    break;
                                if (ai_Unk4d2170(c, p[5], p[6], 0, &ix, &iz) != 0) {
                                    res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                    p[2] = res;
                                }
                            }
                        }
test1d:
                        if (((char *)p)[0x1d] != 0)
                            goto attack;
                    }
                    if ((*(int *)k == 0x16e || *(int *)k == 0x172) && p[8] != 0 &&
                        d3_GetHeightAt(h, G21_POS(c), &ai_Unkfa5f00, &ai_Unkfa5f04, tmp) != 0) {
                        if (ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 0xc, &ix, &iz) != 0)
                            p[2] = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                }
                break;
attack:
                if (*(int *)(G21_CH(c) + 0x128) == 0) {
                    if (*(int *)(k + 0x58) == 2 && p[8] != 0) {
                        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                        ai_Unk4c5dc0(c, 2, p);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        if (d3_GetHeightAt(h, G21_POS(c), &ai_Unkfa5f00, &ai_Unkfa5f04, tmp) == 0)
                            break;
                        if (ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 0xc, &ix, &iz) != 0)
                            p[2] = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                        res = G21_C40(rec[4], m);
                    } else {
                        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                        res = G21_DC0(c, 2, p);
                    }
                    p[2] = res;
                }
                break;
            case 3:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0 && d3_VecEqual(q, (float *)G21_POS(c), 50.0f) == 0 &&
                        ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                        sprintf(buf, "cmb_%i", *(int *)(c + 4));
                        ((char *)p)[0xc] = 1;
                        p[2] = G21_DC0(c, 6, m);
                        p[2] = G21_BC0(buf, ix, iz, 0);
                    }
                    d3sm_GetVertex(h, p[4], p[5], q);
                    if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                        ai_Unk4c5dc0(c, 1, m);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                }
                break;
            case 4:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    id = d3_FindObject(0, 0x40, 0, p + 4);
                    n = 0;
                    if (id != 0) {
                        n = *(int *)(id + 0x200);
                        d3_PointToWorld(id, id + 0x4c, q);
                        if (n == 0 || (ai_Unk4ce270(c, (char *)n) != 0 &&
                                       (*(char *)(n + 8) == 0 || d3_VecEqual(q, (float *)G21_POS((char *)n), 49.5f) == 0))) {
                            if ((char)p[3] == 0 && d3_VecEqual(q, (float *)G21_POS(c), 49.5f) == 0 &&
                                ai_Unk4ce040(p[8], p[9], &ix, &iz) != 0) {
                                ((char *)p)[0xc] = 1;
                                p[8] = ix;
                                p[9] = iz;
                                ai_Unk4c5dc0(c, 6, m);
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                            if ((char)p[3] == 1 && *(int *)(G21_CH(c) + 0x128) == 0 && d3_VecEqual(q, (float *)G21_POS(c), 49.5f) == 0)
                                ((char *)p)[0xc] = 0;
                            else if (d3_VecEqual(q, (float *)G21_POS(c), 49.5f) != 0) {
                                ai_Unk4c5dc0(c, 3, m);
                                m[0] = *(int *)(c + 4);
                                m[1] = 0;
                                res = G21_C40(rec[4], m);
                                goto done;
                            }
                            break;
                        }
                    }
                    d3_PointToWorld(id, id + 0x4c, q);
                    if (ai_Unk4ce270(c, *(char **)(id + 0x200)) != 0 && *(char *)(n + 8) != 0) {
                        if (d3_VecEqual(q, (float *)G21_POS((char *)n), 49.5f) != 0) {
                            res = ai_Unk4d7360((char *)rec, c, *(char **)(id + 0x200));
                            goto done;
                        }
                    }
                    if (*(char *)(n + 8) != 0)
                        break;
                    m[0] = *(int *)(c + 4);
                    m[1] = 0;
                    res = G21_C40(rec[4], m);
                    goto done;
                }
                break;
            case 5:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    m[0] = *(int *)(c + 4);
                    id = 7;
stop:
                    m[1] = 0;
                    ai_Unk4c5dc0(c, id, m);
                    res = G21_C40(rec[4], m);
                    goto done;
                }
                break;
            case 6:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if (d3_FindObject(0, 0x40, 0, p + 4) == 0) {
                        ai_Unk4c5dc0(c, 6, m);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        p[2] = G21_C40(rec[4], m);
                    }
                    n = *(int *)(G21_CH(c) + 0x128);
                    switch (((char *)p)[0x22]) {
                    case 0:
                        if (n == 0) {
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            if (ai_Unk4ce040((char)p[8], ((char *)p)[0x21], &ix, &iz) != 0) {
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                        } else {
                            d3sm_GetVertex(h, (char)p[8], ((char *)p)[0x21], q);
                            if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                                ai_Unk4c5dc0(c, 4, m);
                                ((char *)m)[0x2a] = 1;
                                res = G21_CA0(rec[4], m);
                                goto done;
                            }
                        }
                        break;
                    case 1:
                        if (n == 0) {
                            nm = *(int *)(c + 0x16c) == ai_Unk6f0cec ? "dummy_sp_OFF_WARENSAVE" : "dummy_sp_DEF_WARENSAVE";
                            id = d3_FindObject(0, 0x100, 0, nm);
                            d3_PointToWorld(id, id + 0x4c, q);
                            if (d3_GetHeightAt(h, q, &ix, &iz, tmp) != 0 && ai_Unk4ce040(ix, iz, &ix, &iz) != 0) {
                                ((char *)m)[0x28] = (char)ix;
                                ((char *)m)[0x29] = (char)iz;
                                ((char *)m)[0x2a] = 2;
                                ai_Unk4c5ca0(rec[4], m);
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                        }
                        break;
                    case 2:
                        d3sm_GetVertex(h, (char)p[8], ((char *)p)[0x21], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                            ai_Unk4c5dc0(c, 5, m);
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            res = G21_C40(rec[4], m);
                            goto done;
                        }
                        break;
                    }
                }
                break;
            case 7:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0) {
                        if (ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                            sprintf(buf, "cmb_%i", *(int *)(c + 4));
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_BC0(buf, ix, iz, 0);
                            goto done;
                        }
                    } else {
                        d3sm_GetVertex(h, p[4], p[5], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 30.0f) != 0) {
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_C40(rec[4], m);
                            goto done;
                        }
                    }
                }
                break;
            case 8:
                if ((p[2] == 1 || cm_GetSlotState(p[2]) != 0) && (p[2] = -1, (char)p[3] == 0)) {
                    ((char *)p)[0xc] = 1;
                    res = G21_DC0(c, 6, m);
                    goto done;
                }
                break;
            }
            ai_Unk92bec0 += 0x1f;
            continue;
send:
            m[1] = 0;
            res = G21_C40(rec[4], m);
done:
            p[2] = res;
            ai_Unk92bec0 += 0x1f;
        }
    }
}
