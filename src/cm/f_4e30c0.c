/* 0x4e30c0, leaves the scene (depth 0x6f0d9c): waits for the objects 0x60d36c / 0x5f8438 (0x4284a0, bit 1 of +0x124), clears the scene objects (0x4e2dc0 /
   0x4e2e00), the table 0x5fb5b0 +0xb0 (0x45ef60), 0x433b10 (0xfc2dec), 0x6310e8 / e4 and 0x4473f0. ROUGH */

int ai_Unk4e30c0(void)
{
    char *t, *o;
    int id;

    if (ai_Unk6f0d8c != 0 || ai_Unk6f0d9c <= 0)
        return ai_Unk6f0d8c;
    if (ai_Unk60d36c != -1) {
        o = (char *)evt_FindById(ai_Unk60d36c);
        if (o != 0)
            evt_Stop((EVTSCRIPT *)o);
    }
    t = (char *)ai_Unk5fb5b0;
    if (t != 0 && *(int *)(t + 0x3c8) != 0 && *(int *)(*(char **)(t + 0x3c8) + 0x294) != 0)
        (*(int (**)(char *, int, char *, int))(*(char **)(t + 0x3c8) + 0x294))(t, 5, *(char **)(t + 0x3c8) + 0x298, 0);
    id = ai_Unk5f8438;
    if (id != -1) {
        o = (char *)evt_FindById(id);
        while (o != 0 && (*(unsigned char *)(o + 0x124) & 1)) {
            sv_RunFrame(0, ai_Unk60d360 | 0x80);
            o = (char *)evt_FindById(ai_Unk5f8438);
        }
    }
    ai_Unk4e2dc0();
    ai_Unk4e2e00();
    id = ai_Unk60d36c;
    if (id != -1) {
        o = (char *)evt_FindById(id);
        while (o != 0 && (*(unsigned char *)(o + 0x124) & 1)) {
            sv_RunFrame(0, ai_Unk60d360 | 0x80);
            o = (char *)evt_FindById(ai_Unk60d36c);
        }
    }
    t = (char *)ai_Unk5fb5b0;
    if (*(int *)(t + 0xb0) != 0) {
        d3sm_Destroy(*(int *)(t + 0xb0));
        *(int *)((char *)ai_Unk5fb5b0 + 0xb0) = 0;
    }
    d3anim_FreePath(ai_Unkfc2dec);
    d3t_CamMoveI = 0;
    ai_Unk6310e4 = 0;
    d3if_Reset();
    ai_Unk6f0d9c--;
}
