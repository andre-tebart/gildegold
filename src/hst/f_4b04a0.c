/* 0x4b04a0, the ANSEHEN_BEI_AMTSTRAEGERN text command (callback 0x601b50[8], name 0x601070[8])
   "-PLUS_n" / "-MINUS_n": changes the relation (0x4c4b00) of every character with a title (byte 0x166 or 0x169)
   to the character args[k] by 2n */

int ai_CommandOfficeStanding(G12ARG *args, int k, char *fmt)
{
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0 && *fmt == '-') {
        int sign[2] = { -1, 1 };
        char name[2][6] = { "MINUS", "PLUS" };
        int i, n;

        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i < 2) {
            fmt += strlen(name[i]);
            if (*fmt == '_') {
                n = atoi(fmt + 1) * sign[i] * 2;
                for (i = 0; i < 0x300; i++) {
                    if (ai_IsLivingCharacter((unsigned short)i) && (G12_B(i, 0x166) != 0 || G12_B(i, 0x169) != 0))
                        cm_SendRelations(c->netId, ai_CharsT[i].netId, n, 1);
                }
                return 1;
            }
        }
    }
    return 0;
}
