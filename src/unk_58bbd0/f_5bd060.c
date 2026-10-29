/* 0x5bd060, the clamp of a combined id (88 B): with param_1 and param_3 positive,
   out = the larger of param_1 and min(param_2, param_3 * 10000 + param_1); returns 1, else 0 */

int ai_Unk5bd060(int param_1, int param_2, int param_3, int *param_4)
{
    int v;
    int m;

    if (param_1 < 1 || param_3 < 1)
        return 0;
    v = param_3 * 10000 + param_1;
    m = v <= param_2 ? v : param_2;
    *param_4 = param_1 < m ? m : param_1;
    return 1;
}
