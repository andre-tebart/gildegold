/* 0x50f340, the market stalls of the town: for every market building (prototype 0x47) the two stall objects
   of its object list - 0x1db the sale stall, 0x1dc the purchase stall - get the number of goods in the
   city's import list (0x1db) / export list (0x1dc) (the 16 words at +0x2ac / +0x2cc of the owner city's
   player slot) as their count field, the capacity 4, and both directions of the goods are sent to the
   clients (0x4c4880)
   @deep B3 draft, 203/204 instructions: only 2 code hunks left, the count loops' pointer chain (0x50f39a / 0x50f48d = lea ecx,[eax+eax*2] + lea edi,[ecx*4+base] + mov ecx,0x10; the original uses eax as the lea scratch and puts the counter init first) - allocation, not meaning. All 67 differing instructions were read against the original 0x50f340..0x50f56f: the meaning is equal; the meaning leads found are applied (the third cm_SendExSellObject argument is an int per ai.h - the original zero-extends the unsigned short goods id with and eax,0xffff - 28 -> 12 code hunks) and the rest is regs (edx/ecx for the objId argument, ebp vs ebx for cm_FieldObject in the second loop). spell.py: no hunk matches the menu; the counter-before-pointer spelling was tried, no change. Readability done: Q1 init_StockMarketStalls, Q2 ai_Unk7e41ec -> ai_PlayerSlots + 0x2ac / 0x2cc, Q4 through INIT_BLD.player (0x65) / INIT_OBJ.objId, Q6. */


void init_StockMarketStalls(void)
{
    INIT_BLD *b;
    INIT_OBJ *st;
    int owner, n, i;
    unsigned short *w, *w0;

    b = (INIT_BLD *)gm_BuildQuery(1, 0, INIT_KONTOR_BUILDING);
    while (b != 0) {
        owner = b->player;
        st = (INIT_OBJ *)gm_Query(b->objList, 2, 0, 0x1db, 6);
        if (st != 0) {
            n = 0;
            w0 = (unsigned short *)(ai_PlayerSlotsV[owner] + 0x2ac);
            w = w0;
            for (i = 0x10; i != 0; i--) {
                if (*w != 0)
                    n++;
                w++;
            }
            cm_BeginFieldBlock((char *)st, st->objId);
            cm_AddFieldDelta(1, 1, (int)st - cm_FieldObjectI + 0x1c, &n);
            n = 4;
            cm_AddFieldDelta(4, 1, (int)st - cm_FieldObjectI + 0xe, &n);
            cm_SendFieldDeltas();
            for (i = 0x10; i != 0; i--) {
                if (*w0 != 0) {
                    cm_SendExSellObject(st->objId, -1, *w0, 1, ai_OwnPlayer, 0);
                    cm_SendExSellObject(-1, st->objId, *w0, 1, ai_OwnPlayer, 0);
                }
                w0++;
            }
        }
        st = (INIT_OBJ *)gm_Query(b->objList, 2, 0, 0x1dc, 6);
        if (st != 0) {
            n = 0;
            w0 = (unsigned short *)(ai_PlayerSlotsV[owner] + 0x2cc);
            w = w0;
            for (i = 0x10; i != 0; i--) {
                if (*w != 0)
                    n++;
                w++;
            }
            cm_BeginFieldBlock((char *)st, st->objId);
            cm_AddFieldDelta(1, 1, (int)st - cm_FieldObjectI + 0x1c, &n);
            n = 4;
            cm_AddFieldDelta(4, 1, (int)st - cm_FieldObjectI + 0xe, &n);
            cm_SendFieldDeltas();
            for (i = 0x10; i != 0; i--) {
                if (*w0 != 0) {
                    cm_SendExSellObject(st->objId, -1, *w0, 1, ai_OwnPlayer, 0);
                    cm_SendExSellObject(-1, st->objId, *w0, 1, ai_OwnPlayer, 0);
                }
                w0++;
            }
        }
        b = (INIT_BLD *)gm_BuildNext();
    }
}
