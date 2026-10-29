/* 0x4bb720, posts 0..n random messages for character id (AIMSG type 0x42 with the message type in n); returns the
   number; reseeds ai_Unk6f0730 with timeGetTime() */

int sv_ReturnZero(void);                                                          /* 0x52a040, defined elsewhere */

G14CB ai_Unk602570[0x45] = {                                                     /* 0x602570 */
    ai_MsgRewardPct1to3, ai_MsgRewardPct2to4, ai_MsgOfficeApplicantPayment, ai_MsgPayPct1to3,
    ai_MsgPayPct1to4, ai_MsgRewardPct2to5, ai_MsgRewardPct1to3Type68, ai_MsgPayForBuilding,
    ai_MsgRewardPct1to3Type68, ai_MsgPayPct2to4, ai_MsgPayPct125, ai_MsgPayPctOver120,
    ai_Unk4b6fd0, ai_Unk4b7100, ai_Unk4b7200, ai_Unk4b7310,
    ai_Unk4b7430, ai_Unk4b7550, ai_Unk4b76b0, ai_Unk4b76b0,
    ai_Unk4b77b0, ai_Unk4b78c0, ai_Unk4b79c0, ai_Unk4b7ac0,
    ai_Unk4b7bc0, ai_Unk4b7d80, ai_Unk4b7f40, ai_Unk4b8060,
    ai_Unk4b81a0, ai_Unk4b82c0, ai_Unk4b84b0, ai_Unk4b86c0,
    ai_Unk4b8870, ai_Unk4b8950, ai_Unk4b8a10, ai_MsgJobTime,
    ai_Unk4b8f70, ai_Unk4b9180, ai_Unk4b9420, ai_Unk4b9630,
    ai_Unk4b9810, ai_Unk4b9820, ai_Unk4b9ab0, ai_MsgWorkerStop,
    ai_Unk4b9f30, sv_ReturnZero, ai_Unk4ba0c0, ai_Unk4ba2d0,
    ai_Unk4ba470, ai_Unk4ba630, ai_Unk4ba970, ai_Unk4baa80,
    ai_Unk4baba0, ai_Unk4b9810, ai_Unk4b9810, ai_Unk4b9810,
    ai_MsgLoseGood, ai_Unk4bae20, ai_Unk4baf30, ai_Unk4bb030,
    ai_Unk4bb150, ai_Unk4bb250, ai_Unk4bb330, ai_Unk4bb430,
    ai_Unk4bb4f0, ai_Unk4bb590, ai_Unk4bb630, ai_Unk4bb630,
    ai_Unk4bb630
};
unsigned int ai_Unk6f0730 = 0;  /* 0x6f0730, random seed of the messages */

unsigned int ai_Unk4bb7d0(unsigned int n);

__declspec(dllimport) unsigned long __stdcall timeGetTime(void);

int ai_Unk4bb720(int n, int id)
{
    int k, i;
    AIMSG msg;

    k = ai_Unk4bb7d0(n + 1);
    ai_Unk6f0730 = timeGetTime();
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x42;
    msg.to = id;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 1;
    for (i = k; i > 0; i--) {
        *(unsigned short *)&msg.n = ai_Unk4bb7d0(0x45);
        msg.car = id;
        cm_SendLongMessage(&msg);
    }
    return k;
}
