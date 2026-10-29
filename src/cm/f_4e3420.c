/* 0x4e3420, runs the scene for a ticks (frame wait with the flag 0x8000): the end sets the stop flag 0x6f22a0 */

int ai_Unk4e3420(int a)
{
    int end;

    end = ai_Unk6f0d80;
    if (ai_Unk6f0d8c == 0) {
        if (sv_RunFrame(ai_Unk4e3420, ai_Unk60d360 | 0x8000) != 0) {
            end += a;
            do {
                if (ai_Unk6f0d80 >= end)
                    sv_PopRequest = 1;
                ai_Unk4cee20();
                ai_Unk4e37a0(ai_Unk6f0d90);
            } while (sv_RunFrame(ai_Unk4e3420, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
