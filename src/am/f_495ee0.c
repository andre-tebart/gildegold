/* 0x495ee0, sends each kind-3 character random flags (field 0x1e8) */

/* ---- 0x495ee0..0x497490 ---- */

void ai_Unk495ee0(void)
{
    AIPER *c;
    unsigned int f;
    int i;

    f = 1;
    for (i = 0; i < 768; i++) {
        c = (AIPER *)&ai_CharsT[i];
        if (c->kind == 3) {
            f |= util_RandomU(2) ? 2 : 0;
            f |= util_RandomU(3) ? 4 : 0;
            f |= util_RandomU(3) ? 8 : 0;
            f |= util_RandomU(2) ? 0x10 : 0;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(4, 1, 0x1e8, &f);
            cm_SendFieldDeltas();
        }
    }
}
