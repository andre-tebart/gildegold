/* 0x506890, the reaction to an order of the character a for the character b (the target): a's order record (0x4f2540) is answered (0x4f01d0; for b of the same group 0x4f0900); every object of a (0x501190 (1, 4, id) / 0x501030
   iteration) is told to b (message 0x4c54a0; for the player's own character also a message 0x1b and 0x31 to the other players with an interest), the objects are collected (up to 0x240) and the result is put into the
   player's text (0x1c4a .. 0x1c4e; a list of at most 6 objects plus "and n more" 0xb6) and sent (0x53dda0) to the player(s) concerned. ROUGH */

int gm_EquipmentValue(unsigned char *b, int unused);                     /* 0x509b10 */
int gm_Unk509eb0();                     /* 0x509eb0 */
void ai_Unk4c5010(int a, int b, int c);                     /* 0x4c5010 */
void ai_Unk4c4ac0(int a, int b, int c);                     /* 0x4c4ac0 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
float util_RandomSq(void);                  /* 0x42ec40 */

int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
int gm_BuildingClass();                     /* 0x501d80 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
int gm_Unk509eb0();                     /* 0x509eb0 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
unsigned char * gm_BuildNext(void);                     /* 0x501030 */

void gm_Unk506890(char *a, char *b)
{
    char *objs[0x240];
    char t1[0x800];
    char t2[0x1000];
    char m[0xf8];
    char *o;
    int *e;
    char *s;
    char *pl;
    int n, flag, k, i, j, v;

    n = 0;
    flag = 0;
    if (unk_Unk4f2540(a, (unsigned char **)&o, (int *)&e) != 0) {
        unk_Unk4f01d0((int *)a, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
        if (b != 0 && b != ai_PlayerChar && e[1] == *(int *)(a + 4))
            unk_Unk4f0900((int *)b, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
    }
    s = (char *)gm_BuildQuery(1, 4, *(unsigned short *)a);
    pl = ai_PlayerChar;
    while (s != 0) {
        k = gm_BuildingClass((unsigned char)s[0]);
        if (k == 2)
            cm_SendOwnership(*(int *)(s + 1), *(int *)(pl + 4), *(int *)(pl + 4));
        else {
            cm_SendOwnership(*(int *)(s + 1), *(int *)(b + 4), *(int *)(b + 4));
            flag = 1;
        }
        pl = ai_PlayerChar;
        if (b == pl && k != 2 && (s[0x5a] & 1) == 0) {
            memset(m, 0, 0xf8);
            m[4] = 0x1b;
            *(int *)(m + 8) = *(int *)(pl + 4);
            *(int *)(m + 0xc) = -1;
            m[0x36] = 2;
            *(int *)(m + 0x58) = *(int *)(s + 1);
            *(int *)(m + 0x5c) = gm_EquipmentValue(s, (char)s[0x5c]);
            cm_SendLongMessage(m);
            for (i = 0; (char *)((unsigned)&ai_CharInts + i * 0x21c) < (char *)((unsigned)&gm_PlayerRecordBytes + 0x2); i++) {
                char *c = ai_Chars[i];
                if (*(char **)(c + 0x168 + 4) == s && *(int *)(c + 4) != *(int *)(a + 4)) {
                    memset(m, 0, 0xf8);
                    m[4] = 0x31;
                    *(int *)(m + 8) = *(int *)(pl + 4);
                    *(int *)(m + 0xc) = *(int *)(s + 1);
                    *(int *)(m + 0x28) = *(int *)ai_Time;
                    *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
                    *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
                    *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                    m[0x36] = 1;
                    *(int *)(m + 0x58) = *(int *)(c + 4);
                    cm_SendLongMessage(m);
                }
            }
            pl = ai_PlayerChar;
        }
        objs[n++] = s;
        s = (char *)gm_BuildNext();
    }
    if (b != pl) {
        v = gm_Unk509eb0(a);
        cm_SendMoveGoods(*(int *)(b + 4), -1, ai_OwnPlayer, v);
        v = gm_GetWorth(*(unsigned short *)a);
        cm_BeginFieldBlock(b, *(int *)(b + 4));
        ((int (*)(int, int, int))cm_AddFieldValues)(4, 1, (b - ai_Chars[0]) + 0x1a8);
        if (flag && (b[2] == 3 || b[2] == 8 || b[2] == 9))
            cm_AddFieldDelta(1, 1, (b - ai_Chars[0]) + 2, &v);
        cm_SendFieldDeltas();
        pl = ai_PlayerChar;
    }
    if (b[2] != 6 && b[2] != 7) {
        if ((a[2] == 6 || a[2] == 7 || a[2] == 5) && b == pl) {
            for (i = 0; i < 0x300; i++) {
                char *c = ai_Chars[i];
                if ((c[2] == 6 || c[2] == 7) && *(int *)(c + 4) != *(int *)(a + 4)) {
                    txt_Format(t1, 0x1c4d, *(unsigned short *)a);
                    he_ShowNews(*(int *)(c + 4), -1, t1, 0, 0x5fe, "_NACHRICHTEN_HS_67");
                }
            }
            return;
        }
        for (i = 0; i < 0x300; i++) {
            for (j = 0; j < 8; j++) {
                char *c = ai_Chars[i];
                if ((c[2] == 6 || c[2] == 7) && *(int *)(c + 0x58 + j * 4) == *(int *)(a + 4)) {
                    txt_Format(t1, 0x1c4e, *(unsigned short *)a);
                    he_ShowNews(*(int *)(c + 4), -1, t1, 0, 0x5f4, "_NACHRICHTEN_HS_67");
                    i = 0x300;
                }
            }
        }
        return;
    }
    txt_Format(t2, 0x1c4a, *(unsigned short *)a);
    if (n != 0) {
        txt_Format(t1, 0x1c4b);
        strcat(t2, t1);
        for (i = 0; i < n; i++) {
            if (i >= 6) {
                if (i < n) {
                    txt_Format(t1, 0xb6, n - i);
                    strcat(t2, t1);
                }
                break;
            }
            if ((objs[i][0x5a] & 1) == 0)
                txt_Format(t1, "%1G$A", objs[i]);
            strcat(t2, t1);
        }
    }
    txt_Format(t1, 0x1c4c, gm_Unk509eb0(a));
    strcat(t2, t1);
    he_ShowNews(*(int *)(b + 4), -1, t2, 1, 0x5ff, "_NACHRICHTEN_HS_67");
}
