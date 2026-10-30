/* 0x47e840, whether this machine runs this worker (kinds 0 / 1): the building owner's net id is this
   machine's share of the players (multiplayer); a single player runs every worker */

int ai_IsLocalWorker(AICHAR *c)
{
    unsigned short id;
    AICHAR *o;

    if (c->kind != 0 && c->kind != 1)
        return 0;
    o = c;
    if (c->bld != 0 && (id = c->bld->owner) != 0xffff)
        o = &ai_CharsT[id];
    if (net_Conn.s != -1 && o->netId % ai_NumPlayers != ai_NetPlayer)
        return 0;
    return 1;
}
