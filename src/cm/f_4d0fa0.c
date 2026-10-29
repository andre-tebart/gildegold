/* 0x4d0fa0, sets up the fight scene for the fight record rec (+0x7c flags, +0x10 / +0x14 the attacker's and the
   defender's buildings, +0x34 / +0x38 the two sides, +0x80 / +0xc0 the 16 defender / attacker characters, +0x100 the three
   carts, +0x10c the building whose kind chooses the sounds): the scenario "Kampfszenario_<place>.ed3" by the flags or the
   building kind of the defender (0x4e2e60), the portrait / record tables cleared, the weapon sounds (kampf\*.sbf), the
   speech sets of both sides (types 0 mercenaries / 1 robbers / 2 thieves), the characters placed at the dummies
   "dummy_OFFENSE_ENTRY" / "dummy_DEFENSE_ENTRY" (0x4d0920), the carts "ob_KARREN_<size>_KAMPF.ogr" with their goods
   "WARE_0<n>", the border cells of the scene's map set to 0xd. Rough draft. */

#define G19_SNDS(a, b, c, d, e, s)     a = snd_LoadVoiceBank(S_##s##1, 0);      b = snd_LoadVoiceBank(S_##s##2, 0);      c = snd_LoadVoiceBank(S_##s##3, 0);      d = snd_LoadVoiceBank(S_##s##4, 0);      e = snd_LoadVoiceBank(S_##s##5, 0)
#define S_DIEBE1 "ARBEITER_KOMMENTARE_KLICK_DIEBE.sbf"
#define S_DIEBE2 "K_C2C_ANGRIFF_DIEBE.sbf"
#define S_DIEBE3 "K_C2C_BEFEHL_ERHALTEN_DIEBE.sbf"
#define S_DIEBE4 "K_C2C_HOHN_DIEBE.sbf"
#define S_DIEBE5 "K_C2C_SCHMERZ_DIEBE.sbf"
#define S_RAEUBER1 "ARBEITER_KOMMENTARE_KLICK_RAEUBER.sbf"
#define S_RAEUBER2 "K_C2C_ANGRIFF_RAEUBER.sbf"
#define S_RAEUBER3 "K_C2C_BEFEHL_ERHALTEN_RAEUBER.sbf"
#define S_RAEUBER4 "K_C2C_HOHN_RAEUBER.sbf"
#define S_RAEUBER5 "K_C2C_SCHMERZ_RAEUBER.sbf"
#define S_SOELDNER1 "ARBEITER_KOMMENTARE_KLICK_SOELDNER.sbf"
#define S_SOELDNER2 "K_C2C_ANGRIFF_SOELDNER.sbf"
#define S_SOELDNER3 "K_C2C_BEFEHL_ERHALTEN_SOELDNER.sbf"
#define S_SOELDNER4 "K_C2C_HOHN_SOELDNER.sbf"
#define S_SOELDNER5 "K_C2C_SCHMERZ_SOELDNER.sbf"

void ai_Unk4d0fa0(char *rec)
{
    char buf[0x40], nm[0x40], path[0x40];
    char t[16];
    int q[4];
    char *sc, *b, *bl, *o, *a, *c, *h, *w, *r, *p, *k;
    char **pp;
    int nmax = 10;
    int i, j, n, kind, ty, wd;
    int *cell;
    unsigned char fl = rec[0x7c];

    if (fl & 1) {
        if (fl & 8)
            ai_Unk4e2e60("Kampfszenario_STADT.ed3");
        else
            ai_Unk4e2e60("Kampfszenario_BERGPASS.ed3");
    } else if (fl & 4) {
        ai_Unk4e2e60("Kampfszenario_BERGPASS.ed3");
    } else {
        bl = gm_BuildQuery(1, 1, *(int *)(rec + 0x14));
        k = &ai_BTypes[(unsigned char)bl[0] * 0x24d + (unsigned char)bl[0]];
        kind = (unsigned char)k[0];
        if (gm_IsHomeType(bl) != 0) {
            if (kind == 0x10) {
                ai_Unk4e2e60("Kampfszenario_RAEUBERLAGER.ed3");
                nmax = 15;
            } else if (kind == 0xc)
                ai_Unk4e2e60("Kampfszenario_STEINBRUCH.ed3");
            else if (kind == 0xb)
                ai_Unk4e2e60("Kampfszenario_WALDSTUECK.ed3");
            else if (kind == 0xd)
                ai_Unk4e2e60("Kampfszenario_MINE.ed3");
        } else {
            if (kind == 4) {
                if (k[0x247] == 1)
                    ai_Unk4e2e60("Kampfszenario_SCHMUGGLERLOCH.ed3");
                else if (k[0x247] == 2)
                    ai_Unk4e2e60("Kampfszenario_UNTERSCHLUPF.ed3");
                else if (k[0x247] == 3)
                    ai_Unk4e2e60("Kampfszenario_DIEBESGILDE.ed3");
            } else if (kind == 0x13) {
                if (k[0x247] == 1)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_KLEIN.ed3");
                else if (k[0x247] == 2)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_MITTEL.ed3");
                else if (k[0x247] == 3)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_GROSS.ed3");
            }
        }
    }
    sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    ai_Unk4e3650();
    if (ai_Unk633738 != 0) {
        *(int *)(ai_Unkfd7c50 + d3_CurrentScene * 0x3e0 + 8) = *(int *)(ai_Unkfd7c50 + 8);
        ai_Unk633dd4 = *(int *)(ai_Unkfd7c50 + 8);
        sv_Unk5301a0();
        d3sky_Init(ai_Unk633dd4, 1);
    }
    memset(ai_Unk935d20, -1, 0x80);
    for (p = (char *)ai_Unk9304c0 + 4; (int)p < ((unsigned)((unsigned)ai_Unk9304c0 + 0x140) + 0x4); p += 0x14) {
        *(int *)(p + 4) = -1;
        *(int *)p = 0;
        *(int *)(p + 8) = 0;
    }
    memset((char *)((unsigned)&ai_Unk930600 + 0x4380), 0, 0x1000);
    memset(cm_Bombs, 0, 0x100);
    memset(ai_Unk935a00, 0, 0x200);
    memset(ai_Unk930600, 0, 0x4380);
    for (p = ai_Unk930600; (int)p < ((unsigned)&ai_Unk930600 + 0x4380); p += 0x21c)
        *(unsigned short *)p = 0xffff;
    for (p = ai_Unk930400 + 8; (int)p < ((unsigned)((unsigned)ai_Unk930400 + 0xc0) + 0x8); p += 0x20)
        *(int *)p = -1;
    ai_Unk6f0d0c = sb_LoadSampleBank("kampf.sbf", 1);
    ai_Unk6f0d10 = sb_LoadSampleBank("kampf\\armbrust.sbf", 1);
    ai_Unk6f0d14 = sb_LoadSampleBank("kampf\\bombeundtopf.sbf", 1);
    ai_Unk6f0d18 = sb_LoadSampleBank("kampf\\dolche.sbf", 1);
    ai_Unk6f0d1c = sb_LoadSampleBank("kampf\\pistole.sbf", 1);
    ai_Unk6f0d20 = sb_LoadSampleBank("kampf\\schwerter.sbf", 1);
    b = gm_BuildQuery(1, 1, *(int *)(rec + 0x10c));
    if (b != 0)
        ai_Unk6f0d50 = ai_Unk4d4050(b);
    if (ai_Unk6f0d50 == 2) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, DIEBE);
    } else if (ai_Unk6f0d50 == 1) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, RAEUBER);
    } else if (ai_Unk6f0d50 == 0) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, SOELDNER);
    }
    b = gm_BuildQuery(1, 1, *(int *)(rec + 0x14));
    if (b != 0)
        ai_Unk6f0d54 = ai_Unk4d4050(b);
    if (ai_Unk6f0d54 == ai_Unk6f0d50) {
        ai_Unk6f0d28 = ai_Unk6f0d24;
        ai_Unk6f0d30 = ai_Unk6f0d2c;
        ai_Unk6f0d38 = ai_Unk6f0d34;
        ai_Unk6f0d40 = ai_Unk6f0d3c;
        ai_Unk6f0d48 = ai_Unk6f0d44;
    } else if (ai_Unk6f0d54 == 2) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, DIEBE);
    } else if (ai_Unk6f0d54 == 1) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, RAEUBER);
    } else if (ai_Unk6f0d54 == 0) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, SOELDNER);
    }
    ai_Unk6f0d5c = d3io_LoadObjectRef("ub_STIEFEL", "ub_STIEFEL");
    if (sc == 0) {
        *(char **)(ai_Unk5fb5b0 + 0xb0) = (char *)d3sm_Create(ai_Unk633738, *(int *)(ai_Unk5fb5b0 + 0xb4), 2);
        sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    }
    ai_Unk935d00 = *(float *)(sc + 0x10) * ai_Unk5edc58;
    ai_Unk623818 = 0.15f;
    ai_Unk62381c = 0;
    ai_Unk6f0d04 = rec;
    ai_Unk4d0ca0();
    ai_Unk6f0cec = (int)gm_FindChar(*(int *)(rec + 0x34));
    ai_Unk6f0ce8 = (int)gm_FindChar(*(int *)(rec + 0x38));
    if (ai_CharInts[ai_PlayerId][0] == ai_Unk60cb5c && (rec[0x7c] & 1)) {
        cm_BeginSendGroup();
        for (i = 0; i < 3; i++) {
            if (*(int *)(rec + 0x100 + i * 4) != -1)
                cm_SendExSellObject(-2, -1, 0x154, 1, ai_OwnPlayer, 0);
        }
        cm_EndSendGroup();
        if (cm_SendGroupState() == 0) {
            do
                sv_PumpNet();
            while (cm_SendGroupState() == 0);
        }
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0xc0 + i * 4) != -1) {
            r = ai_Unk4d0920((char *)*(int *)(rec + 0xc0 + i * 4), (int)"dummy_OFFENSE_ENTRY");
            *(int *)(r + 0x16c) = ai_Unk6f0cec;
            if (*(char **)(r + 0x184) != 0) {
                o = *(char **)(*(char **)(r + 0x184) + 0x34);
                d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), 5, 1);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x80 + i * 4) != -1) {
            r = ai_Unk4d0920((char *)*(int *)(rec + 0x80 + i * 4), (int)"dummy_DEFENSE_ENTRY");
            *(int *)(r + 0x16c) = ai_Unk6f0ce8;
            *(float *)(*(char **)(r + 0x184) + 0x1a0) = *(float *)(*(char **)(r + 0x184) + 0x1a0) * ai_Unk5eb8b8;
            if (*(char **)(r + 0x184) != 0) {
                o = *(char **)(*(char **)(r + 0x184) + 0x34);
                d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), 4, 1);
            }
        }
    }
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, ai_Unk4d0f80);
    memset(ai_Unk7603a0, 0, 0x80);
    pp = ai_Unk7603a0;
    for (p = ai_Unk930600; (int)p < ((unsigned)&ai_Unk930600 + 0x4380); p += 0x21c) {
        if (*(unsigned short *)p != 0xffff && *(char **)(p + 0x184) != 0 &&
            *(char **)(*(char **)(p + 0x184) + 0x88) == ai_Unk5fb5b0)
            *pp++ = p;
    }
    for (i = 1; i < 9; i++) {
        sprintf(nm, "WARE_0%i", i);
        o = (char *)d3_FindObject(0, 0x40, 0, nm);
        if (o != 0)
            d3if_Show(o, 0);
    }
    n = 0;
    for (j = 0; j < 3; j++) {
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        gm_ShuffledBytes(9, t);
        sprintf(nm, "dummy_KARREN_0%i", j + 1);
        o = (char *)d3_FindObject(0, 0x100, 0, nm);
        if (o != 0) {
            if (*(int *)(rec + 0x100 + j * 4) == -1)
                d3if_ShowTree(o, 0);
            else {
                a = gm_Query(0, 2, 7, 1, *(int *)(rec + 0x100 + j * 4));
                if (a == 0) {
                    gm_FindObjectInst(*(int *)(rec + 0x100 + j * 4));
                    *(int *)(rec + 0x100 + j * 4) = -1;
                } else {
                    c = gm_Query(*(int *)(a + 0x14), 1, 0, 0x1dd);
                    n = 0;
                    while ((w = gm_Query(*(int *)(c + 0x14), 1, 5)) != 0 && n < 3) {
                        q[n++] = (int)w;
                        gm_QueryNext();
                    }
                    d3_PointToWorld(o, o + 0x4c, buf);
                    ty = *(unsigned short *)a;
                    if (ty == 0x134)
                        sprintf(path, "%s*ob_KARREN_KLEIN_KAMPF.ogr", ai_DataPath);
                    else if (ty == 0x135)
                        sprintf(path, "%s*ob_KARREN_MITTEL_KAMPF.ogr", ai_DataPath);
                    else if (ty == 0x136)
                        sprintf(path, "%s*ob_KARREN_GROSS_KAMPF.ogr", ai_DataPath);
                    h = (char *)d3_LoadGroup(path, 0, 0);
                    d3_SetObjectPos(h, buf);
                    *(int *)(h + 0x200) = (int)a;
                    for (i = 0; i < 3; i++) {
                        sprintf(nm, "WARE_0%i", t[i] + 1);
                        w = (char *)d3_FindObject(0, 0xc0, 0, nm);
                        if (w != 0) {
                            if (i > n - 1)
                                d3if_Show(w, 0);
                            else {
                                *(int *)(w + 0x200) = q[i];
                                d3if_Show(w, 1);
                                *(int *)(w + 0x224) = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    d3sm_Build(*(char **)(ai_Unk5fb5b0 + 0xb0), ai_Unk633738);
    for (i = 0; i < nmax; i++) {
        wd = *(int *)(sc + 0x20);
        cell = *(int **)(sc + 0x24);
        for (j = i; j < wd; j++) {
            cell[((wd - 1) * i + j) * 7] = 0xd;
            cell[((wd - i - 1) * wd + j) * 7] = 0xd;
            cell[(wd * j + i) * 7] = 0xd;
            cell[(wd * (j + 1) - i - 1) * 7] = 0xd;
        }
    }
    if (b != 0)
        ai_Unk60cb58 = (float)gm_BuildingSkill(b, 6, 1) + ai_Unk5eb878;
    else
        ai_Unk60cb58 = 1.0f;
}
