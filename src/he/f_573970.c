/* 0x573970, the approach 2 of the event 0x9d: searches the characters (0x770b80, 0x300) of the kind 0 without the byte +0x164
   whose building (+0x16c) word +0x27 is pl's word; when one is found a message 0x33 (0x4c4b70, the clock, the building +1 at
   +0xc, the character +4 at +0x58) is posted for pl +4, the name (0x629038 when pl is per, else 0x62901c, 0x19 bytes) and the
   texts 0x41e0 / 0x41dd (else 0x41dc) are built and sent as the message 0x5fb (0x53dda0) to pl +4 */

void he_Unk573970(char *pl, char *per)
{
    char msg[0xf8];
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    char *ch;
    int i;

    ch = 0;
    for (i = 0; i < 0x300; i++) {
        char *c = (char *)((unsigned)&ai_Chars + i * 0x21c);
        if (HM_B(c, 2) == 0 && HM_I(c, 0x16c) != 0 && HM_B(c, 0x164) == 0
            && *(unsigned short *)(HM_I(c, 0x16c) + 0x27) == *(unsigned short *)pl) {
            ch = c;
            break;
        }
    }
    if (ch == 0)
        return;
    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x33;
    HM_I(msg, 8) = HM_I(pl, 4);
    HM_I(msg, 0xc) = HM_I(HM_I(ch, 0x16c), 1);
    *(HETIME *)(msg + 0x28) = ai_Time;
    HM_B(msg, 0x36) = 1;
    HM_I(msg, 0x58) = HM_I(ch, 4);
    cm_SendLongMessage(msg);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e0, *(unsigned short *)ch, HM_I(ch, 0x16c));
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(pl, 4), -1, t2, 1, 0x5fb, name);
}
