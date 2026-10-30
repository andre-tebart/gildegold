/* 0x4b62a0, the mission complete sequence: single player 0x4b6110; network: 0x4b5ef0 for the player who did
   it (ai_MissionPartner), 0x4b60f0 for the others */

extern struct {                         /* net.h's NETCONN (the field used here) */
    int s;                              /* 0x00, the socket */
} net_Conn;                             /* 0x1460ae0 */

void ai_MissionCompleteSequence(int id)
{
    if (net_Conn.s != -1) {
        if (ai_CharsT[ai_PlayerId].netId == ai_MissionPartner)
            ai_MissionCompleteByMode(id);
        else
            ai_MissionResultByMode(id);
    } else
        ai_MissionCompleteSingle(id);
}
