/* 0x4e2d40, 0x4e2c70 with the frame wait flags 0x79706 */

int ai_Unk4e2d40(int a)
{
    if (ai_Unk6f0d8c == 0) {
        if (sv_RunFrame(0, 0x79706) != 0) {
            do {
                if (evt_FindById(a) == 0)
                    break;
            } while (sv_RunFrame(0, 0x79706) != 0);
        }
    }
}
