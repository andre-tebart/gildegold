/* 0x47dd90, a master that this machine runs (multiplayer: ids are shared out by modulo) */

int ai_IsLocalMaster(AICHAR *c)
{
    if (!ai_WorksForMaster(c))
        return 0;
    if (ai_NumPlayers) {
        if (net_Conn.s != -1) {
            if (c->netId % ai_NumPlayers != ai_NetPlayer)
                return 0;
        } else if (c->netId % ai_NumPlayers != ai_CurrentPlayer)
            return 0;
    }
    return 1;
}
