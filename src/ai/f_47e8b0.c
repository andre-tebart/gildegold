/* 0x47e8b0, whether this machine runs this character's job: kinds 1 / 2 with a job in an owned
   building whose owner is not kind 7, and (multiplayer) the owner's net id is this machine's share */

int ai_IsLocalJob(AICHAR *c)
{
    unsigned short id;

    if (c->kind != 1 && c->kind != 2)
        return 0;
    if (c->jobId == 0)
        return 0;
    if (c->bld == 0)
        return 0;
    id = c->bld->owner;
    if (id == 0xffff)
        return 0;
    if (ai_Chars[id].kind == 7)
        return 0;
    if (net_Conn.s != -1 && ai_Chars[id].kind != 6 && ai_Chars[id].netId % ai_NumPlayers != ai_NetPlayer)
        return 0;
    return 1;
}
