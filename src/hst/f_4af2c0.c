/* 0x4af2c0, text token: n percent of the count of the first goods of type 9 in the stock (object 0x115) of
   building 0x50b010(1), as a money text
   @deep behaviour: 31 code hunks; the reading is right (building 1 = class 1 via 0x50b010(1), the object 0x115 of its stock, the first goods of type 9, its count in unk0e, then n1 percent of it as money) and the money tail is the same unsolved gm_ConvertMoney call shape as ai_TokenMoney's @deep reason. Q2: gm_FirstBuilding (0x50b010, f3gm.c) has a name with evidence ('the first building of the class n', class 1 = the type 0xf) but its canonical definition returns unsigned char * while all its callers (am.c 5, cm.c, hst.c, this unit) use it as AIBLD *: the rename needs that return type unified first (type debt noted in f3gm.c's layout section), so it is a blocked shared-header/type decision; AIOBJ.unk0e (the count field) and AIOBJ.unk14 (the same list field gm_Query walks) have no evidence yet. Q1 name has evidence (callback 0x601b10[9] = 0x4af2c0, name 0x601730[9] STADTKASSE) */

int ai_TokenCityTreasury(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2, cnt;
    int v = 0;
    AIBLD *b;
    AIOBJ *o;

    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    for (o = gm_Query(o->unk14, 1, 5); o != 0; o = gm_QueryNext()) {
        if (ai_GoodsT[o->id].type == 9) {
            v = o->unk0e;
            break;
        }
    }
    cnt = 0x200;
    n2 = (int)buf;
    AI_ZERO((char *)n2, cnt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    n1 = atoi(fmt + n1);
    txt_FormatString(out, buf, gm_ConvertMoney((int)(n1 * 0.01f * v), ai_OwnPlayer));
    return 1;
}
