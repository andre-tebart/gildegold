/* 0x550ee0, the run handler of the ranking event (a Ghidra miss; ROUGH, the packet field offsets are read from the disassembly but not checked): state 1 re-posts the event (0x18) and resets the clock word to 7; state 0 asks the ranking (0x506180: up to 8 records of 0x24 bytes, the id first), sends a head packet (type words 1 / 3 / 0xcf: the five text lines 0x16b7 .. 0x16bb of 0x18 bytes, the own name of the player 0x6f1bc5 (+0x20 of the 0x7e3f40 record), the literals 0x626fa0 / 0x626fb0, the count) and one packet per ranked player (words 1 / 2 / 0xb9: id, the 0x5085b0 value, the name 0x20 bytes at +0x30, the texts 0x130 / 0x114 / 0x253 + the job bytes, a flag for the kind 6 / 7 characters) by 0x401e70, then posts the event 1 later and goes to state 1 once the minute word reaches 0x15 */

void gm_Unk506180();
int gm_ConvertMoney(int a, unsigned char k);
void udp_Send(char *buf, int len);

void he_Unk550ee0(int ev)
{
    int cnt = 8;
    unsigned char recs[0x120];
    unsigned char head[0x100];
    unsigned char pk[0x100];
    char tmp[0x40];
    int i, st = *(int *)(ev + 0x70), id, ch, v;
    unsigned char *r;
    char *p;

    if (st == 1) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 7;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x70) = 0;
        return;
    }
    if (st != 0)
        return;
    gm_Unk506180(&cnt, recs);
    *(unsigned short *)(head + 0) = 1;
    *(unsigned short *)(head + 2) = 3;
    *(unsigned short *)(head + 4) = 0xcf;
    for (i = 0; i < 5; i++) {
        txt_Format(tmp, "%s", 0x16b7 + i);
        strncpy((char *)head + 6 + i * 0x18, tmp, 0x18);
        head[0x1d + i * 0x18] = 0;
    }
    strcpy((char *)head + 0x7e, (char *)((unsigned)&ai_PlayerSlots + 0x20) + (unsigned)*(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4);
    memcpy(head + 0x9e, (char *)((unsigned)&ai_Unk625820 + 0x1790), 9);
    memcpy(head + 0xb6, (char *)((unsigned)&ai_Unk625820 + 0x1780), 0x11);
    head[0xce] = (unsigned char)cnt;
    udp_Send(head, *(unsigned short *)(head + 4));
    for (i = 0; i < cnt; i++) {
        r = recs + i * 0x24;
        id = *(unsigned short *)r;
        ch = (unsigned)&ai_Chars + id * 0x21c;
        *(unsigned short *)(pk + 0) = 1;
        *(unsigned short *)(pk + 2) = 2;
        *(unsigned short *)(pk + 4) = 0xb9;
        *(int *)(pk + 0xa) = *(int *)(ch + 4);
        v = gm_ConvertMoney(*(int *)(r + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer);
        *(int *)(pk + 0x12) = v;
        *(int *)(pk + 0xe) = *(int *)(r + 0x20);
        pk[0x13] = (*(unsigned char *)(ch + 2) == 6 || *(unsigned char *)(ch + 2) == 7) ? 1 : 2;
        *(int *)(pk + 0x1c) = *(int *)(ch + 0x80);
        strncpy((char *)pk + 0x21, (char *)(ch + 0x30), 0x18);
        p = tmp;
        txt_Format(p, "%s", 0x130 + (*(char *)(ch + 9) != 0 ? 0x5f : 0) + *(unsigned char *)(ch + 0x164));
        strncpy((char *)pk + 0x39, p, 0x20);
        txt_Format(p, "%s", 0x114 + (*(char *)(ch + 9) != 0 ? 0xa : 0) + *(unsigned char *)(ch + 0xd));
        strncpy((char *)pk + 0x59, p, 0x20);
        txt_Format(p, "%s", 0x253 + (*(char *)(ch + 9) != 0 ? 0x28 : 0) + *(unsigned char *)(ch + 0x166));
        strncpy((char *)pk + 0x79, p, 0x24);
        txt_Format(p, "%s", 0x253 + (*(char *)(ch + 9) != 0 ? 0x28 : 0) + *(unsigned char *)(ch + 0x169));
        strncpy((char *)pk + 0x9d, p, 0x24);
        udp_Send(pk, *(unsigned short *)(pk + 4));
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (*(unsigned short *)(ev + 0x56) >= 0x15)
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
}
