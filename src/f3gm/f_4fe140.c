/* 0x4fe140, adds hours / minutes / seconds (any sign for the hours) to the clock c, carrying into the days (the minutes and seconds only upward) */

#pragma pack(push, 1)
typedef struct GCLOCK {                 /* the 14-byte game clock (0x770ae0) */
    int day;
    unsigned short hour;
    int min;
    int sec;
} GCLOCK;
#pragma pack(pop)

int ai_Unk875360[32];  /* 0x875360 */
int ai_Unk919f80[732];  /* 0x919f80 */
char * ai_Goods;  /* 0x91aaf0, the object prototypes ( */
unsigned ai_Unk91ac40[136];  /* 0x91ac40 */
char * ai_Objects;  /* 0x7e3f34, the object instances ( */
/* ai_Unk7e3ff8: view of ai_PlayerSlots (tools/data_views.py) */
int ai_Unk6178c8[] = { 468, 0, 1084227584, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x6178c8 */
extern char ai_Unk61aa38[], ai_Unk61aa20[], ai_Unk61aa00[], ai_Unk61a9ec[];  /* 0x61aa38, 0x61aa20, 0x61aa00, 0x61a9ec */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void util_Randomize();                     /* 0x42ec80 */

void gm_AddTime(GCLOCK *c, int hours, int mins, int secs)
{
    unsigned h;
    int t;
    int d;

    secs = c->sec + secs;
    t = c->min + secs / 0x3c;
    c->sec = secs % 0x3c;
    mins = t + mins;
    c->min = t;
    c->hour = (short)c->hour + (short)(mins / 0x3c);
    c->min = mins % 0x3c;
    h = c->hour + hours;
    if (0x17 < (int)h) {
        d = h / 0x18;
        h = h % 0x18;
        c->day = c->day + d;
    }
    if ((int)h < 0) {
        t = (-h - 1) / 0x18 + 1;
        c->day = c->day - t;
        c->hour = (short)h + (short)t * 0x18;
        return;
    }
    c->hour = (short)h;
}
