/* 0x47dd30, the current AI work load: the number of players times the trade messages sent since the last
   reset, plus the open trade jobs (kind 0xb whose flags byte is still 0) */

int ai_WorkLoad(void)
{
    int n;
    AIJOB *j;

    n = 0;
    for (j = he_FindEvent(1, 0, 0xb); j; j = he_NextEvent()) {
        if (j->flags == 0)
            n++;
    }
    return ai_NumPlayers * ai_TradeMsgCount + n;
}
