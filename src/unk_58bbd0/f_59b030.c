/* 0x59b030, a credit payment of the money lender (304 B): the person 0x505df0 (param_2 + 0xb0), the text
   0x18d5 (param_2 + 0xb4); when 0x521040 (the person, param_2 + 0xb4, the player byte) and the confirmation
   0x521150 (text, 1) succeed: the payment 0x4c47e0 (param_1 + 1, param_2 + 0xb0, the player byte,
   param_2 + 0xb4); 0x506430 (the person) gets the amount at +0x48; when the character's kind
   (0x770b82 + slot * 0x21c) is not 5, the message 0x18d8 (the person, param_1, the amount) goes to 0x53dee0
   (the character 0x770b84 + slot * 0x21c, -1, text, 0, 0x5ec, param_1 + 1, -1, 0, 0); 0x53e1f0 (param_2) */

short * gm_FindChar(int id); void txt_Format(char *out, int id0, ...); int sv_CheckGoodAmount(); int sv_MessageBox();
short * gm_PlayerRecord(char *c); void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_Unk53e1f0(); int gm_Unk50a150();
void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...); void d2_SetObjText(); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
int d2_RemoveWindow(); void d2_RemoveObj(); int gm_Unk50a1c0(); int gm_SumSlotAmounts(void *list, unsigned char i);
int gm_Unk50a880(); void d2_AddPointerItem(int id, int amount); void d2_ClearPointerItems(void); int d2_CountPointerItems();
void d2_EndInputEdit(void); void d2_EnableObject(); void d2_ScrollWindowBy(); void d2_MoveObj(short x, short y, int obj);
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont(); void d2_CopyCaption(char *caption);
void d2_SetObjPic(); int cm_AddFieldDelta(int size, int n, short field, void *v); void cm_SendFieldDeltas(void); void cm_BeginFieldBlock(char *o, int id);
void sv_SetStatusText(char *s); int ai_Unk5e56c4(); void cm_BeginGroup(const char *s); void cm_EndGroup(const char *s);
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f); void cm_SendMoveGoods(int a, int b, unsigned char c, int d); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k);

void ai_Unk59b030(int param_1, int param_2)
{
    char text[0x400];
    char msg[0x200];
    unsigned short *per;
    unsigned short slot;
    int r;

    per = (unsigned short *)gm_FindChar(U_I(param_2 + 0xb0));
    txt_Format(text, 0x18d5, U_I(param_2 + 0xb4));
    if (param_1 == 0)
        return;
    if (sv_CheckGoodAmount(per, U_I(param_2 + 0xb4), U_I((unsigned)&ai_OwnPlayer)) == 0)
        return;
    if (sv_MessageBox(text, 1) == 0)
        return;
    slot = *(unsigned short *)(param_1 + 0x27);
    cm_SendMoveGoods(U_I(param_1 + 1), U_I(param_2 + 0xb0), U_I((unsigned)&ai_OwnPlayer), U_I(param_2 + 0xb4));
    r = gm_PlayerRecord(per);
    if (r != 0)
        U_I(r + 0x48) += U_I(param_2 + 0xb4);
    if (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + slot * 0x21c) != 5) {
        txt_Format(msg, 0x18d8, *per, param_1, U_I(param_2 + 0xb4));
        he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + slot * 0x87), -1, msg, 0, 0x5ec, U_I(param_1 + 1), -1, 0, 0);
    }
    he_Unk53e1f0(param_2);
}
