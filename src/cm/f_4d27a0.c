/* 0x4d27a0, builds the table ai_Unk9300a0[6][5][6] of floats (the values by (row, column) for the six kinds, stored one by one,
   then each row of 6 accumulated with the factor 5ece60: v[c] = v[c] * k + v[c - 1], and the repeats of the previous column
   cleared). Rough draft. */

void ai_BuildFloatTable()
{
    int i, j, k, b, e;
    float t;
    float *p;

    memset(ai_Unk9300a0, 0, 0x2d0);
    ai_Unk9300a0[0x0] = 10.0f;
    ai_Unk9300a0[0x1] = 10.0f;
    ai_Unk9300a0[0x4] = 30.0f;
    ai_Unk9300a0[0x5] = 50.0f;
    ai_Unk9300a0[0x6] = 40.0f;
    ai_Unk9300a0[0x7] = 20.0f;
    ai_Unk9300a0[0xa] = 20.0f;
    ai_Unk9300a0[0xb] = 20.0f;
    ai_Unk9300a0[0xc] = 80.0f;
    ai_Unk9300a0[0xd] = 20.0f;
    ai_Unk9300a0[0x10] = 0;
    ai_Unk9300a0[0x11] = 0;
    ai_Unk9300a0[0x12] = 70.0f;
    ai_Unk9300a0[0x13] = 30.0f;
    ai_Unk9300a0[0x16] = 0;
    ai_Unk9300a0[0x17] = 0;
    ai_Unk9300a0[0x18] = 50.0f;
    ai_Unk9300a0[0x19] = 50.0f;
    ai_Unk9300a0[0x1c] = 0;
    ai_Unk9300a0[0x1d] = 0;
    ai_Unk9300a0[0x1e] = 10.0f;
    ai_Unk9300a0[0x1f] = 10.0f;
    ai_Unk9300a0[0x22] = 30.0f;
    ai_Unk9300a0[0x23] = 50.0f;
    ai_Unk9300a0[0x24] = 80.0f;
    ai_Unk9300a0[0x25] = 20.0f;
    ai_Unk9300a0[0x28] = 0;
    ai_Unk9300a0[0x29] = 0;
    ai_Unk9300a0[0x2a] = 80.0f;
    ai_Unk9300a0[0x2b] = 20.0f;
    ai_Unk9300a0[0x2e] = 0;
    ai_Unk9300a0[0x2f] = 0;
    ai_Unk9300a0[0x30] = 60.0f;
    ai_Unk9300a0[0x31] = 40.0f;
    ai_Unk9300a0[0x34] = 0;
    ai_Unk9300a0[0x35] = 0;
    ai_Unk9300a0[0x36] = 50.0f;
    ai_Unk9300a0[0x37] = 50.0f;
    ai_Unk9300a0[0x3a] = 0;
    ai_Unk9300a0[0x3b] = 0;
    ai_Unk9300a0[0x3c] = 5.0f;
    ai_Unk9300a0[0x3e] = 5.0f;
    ai_Unk9300a0[0x40] = 20.0f;
    ai_Unk9300a0[0x41] = 70.0f;
    ai_Unk9300a0[0x42] = 50.0f;
    ai_Unk9300a0[0x44] = 10.0f;
    ai_Unk9300a0[0x46] = 20.0f;
    ai_Unk9300a0[0x47] = 20.0f;
    ai_Unk9300a0[0x48] = 80.0f;
    ai_Unk9300a0[0x4a] = 20.0f;
    ai_Unk9300a0[0x4c] = 0;
    ai_Unk9300a0[0x4d] = 0;
    ai_Unk9300a0[0x4e] = 70.0f;
    ai_Unk9300a0[0x50] = 30.0f;
    ai_Unk9300a0[0x52] = 0;
    ai_Unk9300a0[0x53] = 0;
    ai_Unk9300a0[0x54] = 50.0f;
    ai_Unk9300a0[0x56] = 50.0f;
    ai_Unk9300a0[0x58] = 0;
    ai_Unk9300a0[0x59] = 0;
    ai_Unk9300a0[0x5a] = 20.0f;
    ai_Unk9300a0[0x5c] = 80.0f;
    ai_Unk9300a0[0x5e] = 0;
    ai_Unk9300a0[0x5f] = 0;
    ai_Unk9300a0[0x60] = 30.0f;
    ai_Unk9300a0[0x62] = 70.0f;
    ai_Unk9300a0[0x64] = 0;
    ai_Unk9300a0[0x65] = 0;
    ai_Unk9300a0[0x66] = 40.0f;
    ai_Unk9300a0[0x68] = 60.0f;
    ai_Unk9300a0[0x6a] = 0;
    ai_Unk9300a0[0x6b] = 0;
    ai_Unk9300a0[0x6c] = 60.0f;
    ai_Unk9300a0[0x70] = 0;
    ai_Unk9300a0[0x71] = 0;
    ai_Unk9300a0[0x76] = 0;
    ai_Unk9300a0[0x77] = 0;
    ai_Unk9300a0[0x7a] = 0;
    ai_Unk9300a0[0x80] = 0;
    ai_Unk9300a0[0x86] = 0;
    ai_Unk9300a0[0x88] = 0;
    ai_Unk9300a0[0x89] = 0;
    ai_Unk9300a0[0x8c] = 0;
    ai_Unk9300a0[0x8e] = 0;
    ai_Unk9300a0[0x8f] = 0;
    ai_Unk9300a0[0x92] = 0;
    ai_Unk9300a0[0x94] = 0;
    ai_Unk9300a0[0x95] = 0;
    ai_Unk9300a0[0x98] = 0;
    ai_Unk9300a0[0x9e] = 0;
    ai_Unk9300a0[0xa4] = 0;
    ai_Unk9300a0[0xa6] = 0;
    ai_Unk9300a0[0xa7] = 0;
    ai_Unk9300a0[0xaa] = 0;
    ai_Unk9300a0[0xac] = 0;
    ai_Unk9300a0[0xad] = 0;
    ai_Unk9300a0[0xb0] = 0;
    ai_Unk9300a0[0xb2] = 0;
    ai_Unk9300a0[0xb3] = 0;
    ai_Unk9300a0[0x6e] = 40.0f;
    ai_Unk9300a0[0x72] = 70.0f;
    ai_Unk9300a0[0x74] = 30.0f;
    ai_Unk9300a0[0x78] = 20.0f;
    ai_Unk9300a0[0x7c] = 10.0f;
    ai_Unk9300a0[0x7d] = 70.0f;
    ai_Unk9300a0[0x7e] = 30.0f;
    ai_Unk9300a0[0x82] = 20.0f;
    ai_Unk9300a0[0x83] = 50.0f;
    ai_Unk9300a0[0x84] = 100.0f;
    ai_Unk9300a0[0x8a] = 100.0f;
    ai_Unk9300a0[0x90] = 100.0f;
    ai_Unk9300a0[0x96] = 20.0f;
    ai_Unk9300a0[0x9a] = 10.0f;
    ai_Unk9300a0[0x9b] = 70.0f;
    ai_Unk9300a0[0x9c] = 30.0f;
    ai_Unk9300a0[0xa0] = 20.0f;
    ai_Unk9300a0[0xa1] = 50.0f;
    ai_Unk9300a0[0xa2] = 100.0f;
    ai_Unk9300a0[0xa8] = 100.0f;
    ai_Unk9300a0[0xae] = 100.0f;
    for (p = ai_Unk9300a0; (int)p < ((unsigned)ai_Unk9300a0 + 0x2d0);) {
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 6; k++, p++) {
                t = (float)(*p * ai_Unk5ece60);
                *p = t;
                if (k != 0)
                    *p = t + p[-1];
            }
        }
    }
    for (b = 0; b < 30; b += 5) {
        for (i = 0; i < 5; i++) {
            for (j = 1; j < 6; j++) {
                for (k = 0; k < 6 - j; k++) {
                    e = (i + b) * 6 + j;
                    if (ai_Unk9300a0[e + k] == ai_Unk9300a0[e - 1])
                        ai_Unk9300a0[e + k] = 0;
                }
            }
        }
    }
}
