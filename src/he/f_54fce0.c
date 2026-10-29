/* 0x54fce0, a Ghidra miss (disassembled): the run handler of the effect event: state -2 creates the effect (0x54fc00 for the type word +0xc2 == 0x177, 0x54fc70 for 0x178, +0xb0 = its handle; the scene switch 0x4478e0 around it) and goes to state 0 (or ends it), state 0 waits for the due time +0xb4 then clears the flag at +0x24 of the handle (0x4225e0) and goes to 1, state 1 waits for the end flag (+0x20 bit 1) of the handle, and a free of the event kills the effect (0x455760) (ROUGH) */

void he_Unk54fce0(int ev)
{
    int st, o = 0, h, sc, r;

    st = *(int *)(ev + 0x70);
    switch (st) {
    case -2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o != 0 && *(int *)(o + 0x61) != 0) {
            sc = U_I((unsigned)&d3_CurrentScene);
            d3if_SelectScene(0, 1);
            if (*(unsigned short *)(ev + 0xc2) == 0x177)
                *(int *)(ev + 0xb0) = he_Unk54fc00(o);
            else if (*(unsigned short *)(ev + 0xc2) == 0x178)
                *(int *)(ev + 0xb0) = he_Unk54fc70(o);
            d3if_SelectScene(sc, 1);
            *(int *)(ev + 0x70) = (*(int *)(ev + 0xb0) != 0) * 2 - 1;
            return;
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        r = gm_CompareTime((unsigned)&ai_Time, ev + 0xb4);
        h = *(int *)(ev + 0xb0);
        if (r == 1) {
            if (h != 0 && mem_IsValid(h) != 0)
                *(int *)(h + 0x24) = 0;
            *(int *)(ev + 0x70) = 2;
            return;
        }
        if (h == 0)
            *(int *)(ev + 0x70) = 0;
        return;
    case 1:
        h = *(int *)(ev + 0xb0);
        if (h != 0 && (*(unsigned char *)(h + 0x20) & 2) == 0)
            return;
        *(int *)(ev + 0x70) = -1;
        return;
    case 2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        h = *(int *)(ev + 0xb0);
        if (h != 0 && mem_IsValid(h) != 0) {
            sc = U_I((unsigned)&d3_CurrentScene);
            d3if_SelectScene(0, 1);
            par_Delete(*(int *)(ev + 0xb0));
            d3if_SelectScene(sc, 1);
        }
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
