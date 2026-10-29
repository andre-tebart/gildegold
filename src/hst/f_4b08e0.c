/* 0x4b08e0, the STRAFE_KERKER text command (callback 0x601b50[11], name 0x601070[11]) "-n": sets the byte field
   0x1b1 of the character args[k] to n (message 0x4c3ba0) */

int ai_CommandPrison(G12ARG *args, int k, char *fmt)
{
    char v = 0;
    AICHAR *c;

    if (*fmt == '-') {
        v = (char)atoi(fmt + 1);
        if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0) {
            cm_BeginFieldBlock((void *)c, c->netId);
            cm_AddFieldValues(1, 1, 0x1b1, &v);
            cm_SendFieldDeltas();
            return 1;
        }
    }
    return 0;
}
