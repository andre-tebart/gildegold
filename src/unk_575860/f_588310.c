/* 0x588310, the drag window of an item (1596 B, ROUGH SKELETON: the call order, the text ids and the case values
   are complete; the widget layout arithmetic and the case split around the texts are summarised as comments):
   idx = the item index, item = 0x91aaf0 + idx * 0x41, o = 0x4cd9d0 (idx). The window "ToolTip\ToolTip_Objekte_
   Waffen" (0x62a940) when o and *o, else "ToolTip\ToolTip_Objekte_Handelsgut" (0x62a91c), 0x475600; the position
   from the window record 0xfb1024 + win * 0x2ac (0x26c / +6) and the screen width 0xfb9072 in 0x46cda0; the
   sections 2 / 1 / 3, 0x4731d0 (0, 0, idx + 0xdf) and the texts idx * 2 + 0x9a9 / idx * 2 + 0x9a8, 0x21 (the byte
   o + 0x40) or 0x1f (0x5089c0 (idx, 0x6f1bc5, (int)0x5087f0 (idx, 0x64)) as float -> int); the section 4 and the
   text 0x20 (o + 0x22) or, with 0x6f2308 / 0x6f230c / 0x75ec60 (the first non-0, 0 -> the text 0x20), the
   characters 0x770b80 + i * 0x21c (+0x16c == that, 0x300 of them) collected in 0x50cdc0 (that, the count, the
   list) -> float; the text 0x22 ((o + 0x22 + the character's byte +0x81 * 1 / 1008) * the float -> int); the
   text 0x25 with the section 4 / 3; the lists 0x61a348 (15 entries of 0x15, the word 0x61a349 == the record's
   word and != 0x1cd) 0x473350 (6, idx * 0xa + 0x28, the byte + 0x427, -2, U_I (0x6ed938)) and the text
   "%s$N" (0x62a90c, the text (the byte * 7 + 0x48e)); the case values 0x225 / 0x226 / 0x227 / 0x293 / 0x294 /
   0x295 -> the texts 0x48 / 0x49 / 0x4a or 0x4e / 0x4f / 0x50, 0x1c1 .. 0x1c3 -> 0x18 / 0x19, 0x1c4 .. 0x1c7 /
   0x2af / 0x2b0 -> 0x1e, 0x1bd .. 0x1c0 / 0x1d0 .. 0x1d2 -> 0x20, 0x1b7 .. 0x1b9 / 0x1ca .. 0x1cc -> 0x1e,
   0x1ba / 0x1bb / 0x1bc / 0x1cd .. 0x1cf -> 0x1f, each 0x473350 (6, the slot * 0xa + 0x28, the value + 0x427,
   -2) and "%s$N"; the 0x1c1 .. 0x1c7 / 0x2af / 0x2b0 kinds with the item kinds 0x17 / 0x25: the extra slot 0x444
   / 0x624; nothing at all -> the text 0x602568; the window */

float gm_GoodsPrice(unsigned id, unsigned char p);

int ai_Unk588310(int idx)
{
    char *item;
    char *o;
    int win;
    int n;
    int i;
    int j;

    item = (char *)U_I((unsigned)&ai_Goods) + idx * 0x41;
    o = (char *)ai_Unk4cd9d0(idx);
    win = d2_OpenForm(0, 0, (char *)((o != 0 && *(int *)o != 0) ? (unsigned)&full_data_62a940 : (unsigned)&full_data_62a91c));
    d2_CentreViewWindows(win);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0xdf), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    d2_SetForm(0, 3);
    if (*(unsigned char *)(o + 0x40) != 0) {
        io_PrintText(0x21, *(unsigned char *)(o + 0x40));
    } else {
        io_PrintText(0x1f, (int)((float (*)(int, int, int))gm_GoodsPrice)(idx, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)gm_ItemPrice(idx, 0x64)));
    }
    n = U_I((unsigned)&ai_PlayerBuilding);
    if (n == 0)
        n = U_I((unsigned)&ai_FallbackBuilding);
    if (n == 0)
        n = U_I((unsigned)&ai_Unk75ec60);
    d2_SetForm(0, 4);
    if (n == 0) {
        io_PrintText(0x20, *(int *)(o + 0x22));
    } else {
        /* ROUGH: the characters of n (the +0x16c field) collected and 0x50cdc0 (n, the count, the list) */
        gm_BuildingSkill(n, 1, 1);
        gm_FindCharacterByMaster(n);
        gm_Unk50cdc0(n, 0, 0);
        gm_FindCharacterByMaster(n);
        io_PrintText(0x22, (int)(*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)) * (float)*(unsigned char *)(o + 0x81) * 0.5 +
                                 (float)(int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8))));
        io_PrintText(0x20, *(int *)(o + 0x22));
    }
    /* ROUGH: the 0x61a348 / 0x61a34a lists (the word == the record's word, != 0x1cd) with 0x473350 (6, ...) and
       the text "%s$N" (0x62a90c); then the kind cases 0x225 .. 0x295 / 0x1b7 .. 0x2b0 with 0x473350 (6, ...) and
       "%s$N"; the extra slot 0x444 / 0x624 for the kinds 0x1c1 .. 0x1c7 / 0x2af / 0x2b0 with the item kinds
       0x17 / 0x25 */
    i = 0;
    j = 0;
    (void)item;
    (void)i;
    (void)j;
    if (win == 0)
        io_PrintText((char *)(unsigned)"$C");
    return win;
}
