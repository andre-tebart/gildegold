/* 0x53daf0, picks the active event window (0x6f262c): drops it when its window is closed, activates the first used slot whose window is open (ROUGH) */

/* the quick jump marker (0x6333e0): set when the active event is a type 0x11 record with bit 0x40 at +0xf0 or a type 0x87 record, outside the network games */
#define HE_JUMP(r) ((r) != 0 && ((*(char *)(r) == 0x11 && (*(unsigned char *)((r) + 0xf0) & 0x40) != 0) || *(char *)(r) == -0x79) && (U_I((unsigned)&ai_GameFlags) & 4) == 0)

void he_Unk53daf0(void)
{
    int *cur;
    int *p;
    char *r;
    int k;

    if (U_I((unsigned)&full_data_62571c) != -1) {
        cur = (int *)U_I((unsigned)&full_data_6f262c);
        if (cur != 0 && cur[1] != -1 && U_I(U_I((unsigned)&d2_Objs) + 0x28 + cur[1] * 0x2ec) == 0) {
            if (cur[2] != 0)
                d2_SetViewEnabled(cur[2], 0);
            r = (char *)*cur;
            if (HE_JUMP(r))
                U_I((unsigned)&tb_Paused) = 0;
            U_I((unsigned)&full_data_6f262c) = 0;
        }
        p = (int *)(unsigned)&full_data_6faa80;
        cur = (int *)U_I((unsigned)&full_data_6f262c);
        do {
            if (p[1] != -1 && cur != p && U_I(U_I((unsigned)&d2_Objs) + 0x28 + p[1] * 0x2ec) != 0) {
                if (cur != 0 && (k = cur[2]) != -1) {
                    if (k != 0) {
                        d2_SetViewEnabled(k, 0);
                        cur = (int *)U_I((unsigned)&full_data_6f262c);
                    }
                    if (cur[1] != -1) {
                        U_I(U_I((unsigned)&d2_Objs) + 0x28 + cur[1] * 0x2ec) = 0;
                        U_I(U_I((unsigned)&d2_Objs) + 0x2c + ((int *)U_I((unsigned)&full_data_6f262c))[1] * 0x2ec) = 0;
                        cur = (int *)U_I((unsigned)&full_data_6f262c);
                    }
                    r = (char *)*cur;
                    if (HE_JUMP(r))
                        U_I((unsigned)&tb_Paused) = 0;
                }
                U_I((unsigned)&full_data_6f262c) = (int)p;
                d2_SetViewEnabled(p[2], 1);
                d2_RaiseViewWindows(((int *)U_I((unsigned)&full_data_6f262c))[2]);
                r = (char *)*(int *)U_I((unsigned)&full_data_6f262c);
                cur = (int *)U_I((unsigned)&full_data_6f262c);
                if (HE_JUMP(r))
                    U_I((unsigned)&tb_Paused) = 1;
            }
            p += 3;
        } while ((int)p < (unsigned)((unsigned)full_data_6faa80 + 0xc0));
    }
}
