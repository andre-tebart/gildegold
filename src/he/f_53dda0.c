/* 0x53dda0, sends the message 0x11 (event text) to the player `pid` (-1 when it is no figure of kind 6 / 7): `text` (+ `text2` behind its NUL; flag 0x10 then) with the clock of the game, event record `ev` at +0x58 (ROUGH) */

#define HM_B(m, o) (*(unsigned char *)((m) + (o)))
#define HM_I(m, o) (*(int *)((m) + (o)))

short * gm_FindChar(int id);
int ai_Unk4c4bc0();
void err_Message(char *text);

int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2)
{
    char msg[0xf8];
    char buf[0x1f90];
    int rec, n, m;
    char *p;
    unsigned char b = (unsigned char)fl;

    rec = gm_FindChar(pid);
    if (rec == 0 || (*(char *)(rec + 2) != 6 && *(char *)(rec + 2) != 7))
        return -1;
    memset(msg, 0, 0xf8);
    HM_I(msg, 8) = pid;
    HM_I(msg, 0xc) = fl;
    HM_B(msg, 4) = 0x11;
    HM_B(msg, 0x36) = 9;
    HM_I(msg, 0x58) = ev;
    HM_B(msg, 0x9c) = b;
    strcpy(buf, text);
    n = strlen(text) + 1;
    if (text2 == 0 || *text2 == 0)
        return ai_Unk4c4bc0(msg, n, buf);
    memcpy(buf + n, text2, strlen(text2) + 1);
    b |= 0x10;
    HM_B(msg, 0xa0) = b;
    return ai_Unk4c4bc0(msg, strlen(text2) + n + 1, buf);
}
