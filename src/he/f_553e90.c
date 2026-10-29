/* 0x553e90, the run handler of a building event (a Ghidra function with a frame; ROUGH): state 0 -> 1; state 1 waits for the due time, takes the building of the kind +0x10 (0x501190 (1, 1, +0x10); none: the record is freed), asks its owner record (0x4ffe90) and starts the order 0x4c4880 (+0xb0, -1, +0xac, 1, player, 0), whose handle goes to +0xb4, state 2; state 2 waits for the order (0x4c3ae0 / 0x4c3b10 give the person), then by the type of the person (0x1f9 / 0x1fd / 0x201: 0x4f0a90 (the owner character, building, 1 / 2 / 3), 0x12d: a message record type 0x2a through 0x4c4b70) tells the owner (a player character gets the news 0x606 / 0x60b with "ob_%s" of the table name 0x91aaf0, texts 0x1c2b / 0x1c2d and "_NACHRICHTEN_HS_53" / "_54") and the town (0x4c4db0 (4, ..)), marks the person (+0x13 |= 0xa0) and frees the record */

int cm_GetSlotState();
int cm_GetSlotValue(int h);
unsigned char *gm_BuildQuery(int n, ...);
char *gm_Query(void *list, int n, ...);
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);
int unk_Unk4f0a90();
int cm_SendLongMessage(char *msg);
void cm_SendExSysMessage(unsigned char id, void *msg);
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);

void he_Unk553e90(int ev)
{
    int st = *(int *)(ev + 0x70);
    int h, p, b, t, q, own;
    unsigned short ty;
    unsigned char msg[0xf8];
    char text[0x400];
    char name[0x80];
    char buf2[0x80];
    int c[4];
    char *rec;
    int tid;

    if (st == 0) {
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
        *(int *)(ev + 0xb4) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(ev + 0xb0), -1, *(unsigned short *)(ev + 0xac), 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    }
    if (st != 2)
        return;
    if (cm_GetSlotState(*(int *)(ev + 0xb4)) == 0)
        return;
    p = cm_GetSlotValue(*(int *)(ev + 0xb4));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0 || p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    ty = *(unsigned short *)p;
    if (ty == 0x1f9)
        unk_Unk4f0a90((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 1);
    else if (ty == 0x1fd)
        unk_Unk4f0a90((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 2);
    else if (ty == 0x201)
        unk_Unk4f0a90((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 3);
    else if (ty == 0x12d) {
        memset(msg, 0, sizeof(msg));
        msg[4] = 0x2a;
        *(int *)(msg + 0xc) = *(int *)(b + 1);
        *(int *)(msg + 0x58) = *(int *)(b + 1);
        *(int *)(msg + 8) = *(int *)(U_I((unsigned)&ai_PlayerChar) + 4);
        msg[0x36] = 2;
        cm_SendLongMessage(msg);
    }
    own = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(unsigned char *)(own + 2) == 6) {
        q = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
        rec = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        tid = 0x606;
        if (*(unsigned char *)(*(unsigned char *)b * 0x24d + U_I((unsigned)&ai_BTypes)) != 6) {
            int o = gm_FindCharacterByMaster(b);

            if (o != 0 && *(unsigned char *)(o + 2) == 1 && (*(unsigned char *)(o + 0x1b4) & 1) != 0 && *(int *)(ev + 0xb8) != 0)
                tid = 0x60b;
        }
        if (q != 0 && *(unsigned short *)q != 0xfd) {
            strcpy(name, rec + 1);
            _strupr(name);
            sprintf(buf2, "ob_%s", name);
            txt_Format(text, 0x1c2b, b, *(unsigned short *)(ev + 0xac) + 0x9a8);
            ((void (*)(int, int, char *, char, int, int, int, char *, char *, int))he_MessageBoxQuickjump)(*(int *)(own + 4), *(int *)(ev + 0x10), text, 0, tid, *(int *)(b + 1), *(int *)(ev + 0xb0), buf2, "_NACHRICHTEN_HS_53", 0);
        } else {
            txt_Format(text, 0x1c2d, b, *(unsigned short *)(ev + 0xac) + 0x9a8);
            he_MessageBoxQuickjump(*(int *)(own + 4), *(int *)(ev + 0x10), text, 0, tid, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_54");
        }
        *(unsigned char *)(p + 0x13) |= 0xa0;
    }
    q = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
    *(HETIME *)c = ai_Time;
    gm_AddTime(c, 0, 0, 1);
    c[0] = *(int *)(b + 1);
    c[1] = *(int *)(q + 2);
    cm_SendExSysMessage(4, c);
    he_Unk53e1f0((int *)ev);
}
