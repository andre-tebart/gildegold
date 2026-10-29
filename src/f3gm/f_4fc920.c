/* 0x4fc920, the daily step of all characters: the kind 0x12 ones get the byte 8 at +2 (0x4c3c00 (1, 1, 2)); every character of kind < 10 or 0x14 that is active runs 0x4fbe90 and, when the lifetime float (+0x20) - 1 equals the age and it has a model (+0x184), the call 0x4c5690 (id, "stallbursche_MANN" / "magd_FRAU" by the byte +9); then 0x4fd120 */

void unk_Unk4fc920(void)
{
    unsigned loc;
    char *c;
    int i;

    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (c[2] == 0x12) {
            loc = 8;
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(1, 1, 2, &loc);
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (((unsigned char)c[2] < 10 || c[2] == 0x14) && c[8] != 0) {
            unk_Unk4fbe90((unsigned short *)c);
            loc = *(unsigned short *)(c + 10);
            if (*(float *)(c + 0x20) - 1.0 == (float)loc && *(int *)(c + 0x184) != 0) {
                char *name;
                if (c[9] == 0)
                    name = "stallbursche_MANN";
                else
                    name = "magd_FRAU";
                ai_Unk4c5690(*(int *)(c + 4), name);
                ai_Unk6f1b98++;
            }
        }
    }
    unk_Unk4fd120();
    ai_Unk6f1b98 = 0;
}
