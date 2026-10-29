/* 0x4fa9b0, runs 0x4fa700 (i, 1, out) for all 0x300 characters, flushing the messages every 0x40 */

void unk_Unk4fa9b0(void)
{
    unsigned char out[16];
    int i;

    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        gm_CharacterWages(i, 1, out);
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
    }
    ai_Unk6f1b98 = 0;
}
