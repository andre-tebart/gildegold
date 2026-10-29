/* 0x575430, the good outcome of the event 0xa4 (ROUGH): a record of the kind 0x5a9f10 (3, 6) is looked up (0x4a5760 (per, 1,
   ..)); with a record of the level +0xe 3 or more the figure's +0xac grows (0x5089c0 / 0x5de518) and the payment 0x4c4830
   (per +4, -1, 0x6f1bc5, the amount) runs; else 0x4c4880 (per +4, -1, 1, 0x6f1bc5, 0, the word) */

void he_Unk575430(char *per)
{
    int w[4];
    int out[4];
    char *rec;

    memset(w, 0, sizeof w);
    w[0] = ai_Unk5a9f10(3, 6) & 0xffff;
    memset(out, 0, sizeof out);
    ai_Unk4a5760(per, 1, w, out);
    rec = (char *)out[0];
    if (rec == 0) {
        cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, w[0]);
        return;
    }
    if (HM_I(rec, 0xe) >= 3) {
        gm_GoodsPrice(w[0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
        HM_I(per, 0xac) = HM_I(per, 0xac) + ai_Unk5de518();
        cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(per, 0xac));
        return;
    }
    cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, w[0]);
}
