/* 0x5ae570, the transport confirmation (305 B): more than 0x20 or no entries: out; the quantity is the last
   0x5087f0 of the entries (the word of the record, the byte +9); the text 0xdc (the count, the kind text 2 * the
   word of the first entry + 0x9a8, the quantity, or in the order of 0x5190a0) and, confirmed (0x521150, 0x101),
   the entries are paid (0x5034b0) and the order 0x4c47e0 is sent */

void ai_Unk5ae570(int param_1, int *param_2)
{
    char text[1024];
    int qty;
    int v;
    int id;
    int i;

    if (param_1 > 0x20)
        return;
    if (param_1 == 0)
        return;
    qty = 0;
    for (i = 0; i < param_1; i++)
        qty = (int)gm_ItemPrice(U_W(param_2[i]), *(unsigned char *)(param_2[i] + 9));
    v = ls_GetLanguage();
    id = param_1 < 2 ? U_W(*param_2) * 2 + 0x9a8 : 0xc10;
    if (v == 0)
        txt_Format(text, 0xdc, qty, id, param_1);
    else if (v == 1)
        txt_Format(text, 0xdc, param_1, id, qty);
    if (sv_MessageBox(text, 0x101) != 0) {
        for (i = 0; i < param_1; i++)
            gm_Unk5034b0(param_2[i]);
        cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, qty);
    }
}
