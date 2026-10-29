/* 0x52adb0, resets the selection (0x75ec5c / 0x75ec60, 0x6f2304) and clears the "selected" bit 2 of the group members (class 0x1d records) of the object under the cursor */

unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
void ch_SetVisible();                     /* 0x402480 */

void sv_Unk52adb0(void)
{
    int o;
    unsigned short *p;

    SV_I((unsigned)&ai_Unk75ec5c) = 0;
    SV_I((unsigned)&ai_Unk75ec60) = 0;
    SV_I((unsigned)&full_data_76631c) = 0;
    SV_I((unsigned)&ai_Unk6f2304) = 0;
    o = SV_I((unsigned)&ai_FallbackBuilding);
    if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        if (o == 0) {
            o = gm_BuildQuery(1, 0, 0x44);
            if (o == 0)
                return;
        }
    } else if (o == 0) {
        SV_I((unsigned)&ai_Unk6f2304) = 0;
        SV_I((unsigned)&ai_Unk75ec5c) = 0;
        SV_I((unsigned)&ai_Unk75ec60) = 0;
        SV_I((unsigned)&full_data_76631c) = 0;
        return;
    }
    SV_B((unsigned)&full_data_6f2338) = 0;
    p = (unsigned short *)((char *(*)())gm_Query)(*(int *)(o + 0x5d), 1, 5);
    while (p != 0) {
        if (ai_Goods[*p * 0x41] == 0x1d)
            *(unsigned *)(p + 0x10) &= 0xfffffffd;
        p = (unsigned short *)gm_QueryNext();
    }
}
