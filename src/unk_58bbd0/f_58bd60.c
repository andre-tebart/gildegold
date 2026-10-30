/* 0x58bd60, the production window "Handel\nHandel_PRODUKTION" (2336 B, ROUGH SKELETON): opens when 0x520fe0
   (the building record) is 0, the event 0x576040 (0x1f, 0, 0, 0); the window 0x4741f0 (0, 0, 0x62b038) and
   its text by the building kind (0x7e3f30 + kind * 0x24d, byte 0: the texts 0x72f / 0x70f / 0x713 / 0x717);
   0x525220 (the record, 2 for the kind 0x1e), 0x58bbd0 (the tables), the control 0x475ff0 and 0x52e860
   (0x270, 0x15e), the window entry +0x74 = 0x54; the frame loop 0x5309a0 (0x58bd60, 0x677c7) with the two
   0x4ffe90 person queries (2, 6, 0, 0xff / 0x2a), the sell list 0x6f5320 / 0x6f5354 (the goods of the
   character, 0x50ab80 / 0x50aaf0 the amounts, 0x53e400 / 0x53e300 the carriers, 0x576340 the confirmation,
   0x4c4b70 the order, 0x4c3ae0 its answer), the 0x75f290 / 0x75f2a0 lists and 0x58cc10 / 0x58d720 / 0x58dd00
   (the columns); the exit writes the progress byte param_1[0x59] and restores the event. ROUGH: the list
   sorting and the per-goods checks are condensed to their calls; the cursor and the column handlers are the
   calls at the tail (0x58cc10, 0x58d720, 0x58dd00) */

struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); int sv_CheckCharacterFlag(); int d2_OpenForm(); void d2_CentreViewWindows(int view);
float gm_GoodsPrice(unsigned id, unsigned char p);   /* 0x5089c0, the price (ai.h / cm.h / rt.h: float) */
void gm_Unk508a80(int v, unsigned char k, char *out);  /* 0x508a80, above its first call (line 1119) */
void sv_OpenPlayerBar(int a, char k); int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...); void sv_Unk52e6d0(int f, int b, int c);
int d2_GetViewWin(); int sv_CreateFormSlider(); void d2_SetPointerSprite(int a, int pic); char * gm_Query(void *list, int n, ...);
int gm_FreeCapacity(); int sv_MessageBox(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int ai_Unk576340(); int cm_SendLongMessage(char *msg); int cm_GetSlotState(); int cm_GetSlotValue(int h);
void snd_PlayFavourComment(void); void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g); int sv_RunFrame(int cb, unsigned mask); void d2_CloseView(int view);
void sv_ClosePlayerBar(void); int gm_ListAmount(); char * gm_Query(void *list, int n, ...); void ai_Unk58cc10();
int ai_Unk58d720(); void ai_Unk58dd00(int param_1, int param_2, int param_3, int param_4); char *_itoa(int, char *, int); void d2_SetObjText();

int ai_ProductionWindow(unsigned char *param_1)
{
    int a;
    int b;
    int c;
    int win;
    int id;
    int p1;
    int p2;
    int n;
    int i;
    int first;
    char *r;
    char kind;

    first = 1;
    if (sv_CheckCharacterFlag(param_1) != 0)
        return 0;
    ai_Unk576040(0x1f, 0, 0, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)"Handel\\Handel_PRODUKTION");
    d2_CentreViewWindows(win);
    kind = *(char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    sv_OpenPlayerBar(param_1, kind == 0x1e ? 2 : 0);
    d2_SetForm(win, 3);
    io_PrintText(0xb7);
    ai_Unk58bbd0();
    d2_SetForm(win, 0);
    n = 0x70b;
    if (kind == 0x13 || kind == 4 || kind == 0x10)
        n = 0x72f;
    else if (kind == 8 || kind == 0xe)
        n = 0x70f;
    else if (kind == 6)
        n = 0x713;
    else if (kind == 0x16)
        n = 0x717;
    else if (kind == 0xd || kind == 0xc || kind == 0xb)
        n = 0x717;
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), n, 0x742);
    id = d2_GetViewWin(win, 2, 0x78, (char *)&a, (char *)&b, (char *)&c);
    sv_CreateFormSlider(0x270, 0x15e, U_I((unsigned)&d2_LastWindow), id);
    *(short *)(U_I((unsigned)&d2_Objs) + 0x74 + c * 0x2ec) = 0x54;
    d2_SetPointerSprite(0, 0);
    while (1) {
        d2_SetForm(win, 2);
        p1 = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0xff);
        if (p1 == 0)
            return 0;
        p2 = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x2a);
        if (p2 == 0)
            return 0;
        /* ROUGH: the sell / buy lists: the clicked control 0x5fe710 selects a goods entry of the list
           0x6f5320 (the record 0x91aaf0 + id * 0x41 words), 0x50ab80 checks the carrier, 0x521150 shows
           the error texts, 0x53e400 / 0x53e300 walk the carriers, 0x576340 (0x20, param_1, 0, the id) asks,
           0x4c4b70 (the 0x3e-dword order record local_f8) sends the order and 0x4c3ae0 waits for it; the
           0x75f290 / 0x75f2a0 lists mark the selected carriers (0x524260, 0x770d08) */
        /* ROUGH: the column handlers */
        ai_Unk58dd00(win, 1, 0, param_1);
        if (first != 0) {
            d2_SetForm(win, 2);
            first = 0;
            U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4) =
                U_I(((unsigned)&ai_Unkf8f13c + 0x248) + U_I((unsigned)&d2_LastWindow) * 0x3c4) * (int)(char)param_1[0x59];
        }
        ai_Unk58d720(param_1, p2);
        n = sv_RunFrame((int)ai_ProductionWindow, 0x677c7);
        if (n == 0)
            break;
    }
    d2_SetForm(win, 2);
    param_1[0x59] = (unsigned char)((U_I(((unsigned)&ai_Unkf8f13c + 0x234) + U_I((unsigned)&d2_LastWindow) * 0x3c4) +
                                     U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4)) /
                                    U_I(((unsigned)&ai_Unkf8f13c + 0x248) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    d2_CloseView(win);
    sv_ClosePlayerBar();
    ai_Unk576040(0x1f, 0, 1, 0);
    return 0;
}
