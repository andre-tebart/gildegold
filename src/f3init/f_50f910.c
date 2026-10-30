/* 0x50f910 @deep far B3 draft, 751 differing instructions of 1156 (1009 code hunks), every hunk read against 0x50f910..0x51072f in this and the previous session: no missing / extra statement, constant or callee left. Meaning fixed on the way (kept): ncl >= 8, the class loop <= 0xd, the unsigned ai_BTypes bytes, the 0x1b / 0x4c long-message field stores (AIMSG), void return, sv_PumpNet result discarded. Left: allocation / frame shape - the frame is 0x40c vs the original's 0x404 (8 bytes, every local slot shifted: cnt 0xc4 vs 0x7c, pl 0x64 vs 0x4c, ch2 0x84 vs 0x174, the 0xf8 message buffer at 0x7c), the cls / 0xb..0xd blocks' store order, and constants pushed from edi instead of as immediates (gm_BuildQuery's 0). spell.py last-bytes pass: rep-stos (the three memsets already match); reload 50fa18 / 50fa62 / 50fb14 / 50fd78 / 5103a1 / 5105db (the values are already int locals held across the calls, no source spelling left); dup-tail 50fc02 / 50fc9b / 50fcc4 / 50fd36 / 50fee9 / 50ff1c / 5100d6 / 51060d / 510686 (both arms are written; the 4-argument cm_SendRelations form and the ai_Chars+2 walker base tried as single edits, REVERTED at 762-763 asm lines vs 753); push-pop 51015c (the early-return guard does not apply); width 50fdbc / 51043e checked (both sides read the same width). Q1-Q7 pass. match-deep owns the frame job. */


/* 0x50f910, init_OpenTown: the opening of a new town's buildings (called by the new-game setup 0x511400; no return
   value, the caller ignores it). For each of up to eight player characters (kind 5 / 6 / 7) the first free building
   whose class lists the character's job (+0x164) is opened (0x50f750); a character without one gets a building of its
   job built from the prototypes (0x58f640 / 0x58f7a0, 0x4c5ae0). The characters are then sent into houses (0x501190 /
   0x501f60 / 0x4c5540 / 0x4c54a0), the buildings of the classes 0xb .. 0xd and 0x10 get their persons and goods, the
   other free buildings whose class is marked 1 in the table below are stocked (cls[]: 0 = nothing, 1 = one or two
   persons, 2 / 3 = special), the start characters (0x50e690) and the opening goods (0x4f8ed0) are made, a message
   0x1b goes to every building (types E / D / F skipped) and the mission-mode message 0x4c out, and finally the
   characters of the kinds 2 .. 7 greet each other (0x4c4b00 both ways). */
extern char *ai_Blds;              /* 0x889f74, the buildings (0xa9 bytes each, 0x100) */
int ai_Unk5fcc6c = 128;  /* 0x5fcc6c */
unsigned int d3_LightKeyBudgetVisible = 512;  /* 0x5fcc70 */
unsigned char init_Detail = 2;  /* 0x61adf8, the detail level of the scene (4 = high / 2 / 1 = low, a byte) */
float init_SoundVolume = 0.699999988f;  /* 0x61adfc, 0.7f: the volume factor of a started town sound ( */
int init_ShopScript = -1;  /* 0x61ae00, the handle of the shopping script running in the town scene (-1 = none; */
INIT_ANIMSET init_ClassAnims[19] = { { 20, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "arbeit/mist_schaufeln", "arbeit/neufegen2", "arbeit/schweissab", "arbeit/schmieden", "", "", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/einhand_vom_tisch", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/manipulieren_stehend", "gestik/zeigen", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "spezial/auf_tisch_legen", "spezial/meckern", "", "", "", "", "", "", "", "", "" } }, { 5, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/haende_reiben", "arbeit/schreiben", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 7, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "gestik/manipulieren_stehend", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/bein_kratzen", "gestik/zeigen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "arbeit/neufegen2", "", "", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 6, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/aufstehen", "sitzend/hinsetzen", "sitzend/wuerfeln", "spezial/meckern", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 8, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/manipulieren_sitzend", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "spezial/meckern", "", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech3", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech5", "gestik/gespraech6", "gestik/zeigen", "", "", "", "", "", "", "", "", "" } }, { 14, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "spezial/meckern", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "arbeit/umruehren_auftisch", "", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/zeigen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/manipulieren_stehend", "", "", "", "", "", "", "", "", "" } }, { 15, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/steh_noise9", "gestik/durch_haar_fahr", "spezial/einhand_vom_tisch", "arbeit/neufegen2", "arbeit/schweissab", "", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/bein_kratzen", "gestik/steh_noise7", "gestik/steh_noise8", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 16, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "arbeit/schoepfen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 18, { "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/zeigen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech5", "gestik/gespraech6", "gestik/manipulieren_hockend", "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/auf_tisch_legen", "spezial/in_andere_hand_geben", "spezial/runterbeug_such", "spezial/einhaendig_vom_boden_nehmen", "arbeit/schweissab", "arbeit/schmieden", "arbeit/schoepfen", "arbeit/metall_erhitzen", "arbeit/neufegen2", "gestik/manipulieren_stehend", "spezial/meckern", "spezial/einhand_vom_tisch", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "", "", "" } }, { 19, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech1", "sitzend/sitzgespraech2", "sitzend/sitzgespraech3", "sitzend/sitzgespraech4", "sitzend/sitzgespraech5", "sitzend/sitzgespraech6", "sitzend/sitzgespraech7", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/gespraech5", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 9, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "gestik/zeigen", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 13, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "arbeit/schreiben", "arbeit/schoepfen", "", "gestik/zeigen", "kampf/stechen", "kampf/streich", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 21, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/einhand_vom_tisch", "spezial/runterbeug_such", "arbeit/schweissab", "arbeit/schmieden", "arbeit/saegen", "arbeit/hobeln", "arbeit/neufegen2", "", "", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/zeigen", "gestik/kopf_nicken", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/manipulieren_hockend", "spezial/meckern", "spezial/auf_tisch_legen", "", "", "", "", "", "" } }, { 11, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/einhand_vom_tisch", "arbeit/schreiben", "arbeit/schoepfen", "arbeit/schmieden", "arbeit/schweissab", "arbeit/baumfaellen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/jubeln", "spezial/meckern", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 22, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/zeigen", "spezial/meckern", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "arbeit/zapfen", "arbeit/umruehren_auftisch", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech1", "sitzend/sitzgespraech2", "sitzend/sitzgespraech3", "sitzend/sitzgespraech4", "sitzend/sitzgespraech5", "sitzend/sitzgespraech6", "sitzend/sitzgespraech7", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen" } }, { 1, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/gespraech5", "gestik/haende_reiben", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 4, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 12, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "arbeit/hacken", "arbeit/mist_schaufeln", "arbeit/schweissab", "", "gestik/manipulieren_stehend", "gestik/bein_kratzen", "gestik/manipulieren_hockend", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 0, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } } };  /* 0x61ae08 .. */
int init_AiTown = 1;  /* 0x624564: .data initial 1, no writer in the exe; nonzero = the opening also stocks the free buildings of the class-1 table and sends the computer players' mail (the AI part of the town) */
extern char ai_MissionMode;        /* 0x624580 */
/* ai_CharInts: view of ai_Chars (tools/data_views.py) */
void init_OpenTown(void)
{
    char cls[0x22] = {0, 0, 3, 0, 1, 1, 0, 1, 1, 1, 0, 2, 2, 2, 1, 0, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 3, 3, 1, 0, 1, 1, 0};
    int cnt[0x22];
    int pl[8];
    int ch2[16];
    char vecA[16], vecB[12];
    char buf260[96];
    int list200[128];
    int ncl, i, j, k, r, q, h, n, nch, m, ok;
    char *p, *b, *c;
    unsigned char *cl;                 /* the ai_BTypes record's bytes: the original reads every one zero-extended */
    unsigned u;
    int msg[8];
    unsigned char bk;

    memset(cnt, 0, sizeof(cnt));
    memset(pl, 0, sizeof(pl));
    memset(ch2, 0, sizeof(ch2));
    ncl = 0;
    p = (char *)ai_Chars + 2;
    do {
        if (ncl >= 8)
            break;
        if (((INIT_CHAR *)(p - 2))->id != 0xffff && (*p == 6 || *p == 7 || *p == 5)) {
            pl[ncl] = (int)(p - 2);
            ncl++;
        }
        p += 0x21c;
    } while ((int)p < INIT_CHARACTERS_END + 2);
    for (i = 0; i < ncl; i++) {
        c = (char *)pl[i];
        if (c == 0 || !(((INIT_CHAR *)c)->kind == 6 || ((INIT_CHAR *)c)->kind == 7 || ((INIT_CHAR *)c)->kind == 5))
            continue;
        ok = 0;
        for (k = 0; k < 0xa900 && !ok; k += 0xa9) {
            b = ai_Blds + k;
            if (((INIT_BLD *)b)->type != 0 && ((INIT_BLD *)b)->owner == 0xffff) {
                cl = (unsigned char *)ai_BTypes + ((INIT_BLD *)b)->type * 0x24d;
                for (j = 0; j < 6; j++) {
                    if (cl[0x223 + j] != 0 && cl[0x223 + j] == (unsigned char)c[0x164]) {
                        h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)b)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        init_OpenBuilding((INIT_BLD *)b);
                        ok = 1;
                        cnt[(unsigned char)*cl]++;
                        break;
                    }
                }
            }
        }
        if (!ok) {
            int proto = gm_FirstTitleNumber((unsigned char)c[0x164]);

            if (ai_BuildingName(proto, buf260) != 0 && (n = ai_FindBauplaetze(buf260, list200)) != 0) {
                m = 0;
                if (n > 0) {
                    do {
                        if (list200[m] != 0)
                            break;
                        m++;
                    } while (m < n);
                }
                d3_PointToWorld((struct D3OBJECT *)list200[m], (D3VECTOR *)(list200[m] + 0x4c), (D3VECTOR *)vecA);
                ai_ObjectAngle(vecB, list200[m]);
                h = cm_RequestCreateGeb(((INIT_CHAR *)c)->netId, proto, vecA, vecB);
                if (h != -1) {
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                }
                h = cm_GetSlotValue(h);
                if (h != 0)
                    init_OpenBuilding((INIT_BLD *)h);
            }
        }
    }
    n = ncl;
    main_SetLoadProgress(0, 0x55);
    for (i = 0; i < n; i++) {
        c = (char *)pl[i];
        if (c == 0)
            continue;
        q = gm_BuildQuery(1, 4, ((INIT_CHAR *)c)->id);
        if (q != 0 && gm_IsHomeType(q) != 0)
            q = 0;
        if (((INIT_CHAR *)c)->kind == 5 || ((INIT_CHAR *)c)->kind == 4) {
            if (q == 0) {
                r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
                if (r == 0)
                    r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
            } else {
                r = gm_NearestFreeBuilding(q, 5);
                if (r == 0)
                    r = gm_NearestFreeBuilding(q, 4);
            }
        } else if (q == 0) {
            r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
            if (r == 0)
                r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
        } else {
            r = gm_NearestFreeBuilding(q, 4);
            if (r == 0)
                r = gm_NearestFreeBuilding(q, 5);
        }
        if (r == 0)
            break;
        cm_SendBoss(((INIT_CHAR *)c)->netId, ((INIT_BLD *)r)->id);
        h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)r)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
        while (cm_GetSlotState(h) == 0)
            sv_PumpNet();
    }
    main_SetLoadProgress(0, 0x5a);
    for (i = 0xb; i <= 0xd; i++) {
        b = (char *)gm_BuildQuery(2, 4, 0xffff, 5, i);
        if (b != 0) {
            n = gm_EquipmentValue(b, 100);
            cm_BeginFieldBlock(b, ((INIT_BLD *)b)->id);
            cm_AddFieldDelta(4, 1, 0x65, &n);
            n = n * 2;
            cm_AddFieldDelta(4, 1, 0x69, &n);
            cm_SendFieldDeltas();
            cl = (unsigned char *)ai_BTypes + ((INIT_BLD *)b)->type * 0x24d;
            cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)ai_PlayerCharC)->netId, ((INIT_CHAR *)ai_PlayerCharC)->netId);
            cm_SendMessage3d(b, 1, cl[0x230], cl[0x223]);
            cm_SendMessage3d(b, 0, cl[0x22f], 0);
            cm_SendMessage3d(b, 0, cl[0x22f], 0);
            cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
            cnt[i]++;
        }
    }
    main_SetLoadProgress(0, 0x5f);
    b = (char *)gm_BuildQuery(2, 4, 0xffff, 5, 0x10);
    if (b != 0) {
        cl = (unsigned char *)ai_BTypes + ((INIT_BLD *)b)->type * 0x24d;
        h = cm_SendMessage3d(b, 2, cl[0x230], 0x2d);
        while (cm_GetSlotState(h) == 0)
            sv_PumpNet();
        h = cm_GetSlotValue(h);
        cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)h)->netId, ((INIT_CHAR *)h)->netId);
        cm_SendMessage3d(b, 0, cl[0x22f], 0);
        cm_SendMessage3d(b, 0, cl[0x230], 0);
        cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
        cnt[0x10]++;
    }
    nch = 0;
    if (init_AiTown != 0) {
        for (k = 0; k < 0xa900; k += 0xa9) {
            b = ai_Blds + k;
            if (((INIT_BLD *)b)->type != 0 && ((INIT_BLD *)b)->owner == 0xffff) {
                cl = (unsigned char *)ai_BTypes + ((INIT_BLD *)b)->type * 0x24d;
                bk = *cl;
                if (cls[bk] == 1 && cnt[bk] < 2) {
                    unsigned char w = 0;

                    cm_BeginSendGroup();
                    for (j = 5; j >= 0; j--) {
                        if (cl[j + 0x223] != 0) {
                            w = cl[j + 0x223];
                            break;
                        }
                    }
                    h = cm_SendMessage3d(b, 2, cl[0x230], w);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    h = cm_GetSlotValue(h);
                    cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)h)->netId, ((INIT_CHAR *)h)->netId);
                    u = gm_PlayerFactor(3000, ai_OwnPlayer);
                    cm_SendMoveGoods(((INIT_CHAR *)h)->netId, -1, ai_OwnPlayer, u);
                    if (nch < 0x10) {
                        ch2[nch] = h;
                        nch++;
                    }
                    if (*cl != 9 && *cl != 5) {
                        if (*cl == 4 || *cl == 0x10 || *cl == 0x13) {
                            cm_SendMessage3d(b, 0, cl[0x22f], 0);
                            cm_SendMessage3d(b, 0, cl[0x230], 0);
                        } else {
                            cm_SendMessage3d(b, 0, cl[0x22f], 0);
                        }
                    }
                    cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
                    if (*cl == 5) {
                        u = gm_PlayerFactor(6000, ai_OwnPlayer);
                        cm_SendMoveGoods(((INIT_CHAR *)h)->netId, -1, ai_OwnPlayer, u);
                    }
                    init_Betrieb((INIT_BLD *)b);
                    cm_EndSendGroup();
                    cnt[bk]++;
                }
            }
        }
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
        for (i = 0; i < nch; i++) {
            c = (char *)ch2[i];
            if (c == 0)
                continue;
            q = gm_BuildQuery(1, 4, ((INIT_CHAR *)c)->id);
            if (q == 0 || gm_IsHomeType(q) != 0) {
                r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
                if (r == 0)
                    r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
            } else {
                r = gm_NearestFreeBuilding(q, 5);
                if (r == 0)
                    r = gm_NearestFreeBuilding(q, 4);
            }
            if (r == 0)
                break;
            cm_SendBoss(((INIT_CHAR *)c)->netId, ((INIT_BLD *)r)->id);
            h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)r)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        for (; i < nch; i++) {
            if (ch2[i] != 0) {
                u = gm_BuildingKindValue(4);
                cm_SendMoveGoods(((INIT_CHAR *)ch2[i])->netId, -1, ai_OwnPlayer, u);
            }
        }
    }
    cm_BeginSendGroup();
    for (k = 0; k < 0xa900; k += 0xa9) {
        unsigned short o;

        b = ai_Blds + k;
        cl = (unsigned char *)ai_BTypes + ((INIT_BLD *)b)->type * 0x24d;
        o = ((INIT_BLD *)b)->owner;
        if (o == 0xffff
            || ((cls[(unsigned char)*cl] != 2 && cls[(unsigned char)*cl] != 1)
                || (ai_Chars[o][2] != 5 && ai_Chars[o][2] != 4))) {
            if (cls[(unsigned char)*cl] == 0 && o == 0xffff)
                cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)ai_PlayerCharC)->netId, ((INIT_CHAR *)ai_PlayerCharC)->netId);
        } else if (init_AiTown != 0) {
            if (*cl != 9 && *cl != 5) {
                if (*cl == 4 || *cl == 0x10 || *cl == 0x13) {
                    cm_SendMessage3d(b, 0, cl[0x22f], 0);
                    cm_SendMessage3d(b, 0, cl[0x230], 0);
                } else {
                    cm_SendMessage3d(b, 0, cl[0x22f], 0);
                }
            }
            cm_SendMessage3d(b, 1, cl[0x230], cl[0x223]);
            cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
            init_Betrieb((INIT_BLD *)b);
        }
    }
    cm_EndSendGroup();
    while (cm_SendGroupState() == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
    }
    ((int (*)(int))init_CreateStartCharacters)(0);   /* @original: the original call pushes one argument (0x510494) although the callee reads none */
    ord_UpdateStatistics();
    b = (char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (((INIT_BLD *)b)->masterId == 0xffff && ((INIT_BLD *)b)->type != 'E' && ((INIT_BLD *)b)->type != 'D' && ((INIT_BLD *)b)->type != 'F') {
            INIT_AIMSG m2;

            m2.type = 0x1b;
            m2.to = ((INIT_CHAR *)ai_PlayerCharC)->netId;
            m2.fromId = -1;
            m2.flags = 2;
            m2.arg[0] = ((INIT_BLD *)b)->id;
            m2.arg[1] = gm_EquipmentValue((unsigned char *)b, (char)((INIT_BLD *)b)->level);
            cm_SendLongMessage((char *)&m2);
        }
        b = (char *)gm_BuildNext();
    }
    if (ai_MissionModeC == 5) {
        INIT_AIMSG m2;
        INIT_TIME *clock = (INIT_TIME *)ai_Time;

        for (i = 0, p = ai_Chars[0]; ((INIT_CHAR *)p)->kind != 6 && (int)(p + 0x21c) < INIT_CHARACTERS_END + 2; p += 0x21c)
            i++;
        memset(&m2, 0, sizeof(m2));
        m2.type = 0x4c;
        m2.to = ai_CharInts[i][0];
        m2.fromId = ai_CharInts[i][0];
        m2.nIds = 0;
        m2.reserved14 = 0;
        m2.time = *clock;
        m2.flags = 1;
        cm_SendLongMessage((char *)&m2);
    }
    he_PostEvent6d();
    he_PostEvent6b();
    he_PostEvent7a();
    he_PostEvent2b();
    nch = 0;
    p = ai_Chars[0];
    do {
        bk = ((INIT_CHAR *)p)->kind;
        u = bk;
        if (bk == 6 || bk == 7 || bk == 5 || bk == 3 || bk == 4 || bk == 2) {
            char *p2;

            m = 0;
            u = nch + 1;
            if ((int)u < 0x300) {
                p2 = p + 0x21c;
                do {
                    bk = ((INIT_CHAR *)p2)->kind;
                    if (bk == 6 || bk == 7 || bk == 5 || bk == 3 || bk == 4 || bk == 2) {
                        u = util_RandomU(0x40);
                        ((int (*)(void *, void *, int))cm_SendRelations)(((INIT_CHAR *)p)->netId, ((INIT_CHAR *)p2)->netId, (u & 0xffff) - 0x20);
                        u = util_RandomU(0x40);
                        u = ((int (*)(void *, void *, int))cm_SendRelations)(((INIT_CHAR *)p2)->netId, ((INIT_CHAR *)p)->netId, (u & 0xffff) - 0x20);
                        m += 2;
                        if (u != 0xffffffff) {
                            while (m % 10 == 0 && cm_GetSlotState(u) == 0)
                                sv_PumpNet();
                        }
                    }
                    p2 += 0x21c;
                } while ((int)p2 < INIT_CHARACTERS_END + 2);
            }
        }
        p += 0x21c;
        nch++;
    } while ((int)p < INIT_LAST_CHARACTER + 2);
}
