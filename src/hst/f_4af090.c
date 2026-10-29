/* 0x4af090, text token for the building of type 0x16 with the highest 0x509b10 rating */

int ai_TokenBestInn(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int best = -1, v;
    AIBLD *b, *pick = 0;

    if (mode == 2)
        return 0;
    if (mode == 1) {
        AI_4FEA50_4((int *)&pick, 0, 0, args[k].id);
        if (pick == 0)
            return 0;
    } else {
        for (b = gm_BuildQuery(1, 5, 0x16); b != 0; b = gm_BuildNext()) {
            v = gm_EquipmentValue((void *)b, b->level);
            if (v > best) {
                pick = b;
                best = v;
            }
        }
        if (pick == 0)
            return 0;
        if (mode == 0)
            args[k].id = pick->id;
    }
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    txt_FormatString(out, buf, pick);
    return 1;
}
