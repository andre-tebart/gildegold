/* 0x4b05e0, the KILL_PLAYER text command (callback 0x601b50[9], name 0x601070[9]): lowers the health
   (0x4fc820) of the character args[k] below zero (gm_AddHealth with -(x + 1)) */

int ai_CommandKillPlayer(G12ARG *args, int k)
{
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0) {
        gm_AddHealth(c->id, (int)-(gm_CharacterHealth((short *)c) + 1.0f));
        return 1;
    }
    return 0;
}
