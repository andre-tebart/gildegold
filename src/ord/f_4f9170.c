/* 0x4f9170, copies the 9 ints of the record 0x91b480 (the clear-able table of 0x4f82a0) to out */

/* the record 0x91b480 the messages 0x4c5e80 carry (0x4f9220 reads the fields) */
typedef struct ORDMSGREC {
    int enabled;                        /* 0x00, 0 nothing, 1 the event runs */
    int count;                          /* 0x04, the messages sent so far */
    int time;                           /* 0x08, the clock's first int (ai_TimeYear) */
    float base;                         /* 0x0c, the accumulator's base */
    float scale;                        /* 0x10, the accumulator's random scale */
    float value;                        /* 0x14, the accumulator */
    float limit;                        /* 0x18, the limit the accumulator is compared to */
    int phase;                          /* 0x1c, 0 idle, 1 / 2 / 3 running */
    int stage;                          /* 0x20, 0 .. 3, picked on every message */
} ORDMSGREC;

typedef struct ORDINITMSG {             /* the message 0x4c5e80 payload: the record and its tag at +0x24 */
    ORDMSGREC record;                   /* 0x00 */
    int tag;                            /* 0x24, 'init' / 'set ' */
} ORDINITMSG;

void ord_GetMessageRecord(int *out)
{
    memcpy(out, ord_MessageRecord, 0x24);
}
