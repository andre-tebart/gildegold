/* 0x53a520, applies the round result of player `pl` to all its figures (reset flags, rewards, relations between the figures and the other players; Sleep(0x14) every 0x20 changes; ROUGH, the __ftol operands are guesses) */

float gm_Unk508740();

void main_Unk53a520(unsigned short pl)
{
    int cnt = 0;
    int left = 0x100;
    unsigned char *c;
    char *k;
    int ik, t, r, v, m;
    int ev[0x3e];
    int tmp;
    char cv, ck, tb;
    unsigned char *p;
    int pe = pl * 0x21c;
    int *pp, *pn;
    int sub, ps, pq, qb, q, r2, cnt2, ft, pass;
    int nb[0x25];
    char buf[0x40];
    unsigned u;

    c = (unsigned char *)U_I((unsigned)&ai_Blds);
    do {
        if (*c != 0 && *(unsigned short *)(c + 0x27) == pl) {
            k = (char *)(U_I((unsigned)&ai_BTypes) + *c * 0x24d);
            c[0x51] = 0; c[0x52] = 0; c[0x53] = 0; c[0x54] = 0;
            c[0x55] = 0; c[0x56] = 0; c[0x57] = 0; c[0x58] = 0;
            c[0x4d] = 0; c[0x4e] = 0; c[0x4f] = 0; c[0x50] = 0;
            if ((*(unsigned short *)(c + 0x5a) & 0x3800) != 0) {
                cm_SendSetField(*(int *)(c + 1), 0x5a, 2, 0, 0x3800);
                cnt++;
            }
            cv = *k;
            ik = cnt;
            if (cv == 4 || cv == 0x10 || cv == 0x13) {
                r = gm_Query(*(int *)(c + 0x5d), 1, 0, 0xca);
                while (r != 0) {
                    ik++;
                    if (*(unsigned char *)(r + 0x37) < 6) {
                        ai_Unk4c5010(*(int *)(c + 1), -1, *(int *)(r + 2));
                    } else {
                        v = gm_FindBuildingById(*(int *)(r + 0x2a));
                        if (v == 0) {
                            ai_Unk4c47b0(*(int *)(r + 2));
                        } else {
                            char b = *(char *)(r + 0x37) - (char)gm_BuildingRank(v) * 2;
                            tmp = (b < 0) ? 0 : (unsigned char)b;
                            cm_BeginFieldBlock(r, *(int *)(r + 2));
                            cm_AddFieldDelta(1, 1, r - U_I((unsigned)&cm_FieldObject) + 0x37, &tmp);
                            cm_SendFieldDeltas();
                        }
                    }
                    r = gm_QueryNext();
                }
            } else if (cv == 5) {
                tmp = 0;
                cm_BeginFieldBlock(c, *(int *)(c + 1));
                ik = 0;
                do {
                    cm_AddFieldValue(4, 1, (int)c + ik * 12 - U_I((unsigned)&cm_FieldObject) + 0x71, &tmp);
                    ik++;
                } while (ik < 3);
                cm_SendFieldValues();
                ik = cnt + 1;
                u = pl;
                if (*(char *)(((unsigned)&ai_Chars + 0x2) + u * 0x21c) != 6 && *(char *)(((unsigned)&ai_Chars + 0x2) + u * 0x21c) != 7
                    && gm_GetGoodsValue(PL(u)) < 160000) {
                    cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + u * 0x21c), -1, U_I((unsigned)&ai_OwnPlayer), (unsigned char)k[0x247] * 16000);
                    ik = cnt + 2;
                }
            } else if (cv == 0xd || cv == 0xc || cv == 0xb) {
                if (*(short *)(c + 0x27) == **(short **)(unsigned)&ai_PlayerChar) {
                    r = gm_FindCharacterByMaster(c);
                    if (r == 0) {
                        cm_SendMessage3d(c, 1, 0, *(char *)(U_I((unsigned)&ai_BTypes) + *c * 0x24d + 0x223));
                        ik = cnt + 1;
                    } else if ((*(unsigned char *)(r + 0x1b4) & 1) == 0) {
                        ev[1] = *(int *)(r + 0x1b8);
                        ev[2] = *(int *)(r + 0x1bc);
                        ev[0] = *(int *)(r + 0x1b4) | 1;
                        ev[3] = *(int *)(r + 0x1c0);
                        cm_BeginFieldBlock(r, *(int *)(r + 4));
                        cm_AddFieldDelta(4, 1, r - U_I((unsigned)&cm_FieldObject) + 0x1b4, ev);
                        cm_SendFieldDeltas();
                    }
                }
            } else if (cv == 0x1d) {
                unk_Unk4f1de0(c);
            }
            if (*(short *)(c + 0x25) == **(short **)(unsigned)&ai_PlayerChar && (c[0x5a] & 3) == 0) {
                cv = *k;
                if (cv != 1 && cv != 3 && cv != 6 && cv != 10 && cv != 11 && cv != 12 && cv != 13
                    && cv != 15 && cv != 0x1c && cv != 0x1b && cv != 0x1a && cv != 0x19 && cv != 0x18 && cv != 0x17) {
                    memset(ev, 0, 0xf8);
                    ((char *)ev)[0] = 0x1b;
                    ev[1] = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2);
                    ev[2] = -1;
                    ((char *)ev)[0x16] = 2;
                    ev[0x1d] = gm_EquipmentValue(c, (int)(char)c[0x5c]);
                    ev[0x1c] = *(int *)(c + 1);
                    cm_SendLongMessage(ev);
                }
            }
            if (*(char *)(((unsigned)&ai_Chars + 0x2) + pl * 0x21c) == 0xd || *(char *)(((unsigned)&ai_Chars + 0x2) + pl * 0x21c) == 0
                || (cnt = ik, U_I((unsigned)&ai_CharInts + pl * 0x21c) == -1)) {
                cm_SendOwnership(*(int *)(c + 1), *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2), *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2));
                cnt = ik + 1;
                cv = *k;
                if (cv != 0xd && cv != 0xc && cv != 0xb) {
                    memset(ev, 0, 0xf8);
                    ((char *)ev)[0] = 0x1b;
                    ev[1] = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2);
                    ev[2] = -1;
                    ((char *)ev)[0x16] = 2;
                    ev[0x1d] = gm_EquipmentValue(c, (int)(char)c[0x5c]);
                    ev[0x1c] = *(int *)(c + 1);
                    cm_SendLongMessage(ev);
                    cnt = ik + 2;
                    pp = (int *)(unsigned)&ai_CharInts;
                    do {
                        if (pp[0x5a] == (int)c) {
                            memset(ev, 0, 0xf8);
                            ((char *)ev)[0] = 0x31;
                            ev[1] = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2);
                            ev[2] = *(int *)(c + 1);
                            ev[0x0a] = U_I(((unsigned)&ai_Time + 0x8));
                            ev[0x08] = U_I((unsigned)&ai_Time);
                            ev[0x09] = U_I((unsigned)&ai_TimeHour);
                            ev[0x1c] = *pp;
                            ((char *)ev)[0x16] = 1;
                            *(short *)((char *)ev + 0x14) = *(short *)((unsigned)&ai_Time + 0xc);
                            cm_SendLongMessage(ev);
                            cnt++;
                        }
                        pp += 0x87;
                    } while ((int)pp < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
                }
            }
            u = util_RandomInt(100);
            if ((unsigned short)u > 0x1e) {
                cv = *k;
                if (cv != 0xd && cv != 0xc && cv != 0xb && cv != 0x10 && (c[0x5a] & 1) == 0 && cv != 10) {
                    u = util_RandomInt(3);
                    gm_Unk50cad0(c, -((int)(u & 0xffff) + 2));
                    cnt++;
                }
            }
        }
        c += 0xa9;
        left--;
    } while (left != 0);
    t = gm_Unk504350(*(char *)(((unsigned)&ai_CharInts + 0x160) + pl * 0x21c));
    if (t > 1 && t < 6)
        gm_HasBuilding(t);
    p = (unsigned char *)gm_BuildQuery(1, 6);
    while (1) {
        if (p == 0) {
            if (cnt > 0x20) {
                sv_PumpNet();
                Sleep(0x14);
                cnt = 0;
            }
            t = gm_BuildQuery(1, 4, pl);
            while (t != 0) {
                r = gm_Unk508d80(t, pl, buf);
                if (r != 0) {
                    if (*(int *)(buf + 0x30) != 0 && (r2 = gm_Query(*(int *)(t + 0x5d), 2, 6, 0, 0xe8)) != 0
                        && (v = gm_Unk509e30(*(int *)(r2 + 0x14), U_I((unsigned)&ai_OwnPlayer))) > 0) {
                        cnt++;
                        cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(t + 0x27) * 0x21c), *(int *)(r2 + 2), U_I((unsigned)&ai_OwnPlayer), v);
                    }
                    if (*(int *)(buf + 0x3c) != 0) {
                        r2 = gm_Query(*(int *)(t + 0x5d), 1, 0, 0xf0);
                        if (r2 != 0 && (v = gm_Unk509e30(*(int *)(r2 + 0x14), U_I((unsigned)&ai_OwnPlayer))) > 0) {
                            cnt++;
                            cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(t + 0x27) * 0x21c), *(int *)(r2 + 2), U_I((unsigned)&ai_OwnPlayer), v);
                        }
                    }
                }
                t = gm_BuildNext();
                if (cnt > 0x20) {
                    sv_PumpNet();
                    Sleep(0x14);
                    cnt = 0;
                }
            }
            return;
        }
        if (*(unsigned short *)(p + 0x27) == pl) {
            ps = PL(*(unsigned short *)(p + 0x27));
            if (*(char *)(U_I((unsigned)&ai_BTypes) + *p * 0x24d) == 0x1e) {
                r = ahm_OwnerChar(p);
                if (r != 0) {
                    v = ahm_ObjectValue(p);
                    cm_SendMoveGoodsUnchecked(*(int *)(r + 4), *(int *)(ps + 2), U_I((unsigned)&ai_OwnPlayer), v);
                    cnt++;
                }
            } else {
                r = gm_Unk502eb0(p, nb);
                if (r != 0) {
                    pass = 0;
                    pn = nb + 1;
                    do {
                        sub = 0xc;
                        do {
                            ft = (int)gm_Unk508740(ps, *p, -1, pass);
                            q = *pn;
                            cnt2 = cnt;
                            if (q != 0xffff) {
                                cv = 0;
                                qb = q * 0x21c;
                                pq = PL(q);
                                if (U_I(((unsigned)&ai_Unk770cec + 0x18) + qb) == 0) {
                                    if (U_I(((unsigned)&ai_Unk770cec + 0x4) + qb) == 0) {
                                        r2 = gm_BuildingMainObject(p);
                                        if (r2 != 0 && *(int *)(r2 + 2) != -1) {
                                            cnt++;
                                            gm_PlaceOnStage(U_I((unsigned)&ai_CharInts + qb), *(int *)(p + 1), *(int *)(r2 + 2), 0, 0);
                                        }
                                    } else {
                                        r2 = unk_Unk4fdc10(U_I(((unsigned)&ai_Unk770cec + 0x4) + qb));
                                        if (r2 != 0 && *(int *)(r2 + 2) != -1) {
                                            cnt++;
                                            gm_PlaceOnStage(U_I((unsigned)&ai_CharInts + qb), *(int *)(U_I(((unsigned)&ai_Unk770cec + 0x4) + qb) + 1), *(int *)(r2 + 2), 0, 0);
                                        }
                                    }
                                } else {
                                    *(unsigned char *)(U_I(((unsigned)&ai_Unk770cec + 0x18) + qb) + 0x8c) &= 0xfb;
                                }
                                if (*(char *)(((unsigned)&ai_Chars + 0x2) + qb) != 0x14)
                                    cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + qb), U_I((unsigned)&ai_CharInts + pe), U_I((unsigned)&ai_OwnPlayer), ft);
                                if (*(int *)(p + 0x3d) < 100 && gm_Opinion(pl, *(int *)pq) > -0x78) {
                                    v = (100 - *(int *)(p + 0x3d)) / 3;
                                    if (v < 1)
                                        v = 1;
                                    cv = (char)-v;
                                } else if (*(int *)(p + 0x3d) > 100 && gm_Opinion(pl, *(int *)pq) < 100) {
                                    cv = 1;
                                    v = (*(int *)(p + 0x3d) - 100) / 3;
                                    if (v > 0)
                                        cv = (char)v;
                                }
                                v = *(int *)(p + 0x41);
                                if (v < 100) {
                                    if (gm_Opinion(pl, *(int *)pq) >= -0x77) {
                                        v = (*(int *)(p + 0x41) - 100) / 5;
                                        if (v < 1)
                                            v = 1;
                                        cv += (char)v;
                                    }
                                } else if (v > 100 && gm_Opinion(pl, *(int *)pq) < 100) {
                                    v = (100 - *(int *)(p + 0x41)) / 5;
                                    if (v < 1)
                                        v = 1;
                                    cv -= (char)v;
                                }
                                cnt2 = cnt + 1;
                                if (cv != 0) {
                                    cm_SendRelations(U_I((unsigned)&ai_CharInts + pe), U_I((unsigned)&ai_CharInts + qb), (int)cv, 0);
                                    cnt2 = cnt + 2;
                                }
                                cnt = cnt2;
                                cm_BeginFieldBlock(pq, U_I((unsigned)&ai_CharInts + qb));
                                cm_AddFieldDelta(4, 1, 0x19c, &ft);
                                cm_SendFieldDeltas();
                                m = gm_Opinion(pl, *(int *)pq);
                                cnt2 = cnt + 1;
                                if (m + cv < -0x1a) {
                                    u = util_RandomInt(0x4a);
                                    if (-(int)(u & 0xffff) < m + cv) {
                                        u = util_RandomInt(100);
                                        memset(ev, 0, 0xf8);
                                        MSG_B(ev, 4) = ((unsigned short)u < 0x33) ? 0x34 : 0x33;
                                        MSG_I(ev, 8) = U_I((unsigned)&ai_CharInts + pe);
                                        MSG_I(ev, 0xc) = *(int *)(p + 1);
                                        MSG_I(ev, 0x28) = U_I((unsigned)&ai_Time);
                                        MSG_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
                                        MSG_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                                        *(short *)((char *)ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
                                        MSG_B(ev, 0x36) = 1;
                                        MSG_I(ev, 0x58) = U_I((unsigned)&ai_CharInts + qb);
                                        gm_AddTime((struct GCLOCK *)((char *)ev + 0x28), (util_RandomInt(6) & 0xffff) + 4, 0, 0);  /* the 0, 0 are pushed first (0x53ae4e) */
                                        cm_SendLongMessage(ev);
                                        cnt2 = cnt + 2;
                                    }
                                    cnt = cnt2;
                                    ck = *(char *)(((unsigned)&ai_CharInts + 0x161) + qb);
                                    if (ck == 2 || ck == 1 || ck == 0x13 || ck == 0x14 || ck == 0x10 || (cnt2 = cnt, ck == 0xf)) {
                                        cm_BeginFieldBlock(pq, U_I((unsigned)&ai_CharInts + qb));
                                        u = *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x79) + qb);
                                        tb = (char)(int)((float)u * 0.95f);
                                        if (42.0 < (float)(int)u)
                                            cm_AddFieldDelta(1, 1, pq + 0x82 - U_I((unsigned)&cm_FieldObject), &tb);
                                        u = *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x7a) + qb);
                                        tb = (char)(int)((float)u * 0.95f);
                                        if (42.0 < (float)(int)u)
                                            cm_AddFieldDelta(1, 1, pq + 0x83 - U_I((unsigned)&cm_FieldObject), &tb);
                                        cm_SendFieldDeltas();
                                        cnt2 = cnt + 1;
                                    }
                                }
                            }
                            cnt = cnt2;
                            pn++;
                            sub--;
                        } while (sub != 0);
                        pass++;
                    } while (pass < 2);
                    q = 0;
                    do {
                        ft = (int)gm_Unk508740(PL(*(unsigned short *)(p + 0x27)), *p, -1, 0);
                        r2 = nb[25 + q];
                        if (r2 != 0xffff) {
                            cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + r2 * 0x21c), U_I((unsigned)&ai_CharInts + pe), U_I((unsigned)&ai_OwnPlayer), ft);
                            cm_BeginFieldBlock(PL(r2), U_I((unsigned)&ai_CharInts + r2 * 0x21c));
                            cm_AddFieldDelta(4, 1, 0x19c, &ft);
                            cm_SendFieldDeltas();
                            cnt += 2;
                        }
                        q++;
                    } while (q < 0xc);
                }
            }
        }
        p = (unsigned char *)gm_BuildNext();
    }
}
