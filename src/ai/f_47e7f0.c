/* 0x47e7f0, the character can act now (this machine runs it, and no flag 0x3000) */

int ai_CanAct(AICHAR *c)
{
    if ((net_Conn.s == -1 || c->kind == 6 || c->netId % ai_NumPlayers == ai_NetPlayer) && (c->flags1cc & 0x3000) == 0)
        return 1;
    return 0;
}
