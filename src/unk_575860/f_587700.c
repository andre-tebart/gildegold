/* 0x587700, the graveyard grave-detail message (269 B): rec / ch the character / sel the picked line (1 / 2 / 4).
   The line 0x413a (the grave's word) through 0x5176d0 and 0x521150 (the line, 1) == 0 -> 0; 0x4ffe90
   (rec + 0x5d, 1, 0, 0x211); the message (type 0x8f at +4, 1 at +0x36, the player's character word at +8, rec + 1
   at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the 0x4ffe90 answer's +2 (or -1)
   at +0x60, the character +4 at +0x58, sel at +0x5c) through 0x4c4b70; 1 */

int ai_Unk587700(char *rec, char *ch, int sel)
{
    char msg[0xf8];
    char buf[0x400];
    int r;

    txt_Format(buf, (char *)0x413a, *(unsigned short *)rec);
    if (sv_MessageBox(buf, 1) == 0)
        return 0;
    r = gm_Query(rec + 0x5d, 1, 0, 0x211);
    memset(msg, 0, 0xf8);
    *(unsigned char *)(msg + 0x04) = 0x8f;
    *(unsigned char *)(msg + 0x36) = 1;
    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    *(int *)(msg + 0x0c) = U_I(rec + 1);
    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    if (r != 0)
        *(int *)(msg + 0x60) = *(int *)(r + 2);
    else
        *(int *)(msg + 0x60) = -1;
    *(int *)(msg + 0x58) = *(int *)(ch + 4);
    *(int *)(msg + 0x5c) = sel;
    cm_SendLongMessage(msg);
    return 1;
}
