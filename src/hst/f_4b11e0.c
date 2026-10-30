/* 0x4b11e0, text command "-PLUS_<kind>_n" / "-MINUS_<kind>_n" (kind WAFFEN / BUECHER / ENDPRODUKTE): scales
   the prices of the player's stock of that kind by 1 +- n% (0x4c56d0)
   @deep meaning read equal against the disassembly (all 14 hunks), only allocation / scheduling left plus a Q2 block. This session: the stock block stride (0x7d6a70 + player * 0x2a90, f3gm.c's GM_GOODS / f3init.c's INIT_STOCKBLOCK; was G12STOCK[][0x55], stride 0x2a80, wrong records for every ai_OwnPlayer > 0) and &e instead of e.unk08 to 0x4c56d0 (that callee memcpy's 0x80 bytes; the original's lea [esp+0x54] with 8 pushed bytes is e+0) are fixed: 332 -> 275 differing instructions, frame 0xbc now equal, every slot equal (sign 0x14, name 0x1c, kind 0x28, e 0x4c). What is left: the register permutation fmt/i/cursor = edx/ebx/ebp here vs ebp/edx/ebx in the original (the original keeps fmt in ebp as ai_CommandLaw does - ours loads it into edx before sub esp and spills i to [esp+0x10] inside the name loop, one extra store), the FPU kept-alive pattern in the three loop tails (original fst g; fmul e.demand, ours fstp g; fld e.demand; fmul g) and the 167-instruction prologue store order (the string copies are verified against .rdata: name MINUS/PLUS at 0x601bc4/0x601bbc, kind WAFFEN/BUECHER/ENDPRODUKTE at 0x601c40/0x601c38/0x601c2c, sign -1/1). Tried and reverted: a per-loop G12STOCK *s pointer local (336 asm, code 48 bytes smaller) - allocation, not meaning. Q2 block: ai_Unk4c56d0 (0x4c56d0 = message 0x3f, k and 0x80 bytes of p, cm.c; cm_SendGoodsRecord is the evidenced name) and ai_Unk4cd9d0 (0x4cd9d0, the record of ten with id, cm.c) - both need their own units' evidence before the rename. */

typedef struct G12STOCK {               /* 0x80 bytes, a player's goods record (f3init.c's INIT_STOCK, f3gm.c's GM_GOODS) */
    unsigned short item;                /* 0x00, the goods prototype (an ai_GoodsT index) */
    char pad02[2];
    int amount;                         /* 0x04, the amount the player holds */
    char pad08[0x18];                   /* 0x08 .. 0x1f */
    float demand;                       /* 0x20, the amount the market wants */
    float mean;                         /* 0x24 */
    char pad28[0x10];                   /* 0x28 .. 0x37 */
    float price;                        /* 0x38, the stored price */
    char pad3c[0x44];                   /* 0x3c .. 0x7f */
} G12STOCK;

typedef struct G12STOCKBLOCK {          /* the 0x55 goods records of one player; the next player's records start 0x2a90
                                           bytes on (f3init.c's INIT_STOCKBLOCK: four price-index ints precede each block) */
    G12STOCK rec[0x55];                 /* 0x00, at 0x7d6a70 + player * 0x2a90 (f3gm.c's GM_GOODS) */
    char pad2a80[0x10];                 /* 0x2a80 */
} G12STOCKBLOCK;

extern char ai_Unk7d6a60[];  /* 0x7d6a60, the five 0x2a90-byte stock blocks (f3gm.c's definition) */
#define init_StockBlocks ((G12STOCKBLOCK *)(ai_Unk7d6a60 + 0x10))  /* 0x7d6a70, the records of block 0; a
                                       view into the one 0xd4d0-byte object, not a separate COMMON symbol
                                       (a separate definition let the block writer/reader overwrite the
                                       globals the linker placed inside the stock area) */

typedef struct G12REC10 {               /* 0x5c bytes, one of the ten records 0x4cd9d0 searches (ai_Unk60c7c0) */
    int id;                             /* 0x00, the id 0x4cd9d0 matches */
    char pad04[0x58];
} G12REC10;
void ai_Unk4c56d0(void *p, unsigned char k);                                /* 0x4c56d0, defined elsewhere */

int ai_CommandDemand(int a, int b, char *fmt)
{
    G12STOCK e;
    float f, g;
    int i, j;

    if (*fmt != '-')
        return 0;
    {
    int sign[2] = { -1, 1 };
    char name[2][6] = { "MINUS", "PLUS" };
    char kind[3][12] = { "WAFFEN", "BUECHER", "ENDPRODUKTE" };

    fmt++;
    for (i = 0; i < 2; i++) {
        if (memcmp(fmt, name[i], strlen(name[i])) == 0)
            break;
    }
    if (i >= 2)
        return 0;
    fmt += strlen(name[i]);
    if (*fmt != '_')
        return 0;
    fmt++;
    for (j = 0; j < 3; j++) {
        if (memcmp(fmt, kind[j], strlen(kind[j])) == 0)
            break;
    }
    if (j >= 3)
        return 0;
    fmt += strlen(kind[j]);
    if (*fmt != '_')
        return 0;
    f = (float)(atoi(fmt + 1) * sign[i]) * 0.01;
    switch (j) {
    case 0:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0) {
                G12REC10 *r = (G12REC10 *)ai_Unk4cd9d0(init_StockBlocks[ai_OwnPlayer].rec[i].item);

                if (r != 0 && r->id != 0) {
                    g = f + 1.0f;
                    e = init_StockBlocks[ai_OwnPlayer].rec[i];
                    e.mean = g * e.demand;
                    e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                    ai_Unk4c56d0(&e, ai_OwnPlayer);
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0
                && ai_GoodsT[init_StockBlocks[ai_OwnPlayer].rec[i].item].type == 8) {
                g = f + 1.0f;
                e = init_StockBlocks[ai_OwnPlayer].rec[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                ai_Unk4c56d0(&e, ai_OwnPlayer);
            }
        }
        break;
    case 2:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0
                && ai_GoodsT[init_StockBlocks[ai_OwnPlayer].rec[i].item].type == 0x20) {
                g = f + 1.0f;
                e = init_StockBlocks[ai_OwnPlayer].rec[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                ai_Unk4c56d0(&e, ai_OwnPlayer);
            }
        }
        break;
    }
    }
    return 1;
}
