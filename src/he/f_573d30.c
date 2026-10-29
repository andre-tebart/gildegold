/* 0x573d30, the default approach of the event 0x9d: posts a message 0x9f (0x4c4b70) from per +4 with the clock, -1 at +0xc;
   then the name (0x629038 / 0x62901c) and the texts 0x41e3 / 0x41dd (else 0x41dc) and the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573d30(char *pl, char *per)
{
    char msg[0xf8];
    char name[0x19];
    char t1[0x400];
    char t2[0x400];

    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x9f;
    HM_I(msg, 8) = HM_I(per, 4);
    HM_I(msg, 0xc) = -1;
    HM_I(msg, 0x28) = U_I((unsigned)&ai_Time);
    HM_I(msg, 0x2c) = U_I((unsigned)&ai_TimeHour);
    HM_I(msg, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    HM_B(msg, 0x36) = 1;
    cm_SendLongMessage(msg);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e3);
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
