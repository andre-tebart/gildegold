/* 0x4b0950, the AMTSTRAEGER_INVENTAR_PLUS text command (callback 0x601b50[12], name 0x601070[12]) "-xxxNAME":
   0x4c4880 from every titled character (byte 0x166 or 0x169) with the value of 0x4fe900(fmt + 4) */

int gm_FindObjectProto(char *s);                                                       /* 0x4fe900 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);           /* 0x4c4880 */

int ai_CommandOfficeInventoryPlus(int a, int b, char *fmt)
{
    char *s;
    int v, i;

    if (*fmt != '-')
        return 0;
    s = fmt + 1;
    if (strlen(s) < 4)
        return 0;
    s += 3;
    tolower((int)(s + 1));      /* sic: a pointer */
    v = gm_FindObjectProto(s);
    if (v == 0)
        return 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingCharacter((unsigned short)i) && (G12_B(i, 0x166) != 0 || G12_B(i, 0x169) != 0))
            cm_SendExSellObject(ai_Chars[i].netId, -1, v, 1, ai_OwnPlayer, 0);
    }
    return 1;
}
