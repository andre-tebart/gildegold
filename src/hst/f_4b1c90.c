/* 0x4b1c90, the INVENTAR_PLUS text command (callback 0x601b50[26], name 0x601070[26]) "-xxxNAME": 0x4c4880 from
   the character args[k] with the value of 0x4fe900(fmt + 4) (cf. ai_CommandOfficeInventoryPlus) */

int ai_CommandInventoryPlus(G12ARG *args, int k, char *fmt)
{
    AICHAR *c;
    char *s;
    int v;

    c = gm_FindChar(args[k].id);
    if (c != 0 && ai_IsLivingCharacter(c->id) && *fmt == '-') {
        s = fmt + 1;
        if (strlen(s) >= 4) {
            s += 3;
            tolower((int)(s + 1));      /* sic: a pointer */
            v = gm_FindObjectProto(s);
            if (v != 0) {
                cm_SendExSellObject(c->netId, -1, v, 1, ai_OwnPlayer, 0);
                return 1;
            }
        }
    }
    return 0;
}
