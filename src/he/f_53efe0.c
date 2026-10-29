/* 0x53efe0, posts the event 0x6d (once) */

/* the 0x6d / 0x6b / 0x7a events of the current player when none exists yet (one message of 0xf8 bytes) */
#define HE_ONCE(typ) \
    char ev[0xf8]; \
    if (he_FindEvent(1, 0, typ) == 0) { \
        memset(ev, 0, 0xf8); \
        HM_I(ev, 0x28) = U_I((unsigned)&ai_Time); \
        HM_B(ev, 4) = typ; \
        HM_I(ev, 0xc) = -1; \
        HM_B(ev, 0x36) = 2; \
        *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc); \
        HM_I(ev, 8) = U_I((unsigned)&ai_CharInts + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c); \
        HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour); \
        HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8)); \
        cm_SendLongMessage(ev); \
    }

void he_PostEvent6d(void)
{
    HE_ONCE(0x6d)
}
