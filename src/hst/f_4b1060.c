/* 0x4b1060, text command "-n": frees the holders of the offices 0x1e..0x21 (0x4e6680) and sends message 0x80
   (arg 4), today 8:00 */

typedef struct G12OBJ {                 /* 0x18 bytes, filled by 0x4e6ec0 (hmt.h: HMTOBJ) */
    int obj;                            /* 0x00 */
    int id;                             /* 0x04, a character id or -1 */
    char pad08[0x10];
} G12OBJ;

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out);                                         /* 0x4e6ec0, defined elsewhere */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);                /* 0x4e6680, defined elsewhere */

int ai_Unk4b1060(int a, int b, char *fmt)
{
    G12OBJ o;
    AIMSG msg;

    if (*fmt != '-')
        return 0;
    if (amt_GetFirstOffice(0x1e, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x1f, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x20, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x21, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    atoi(fmt + 1);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x80;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.flags = 2;
    msg.car = 4;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}
