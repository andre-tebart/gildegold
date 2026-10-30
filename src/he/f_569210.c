/* 0x569210, "he_RunProduktion()", the run handler of the production event record ev (+0xbc the building, +0xc0 the
   building of the goods, +0xac the good, +0x14 / +0x18 the pending orders, +0x70 the state -2 .. 2 by the jump table
   0x56a0e4, +0xb4 / +0xb8 the amount left, +0xcc / +0xd4 the factors, +0xd0 the rounds, +0x52 / +0x60 the clocks, +0xc4 the
   handle of the transport, +0xc8 the missing-goods flag): state 0 -> 1; state 1 with orders: the goods are fetched from the
   goods building (0x4ffe90 / 0x50ab80 / 0x50aaf0), the amount is used up by the rounds (the rate is the 0x5092e0 value, the
   level of the owner (+0x81), the workers +0x8c (8 ids, their 0x4fc8e0 skill) ..); a missing good gives the news
   0x1bb2 .. 0x1bb4 (_NACHRICHTEN_ROHSTOFFMANGEL_HS_xx by the type) to the owner (0x53dee0); state 2 without orders frees the
   record (hands the characters on with 0x524260). ROUGH, from the disassembly: the control flow is kept by labels */

float ahm_PlaceFactor();
void ai_Unk4c5840(int a);
void ai_Unk4c48e0(int a, short b, int c);
void sv_CountGroupMembers(char *g);
int ai_Unk5740f0();
int ai_Unk5ba700();
void cm_SendRelations(int a, int b, int c, int d);
int gm_FreeCapacity();
int gm_ListAmount();
int gm_BuildingMainObject(unsigned char *b);
int gm_Unk509400();
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);
int ai_InventoryHas(void *ch, int id);

/* unlinked, the table of the raw material shortage news (0x628a5c .. 0x628bdc) by the type id of the building (0x569d17 / 0x5694d8) */
static __inline char *he_ProdShortage(int type)
{
    switch (type) {
    case 7: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_01";
    case 8: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_03";
    case 0xb: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_05";
    case 0xc: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_07";
    case 0xd: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_09";
    case 0xe: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_11";
    case 0x12: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_13";
    case 0x14: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_15";
    case 0x15: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_17";
    case 0x16: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_19";
    case 0x20:
    case 0x1f:
    case 0x1d: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_20";
    }
    return 0;
}

void he_Unk569210(int ev)
{
    char text[0x400];
    char name[0x24];
    char crlf[2];
    int b, b2, g, t, w, o, chk, ck, tp, found, rec, news, i, n, h, owner;
    float f, rate, fv, sum, v;
    int bo[2];

    name[0] = 0;
    if (*(unsigned short *)(ev + 8) == 0xffff) {
        b = gm_FindBuildingById(*(int *)(ev + 0xbc));
        if (b == 0)
            goto done_free;
        txt_Format(text, "he_RunProduktion(): geb:%G", b);
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        goto state_free;
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0)
            goto work;
        *(int *)(ev + 0x70) = 2;
        return;
    case 2:
        goto state2;
    default:
        return;
    }
work:
    w = *(int *)(ev + 0xc4);
    if (w == -1)
        goto fetch;
    if (cm_GetSlotState(w) == 0)
        return;
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    if (cm_GetSlotState(*(int *)(ev + 0xc4)) != 2)
        goto fetch;
    chk = gm_FindCharacterByMaster(b);
    found = 0;
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            found = rec;
    }
    if (found == 0)
        goto arrived;
    if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
        goto arrived;
    if (*(int *)(ev + 0xc8) != 0)
        goto arrived;
    gm_FindById(0, bo, 0, *(int *)(ev + 0xc0));
    b2 = bo[0];
    t = gm_BuildingMainObject(b2);
    g = gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac));
    if (g != 0 && gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb3, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_04");
        goto news;
    }
    if (gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb4, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_05");
        goto news;
    }
    if (*(char *)(((unsigned)&ai_Unkf31160 + 0x1) + *(int *)(ev + 0xc4) * 10) != 8)
        goto arrived;
    txt_Format(text, 0x1bb2, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
    if (*(char *)tp == 0x1e)
        strcpy(name, "_NACHRICHTEN_DOMIZIL_02");
    else if (he_ProdShortage(*(unsigned char *)tp) != 0)
        strcpy(name, he_ProdShortage(*(unsigned char *)tp));
news:
    owner = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7) {
        news = 0x5f1;
        chk = gm_FindCharacterByMaster(b);
        if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
            news = 0x60d;
        if (name[0] != 0)
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, name);
        else
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, 0);
    }
    if (b != 0 && *(int *)(b + 0x95) != -1)
        ai_Unk4c5840(*(int *)(b + 1));
    *(int *)(ev + 0xc8) = 1;
arrived:
    *(int *)(ev + 0xc4) = -1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    return;
fetch:
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    if (*(int *)(b + 0x95) == -1)
        ai_Unk4c5840(*(int *)(b + 1));
    tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    n = gm_BuildingSkill(b, 1, 1);
    f = (float)n * 0.01 + 1.0;
    chk = gm_FindCharacterByMaster(b);
    owner = chk != 0 ? chk : (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7)
        f = (float)*(unsigned char *)(owner + 0x81) * 0.000992063492063492 + f;
    else
        f = 0.0f + f;
    if ((*(unsigned char *)(b + 0x5a) & 0x20) && ai_Unk5ba700(b, 0x178))
        f = f * 0.5;
    if (*(unsigned char *)(ev + 0xd0) >= 0x3c) {
        n = 0;
        sum = 0.0f;
        for (i = 0; i < 8; i++) {
            w = *(int *)(ev + 0x8c + i * 4);
            if (w == -1)
                continue;
            if (*(int *)(b + 0x41) != 2)
                cm_SendRelations(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), w, 2 - *(int *)(b + 0x41), 0);
            o = gm_FindChar(w);
            if (o == 0)
                continue;
            fv = unk_Unk4fc8e0(o);
            if (*(unsigned char *)(o + 0x165) == 0x1b || *(unsigned char *)(o + 0x165) == 0x1c) {
                fv = (gm_GetSkill(o, 1) + 0.5) * fv;
                if (*(unsigned char *)(o + 0x1b4) & 1)
                    fv = 0.01f;
            }
            if (*(unsigned char *)(o + 0x1cc) & 4)
                fv = ahm_PlaceFactor(o, *(unsigned short *)(ev + 0xac)) * fv;
            if (ai_InventoryHas((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, 0x2ba))
                fv = fv * 1.25f;
            sum = fv + sum;
            n++;
        }
        if (n != 0) {
            *(float *)(ev + 0xd4) = sum / n;
            if (ai_Unk5740f0((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c))
                *(float *)(ev + 0xd4) = *(float *)(ev + 0xd4) * 0.9f;
        }
        sv_CountGroupMembers(ev);
        *(float *)(ev + 0xcc) = (gm_Unk50c980(*(unsigned short *)(b + 0x27), 8, ev + 0x8c) - 0.5) * 0.25;
        *(unsigned char *)(ev + 0xd0) = 0;
    }
    ck = ev + 0x60;
    n = gm_Unk509400(ck, (unsigned)&ai_Time, 0);
    if (n > 0)
        *(unsigned char *)(ev + 0xd0) += n;
    v = f;
    if (*(unsigned char *)(b + 0x5b) & 0x10)
        v = v + 0.1f;
    rate = (v + (float)(*(int *)(b + 0x41) - 2) * 0.1 + *(float *)(ev + 0xcc)) * *(float *)(ev + 0xd4);
    gm_FindById(0, bo, 0, *(int *)(ev + 0xc0));
    b2 = bo[0];
    if (b2 == 0)
        goto done_free;
    if (*(unsigned short *)b2 != 0x2a && *(unsigned short *)b2 != 0x116)
        goto done_free;
    g = gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac));
    if (g != 0 && gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb3, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_04");
        t = 0;
        goto lacking;
    }
    if (gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb4, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_05");
        t = 0;
        goto lacking;
    }
    {
        char *rec91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        unsigned short *gp = (unsigned short *)(rec91 + 0x2e);
        t = 0;
        for (i = 0; i < 4; i++, gp++) {
            if (*gp == 0)
                continue;
            o = gm_Query(*(int *)(b2 + 0x14), 1, 0, *gp);
            if (o == 0 || gm_ListAmount(b2, o) < *(unsigned short *)((char *)gp - 8)) {
                txt_Format(text, 0x1bb2, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
                tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
                if (he_ProdShortage(*(unsigned char *)tp) != 0)
                    strcpy(name, he_ProdShortage(*(unsigned char *)tp));
                goto lacking;
            }
        }
        if (*(int *)(ev + 0xc8) != 0) {
            h = (int)he_FindEvent(2, 0, 0x11, 3, *(int *)(ev + 4));
            if (h != 0)
                he_Unk53e1f0((int *)h);
            h = *(int *)(ev + 0x88);
            *(int *)(ev + 0xc8) = 0;
            if (h != 0) {
                he_Unk53e840((int *)h);
                *(int *)(ev + 0x88) = 0;
            }
            *(HETIME *)ck = *(HETIME *)(ev + 0x52);
        }
        for (;;) {
            if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
                return;
            n = gm_Unk509400(ck, ev + 0x52, 0);
            *(float *)(ev + 0xb4) = *(float *)(ev + 0xb4) - (float)n * ((float)*(int *)(ev + 0x14) * 1.3f + *(int *)(ev + 0x18)) * rate;
            i = 0;
            while (!(*(float *)(ev + 0xb4) > 0.0f)) {
                *(float *)(ev + 0xb4) = (float)*(unsigned int *)(rec91 + 0x22) + *(float *)(ev + 0xb4);
                *(float *)(ev + 0xb8) = *(float *)(ev + 0xb4);
                i++;
            }
            if (i != 0) {
                *(int *)(ev + 0xc4) = ((int (*)(int, short, int))ai_Unk4c48e0)(*(int *)(ev + 0xc0), *(unsigned short *)(ev + 0xac), i);
            } else if (*(int *)(ev + 0xc4) == -1) {
                if (gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac)) == 0)
                    *(int *)(ev + 0xc4) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(b2 + 2), -1, *(unsigned short *)(ev + 0xac), 0, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            }
            *(HETIME *)ck = *(HETIME *)(ev + 0x52);
            gm_AddTime(ev + 0x52, 0, 1, 0);
            if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
                return;
        }
    }
lacking:
    t = gm_BuildingMainObject(b);
    if (*(int *)(ev + 0xc8) == 0) {
        owner = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
        if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7) {
            news = 0x5f1;
            chk = gm_FindCharacterByMaster(b);
            if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
                news = 0x60d;
            if (t != 0)
                he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, name);
            else
                he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), -1, 0, name);
        }
    }
    if (b != 0 && *(int *)(ev + 0xc8) == 0 && *(int *)(b + 0x95) != -1)
        ai_Unk4c5840(*(int *)(b + 1));
    *(int *)(ev + 0xc8) = 1;
    *(int *)(ev + 0xc4) = -1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x52);
    return;
state2:
    if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0) {
        *(int *)(ev + 0x70) = 1;
        return;
    }
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            sv_MoveToGroup(b, *(unsigned short *)rec, 0, 0);
    }
    if (b == 0)
        goto done_free;
    ai_Unk4c5840(*(int *)(b + 1));
    he_Unk53e1f0((int *)ev);
    return;
state_free:
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    if (bo[0] != 0)
        ai_Unk4c5840(*(int *)(bo[0] + 1));
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            sv_MoveToGroup(0, *(unsigned short *)rec, 0, 0);
    }
done_free:
    he_Unk53e1f0((int *)ev);
}
