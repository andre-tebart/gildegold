/* 0x4b5950, sets ai_GameOver and runs 0x53bb20 through 0x52fb50 */

extern int ai_GameOver;
void sv_PopTo(int v);                                             /* 0x52fb50, defined elsewhere */
void main_GameLoop(int ctl);                                                         /* 0x53bb20, defined elsewhere */

void ai_QuitGame(int x)
{
    ai_GameOver = 1;
    sv_PopTo(main_GameLoop);
}
