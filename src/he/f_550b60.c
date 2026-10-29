/* 0x550b60 (a Ghidra function with 0x5509f0 .. glued before it), the run handler of the "Sensenmann" event: states -2 / -1 free the object (0x447190), the sound (0x4e3640, flag 0x6f2634) and the record; state 0 flies the object (0x550370: 1 = on, 2 = arrived: the next target building by +0xc0 steps, 0x550850), state 2 restarts the flight (0x5506c0) (ROUGH) */

void he_Unk550b60(int ev)
{
    char text[0x200];
    int st = *(int *)(ev + 0x70);
    int r, c0, n, idx, esi;

    if (st == -2 || st == -1) {
        if (*(int *)(ev + 0xc8) != 0)
            d3if_Delete((struct D3OBJECT *)*(int *)(ev + 0xc8));
        if (U_I((unsigned)&full_data_6f2634) != 0)
            ai_Unk4e3640();
        U_I((unsigned)&full_data_6f2634) = 0;
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0xc8) == 0 && he_Unk550070(ev) == 0) {
        he_Unk53e1f0((int *)ev);
        if (U_I((unsigned)&full_data_6f2634) != 0)
            ai_Unk4e3640();
        U_I((unsigned)&full_data_6f2634) = 0;
        return;
    }
    if (U_I((unsigned)&full_data_6f2634) == 0) {
        ai_Unk4e3620((void *)(unsigned)"cd2\\DerSeuchenzug.mp3");
        if (U_I((unsigned)&snd_TrackHandles) != 0)
            U_I((unsigned)&full_data_6f2634) = 1;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (st == 0) {
        r = he_Unk550370(ev);
        if (r == 1) {
            *(int *)(ev + 0x70) = 0;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            return;
        }
        if (r == 2) {
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb4);
            c0 = *(int *)(ev + 0xc0);
            if (c0 > 0) {
                n = 0x100;
                idx = *(int *)(ev + 0xb8);
                do {
                    unsigned lo = 0, hi = 0;
                    int e, k;

                    esi = U_I((unsigned)&ai_Blds) + idx * 0xa9;
                    e = U_I((unsigned)&ai_BTypes) + *(unsigned char *)esi * 0x24d;
                    k = *(unsigned char *)e;
                    if (k > 0x1f)
                        hi = 1u << (k - 0x20);
                    else
                        lo = 1u << k;
                    if (*(unsigned short *)(esi + 0x27) != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
                        break;
                    idx = (idx + *(int *)(ev + 0xbc)) & 0xff;
                    esi = 0;
                    n--;
                } while (n != 0);
                *(int *)(ev + 0xb8) = (*(int *)(ev + 0xbc) + idx) & 0xff;
                if (esi == 0) {
                    *(int *)(ev + 0x70) = -1;
                    return;
                }
                txt_Format(text, (char *)((unsigned)&ai_Unk625820 + 0x1760), esi);
                *(int *)(ev + 0xb4) = *(int *)(esi + 1);
                *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) - 1;
                if (he_Unk550850(ev) != 0) {
                    gm_AddTime(ev + 0x52, 0, 0xf, 0);
                    *(int *)(ev + 0x70) = 2;
                } else {
                    gm_AddTime(ev + 0x52, 0, 2, 0);
                    *(int *)(ev + 0x70) = -1;
                }
                return;
            }
            if (c0 == 0) {
                *(int *)(ev + 0xb4) = *(int *)(ev + 0xac);
                if (he_Unk550850(ev) != 0) {
                    gm_AddTime(ev + 0x52, 0, 0xf, 0);
                    *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) - 1;
                    *(int *)(ev + 0x70) = 2;
                    return;
                }
                gm_AddTime(ev + 0x52, 0, 2, 0);
            }
            *(int *)(ev + 0x70) = -1;
            return;
        }
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x70) = -1;
        return;
    }
    if (st == 2) {
        gm_AddTime(ev + 0x52, 0, 2, 0);
        if (he_Unk5506c0(ev) != 0) {
            *(int *)(ev + 0x70) = 0;
        } else {
            ai_Unk5a92b0();
            *(int *)(ev + 0x70) = -1;
        }
    }
}
