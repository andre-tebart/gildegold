/* 0x4e2c70, runs the frame wait (0x5309a0, flag 0x8000) and 0x4284a0(a) until it says 0 */

int ai_Unk4e2c70(int a)
{
    if (ai_Unk6f0d8c == 0) {
        if (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0) {
            do {
                if (evt_FindById(a) == 0)
                    break;
            } while (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
