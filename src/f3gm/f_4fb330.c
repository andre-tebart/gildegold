/* 0x4fb330, the record of the table kind (0: 0x612bd8, 1: 0x614518) with the id: prepared by 0x4fb620 (the head numbers); the first free record when the id is not found (0x5f records of 0x44 bytes); 0 for another kind */

/* the head tables (records of 0x44 bytes: +0 the id, +4 the name, +0x34 four head numbers): 0x6118a8 / 0x6118f0 (the two default entries), 0x611938 (0x23 records, kind 0), 0x612288, 0x612bd8 (0x5f records, kind 0) / 0x614518 (0x5f, kind 1), 0x617398 (3 records), the history 0x91ac40 (8 records) */
extern unsigned ai_Unk6118a8[], ai_Unk6118f0[], ai_Unk611938[], ai_Unk612288[], ai_Unk612bd8[], ai_Unk614518[], ai_Unk617398[], ai_Unk91ac40[];  /* 0x6118a8, 0x6118f0, 0x611938, 0x612288, 0x612bd8, 0x614518, 0x617398, 0x91ac40 */
int d3_CurrentScene = 0;  /* 0x633444 */
char ai_Unk6f0454[1] = {0};  /* 0x6f0454 .. */
int ai_Unk6f1b90 = 0;  /* 0x6f1b90, the number of icon names */
char * ai_Unk6f1b94 = 0;  /* 0x6f1b94, the icon names ( */
int ai_Unk6f1b98 = 0;  /* 0x6f1b98, the number of queued messages ( */
short * ai_Unk6f1bac = 0;  /* 0x6f1bac */
int ai_Unk6f1bb0 = 0;  /* 0x6f1bb0 */
void unk_Unk4fb620(unsigned *rec);
int ch_CurrentGroup = -1;  /* 0x5f1294 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */

unsigned *unk_Unk4fb330(int kind, int id)
{
    unsigned *t;
    int i;

    i = 0;
    if (kind == 0)
        t = ai_Unk612bd8;
    else if (kind == 1)
        t = ai_Unk614518;
    else
        return 0;
    do {
        if (t[i * 0x11] == 0)
            break;
        if (t[i * 0x11] == (unsigned)id) {
            unk_Unk4fb620(t + i * 0x11);
            return t + i * 0x11;
        }
        i++;
    } while (i < 0x5f);
    return t + i * 0x11;
}
