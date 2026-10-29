/* 0x52b500, keeps the "extras" (script objects "sp_<id>") of the characters of the group of the building a (the table 0x7603a0 = the characters of 0x52b2a0): a character gets the object of its role (names "bewohner",
   "besucher", "meister", "pause", "faulenzen" or the one from the 0x75ec80 table of 0x52b3c0), created from the script "<scripts>locations\<class>\<name><n>_<class>.esc" (n = the first free number among the
   objects already using the name) and linked back (+0xc94 / +0x28); a character whose role ended loses the object (0x429e90); ROUGH */

int init_BuildingKindName();                     /* 0x50dba0 */
struct VFSENTRY * vfs_FindFile(char *path, struct VFSDIR *d, struct VFSDIR **found);                     /* 0x42f8f0 */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */

void sv_Unk52b500(unsigned char *a)
{
    char *cls, *kind, *c, *g, *w, *o4, *r;
    short *it;
    int h, i, ok, k, n, d, v, len, *p;
    char name[0x20], cn[0x60], path[0x80], sp[0x80];
    int used[8];

    cls = 0;
    it = (short *)SV_I((unsigned)&sv_PanelShown);
    if (it != 0 && *it != 0xfd)
        cls = ai_Goods + *(unsigned short *)it * 0x41;
    kind = ai_BTypes + *a * 0x24d;
    if (SV_I((unsigned)&full_data_6237c8) + 500 < SV_I((unsigned)&ai_Unk6333cc)) {
        if (ai_Unk5fb5b0 != 0 && (k = *(int *)(ai_Unk5fb5b0 + 0x3c8)) != 0 && *(int *)(k + 0x318) != 0)
            ((int (*)())*(int *)(k + 0x318))(ai_Unk5fb5b0, 6, k + 0x31c, 0);
        SV_I((unsigned)&full_data_6237c8) = SV_I((unsigned)&ai_Unk6333cc);
    }
    h = gm_ClockSeason((unsigned)&ai_Time);
    for (i = 0; i < 0x20; i++) {
        c = (char *)SV_I((unsigned)&ai_Unk7603a0 + i * 4);
        if (c == 0 || (g = *(char **)(c + 0x184)) == 0)
            continue;
        if (*(int *)(g + 0x2c) != *(int *)(a + 1) || *(char *)(c + 8) == 0 || *(char **)(g + 0x88) != ai_Unk5fb5b0)
            continue;
        ok = 0;
        name[0] = 0;
        if (sv_Unk52b3c0(c, a, name) == 0) {
            k = *kind;
            if (k == 0x16 && it != 0 && *it == 0x120 && *(int *)(g + 0x30) == *(int *)((char *)it + 2) && (*(unsigned *)(c + 0x1c8) & 0x1000) == 0) {
                strcpy(name, "besucher");
                ok = 1;
            } else if (k == 1 && *(unsigned char **)(c + 0x170) == a && (*(unsigned *)(c + 0x1c8) & 0x1000) == 0) {
                strcpy(name, "bewohner");
                ok = 1;
            } else if (*(char *)(c + 0x180) == 0 && *(unsigned char **)(c + 0x16c) == a && *(int *)(g + 0x28) == -1) {
                if (*(char *)(c + 0x164) == 0) {
                    if (*(char *)(c + 0x165) == 0x1c && (*(unsigned char *)(c + 0x1b4) & 1) != 0)
                        strcpy(name, "faulenzen");
                    else
                        strcpy(name, "pause");
                } else
                    strcpy(name, "meister");
                ok = 1;
            }
        } else {
            k = *kind;
            if (k == 0xd) {
                if (it != 0) {
                    if (*it == 0x37 && *(int *)(g + 0x30) == *(int *)((char *)it + 2)) {
                        ok = 1;
                        goto l7f0;
                    }
                    if (*it != 0xfd || *(int *)(g + 0x30) != *(int *)((char *)it + 2))
                        goto l7f0;
                }
                w = *(char **)(c + 0x17c);
                ok = 1;
                if (w != 0 && *(int *)(w + 200) != 0)
                    ok = 0;
            } else {
                if (k == 8 && *it == 0xdd && *(char *)(c + 0x180) == 0x15) {
                    ok = 1;
                    goto l7f0;
                }
                w = *(char **)(c + 0x17c);
                ok = 1;
                if (w != 0 && *w == 4 && *(int *)(w + 200) != 0)
                    ok = 0;
            }
        }
l7f0:
        if (*(char *)(c + 0x165) == 0x1c && (*(unsigned char *)(c + 0x1b4) & 1) != 0 && *(int *)(g + 0x28) == -1) {
            strcpy(name, "faulenzen");
        } else if (!ok) {
            goto lsecond;
        }
        if (name[0] != 0) {
            if (*(int *)(g + 0x28) == -1) {
                for (n = 0; n < 8; n++)
                    used[n] = 1;
                for (p = (int *)(unsigned)&ai_Unk7603a0; (int)p < (unsigned)((unsigned)ai_Unk7603a0 + 0x80); p++) {
                    if (*p != 0 && (w = *(char **)(*p + 0x184)) != 0 && (v = *(int *)(w + 0x28)) != -1
                        && (o4 = (char *)evt_FindById(v)) != 0 && (r = (char *)strstr(o4, name)) != 0) {
                        len = strlen(name);
                        d = r[len] - '0';
                        if (d < 0 || d > 9)
                            break;
                        used[d] = 0;
                    }
                }
                n = 0;
                do {
                    if (used[n] != 0)
                        break;
                    n++;
                } while (n < 8);
                if (*(char *)(c + 0x180) == 0x16)
                    strcpy(cn, "Training_Allgemein");
                else if (cls == 0)
                    init_BuildingKindName(*a, cn);
                else
                    strcpy(cn, cls + 1);
                sprintf(path, "%slocations\\%s\\%s%i_%s.esc", evt_Path, cn, name, n, cn);
                if (vfs_FindFile(path, SV_I((unsigned)&d3_Unk6333fc), &d) == 0)
                    sprintf(path, "%slocations\\%s\\%s0_%s.esc", evt_Path, cn, name, cn);
                if (vfs_FindFile(path, SV_I((unsigned)&d3_Unk6333fc), &d) != 0) {
                    sprintf(sp, "sp_%i", *(int *)(c + 4));
                    o4 = (char *)evt_LoadScript(path);
                    if (o4 != 0) {
                        evt_RunScriptArgs((EVTSCRIPT *)o4, 1, sp);
                        *(char **)(o4 + 0xc94) = g;
                        *(int *)(g + 0x28) = *(int *)(o4 + 0x100);
                    }
                }
            }
            continue;
        }
lsecond:
        ok = 0;
        if (*(unsigned char **)(c + 0x16c) == a && g != 0 && (v = *(int *)(g + 0x28)) != -1) {
            if (*(char *)(c + 0x180) == 0 || *(float *)((unsigned)&ai_Unk617840 + h * 4) < (float)SV_US((unsigned)&ai_TimeHour))
                ok = 1;
            w = *(char **)(c + 0x17c);
            if ((w != 0 && *w == 4 && *(int *)(w + 200) != 0) || ok) {
                ok = 0;
                o4 = (char *)evt_FindById(v);
                if (o4 != 0) {
                    w = (char *)(unsigned)&full_data_75ec84;
                    do {
                        if (ok)
                            goto lfree;
                        if (*(int *)(w - 4) != 0 && *w != 0 && strstr(o4, w) != 0)
                            ok = 1;
                        w += 0x20;
                    } while ((int)w < ((unsigned)((unsigned)full_data_75ec84 + 0x5fc) + 0x4));
                    if (ok)
lfree:
                        evt_Stop((EVTSCRIPT *)o4);
                }
            }
        }
    }
}
