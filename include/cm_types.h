/* The message records of cm's rings ai_Unka69140 / ai_Unk9367e0 (G17MSG) and ai_Unkf31160 (G17STAT), for the
   units that declare them without the rest of cm.h: types only. cm.h includes it. */
#ifndef CM_TYPES_H
#define CM_TYPES_H

#pragma pack(push, 1)
typedef struct G17MSG {                 /* 0x99 bytes, a game message (its head is net.h's NETPKT) */
    unsigned char type;                 /* 0x00 */
    unsigned short size;                /* 0x01, ai_Unk4c3180 */
    unsigned char unk03;                /* 0x03 */
    int idx;                            /* 0x04, its slot in the send ring */
    unsigned int seq;                   /* 0x08 */
    unsigned int next7;                 /* 0x0c, seq of the next part of a split message (type 7), 0: none */
    unsigned char data[0x81];           /* 0x10 */
    struct G17MSG *prev;                /* 0x91 */
    struct G17MSG *next;                /* 0x95 */
} G17MSG;

typedef struct G17STAT {                /* 10 bytes, ai_Unkf31160[]: the state of a sent message */
    unsigned char state;                /* 0x00, 0 sent, 1 free, 2 failed */
    unsigned char unk01;
    int idx;                            /* 0x02 */
    int value;                          /* 0x06 */
} G17STAT;
#pragma pack(pop)

#endif
