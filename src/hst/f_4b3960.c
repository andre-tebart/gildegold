/* 0x4b3960, the yearly history: tells the others (message 5 with the tick count) in a network game, waits
   while ai_Unkf83650, then shows the history dialog of hst_TextsOn (1: texts, 2: texts and commands) */

extern int cm_NetFlags;      /* 0x6f2594, game flags (8: network game) */
extern int ai_Unkf83650;
void cm_SendExSysMessage(unsigned char id, void *msg);                                            /* 0x4c4db0, defined elsewhere */
void sv_PumpNet(void);                                                         /* 0x52fe00, defined elsewhere */
__declspec(dllimport) unsigned long __stdcall GetTickCount(void);

void ai_Unk4b3960(int cmd)
{
    int msg[0x1f];

    if ((cm_NetFlagsB & 8) || net_Conn.s == -1) {
        msg[0] = GetTickCount();
        cm_SendExSysMessage(5, msg);
    }
    while (ai_Unkf83650 != 0) {
        if (net_Conn.closed != 0)
            return;
        sv_PumpNet();
    }
    if (net_Conn.closed == 0 && hst_TextsOn != 0) {
        if (hst_TextsOn == 1)
            ai_Unk4b3350();
        else if (hst_TextsOn == 2)
            ai_Unk4b36d0(cmd);
    }
}
