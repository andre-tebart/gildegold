/* 0x5a6b70, a map position helper (mm): the centre (float)0xfd7c50 + 0x20 * 0.5f, the x / y of the record
   (+8 / +0xc) around it scaled by 5.33f, the 51.0f edge correction over half the map size 0x6f4ee8 / 0x6f4ee4,
   written to +0x10 / +0x14 as param_2 / param_3 plus half the size plus the correction */

void ai_Unk5a6b70(int param_1, int param_2, int param_3)
{
    float half;
    float x;
    float y;
    float cx;
    float cy;

    half = (float)U_I((unsigned)&ai_Unkfd7c50 + 0x20) * 0.5f;
    x = ((float)U_I(param_1 + 8) - half) * 5.33f;
    y = ((float)U_I(param_1 + 0xc) - half) * 5.33f;
    cx = (fabs(x) / ((float)U_I((unsigned)&ai_MapSize) * 0.5f)) * 51.0f;
    if (0.0f <= x)
        cx = cx + x;
    else
        cx = x - cx;
    cy = (fabs(y) / ((float)U_I((unsigned)&full_data_6f4ee4) * 0.5f)) * 51.0f;
    if (0.0f <= y)
        cy = cy + y;
    else
        cy = y - cy;
    *(float *)(param_1 + 0x10) = (float)param_2 + (float)U_I((unsigned)&ai_MapSize) * 0.5f + cx;
    *(float *)(param_1 + 0x14) = (float)U_I((unsigned)&full_data_6f4ee4) * 0.5f + (float)param_3 + cy;
}
