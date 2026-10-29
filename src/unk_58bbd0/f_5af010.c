/* 0x5af010, the total amount of a transport record (400 B): the kind record 0x7e3f30 + kind * 0x24d; the owner
   is the +0x65 word for the kind 0x47, else the player 0x6f1bc5. Both carrier tables (0x6f58a0 .. 0x6f5a60 and
   0x6f4f60 .. 0x6f52e0, 0x38-byte rows, the good at +0, the control id at +0x10) add the row's amount 0x475980
   times its price 0x5089c0 (times 1.1 for a "\n" kind, else times the float +0x49 of the record; the +0x65
   amount again for the modes 2 / 4). 0x6f22dc = the negated total */

void ai_Unk5af010(unsigned char *param_1, int param_2, int param_3)
{
    int n;
    float sum;
    char *kind;
    int owner;
    int *row;

    kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    sum = 0.0f;
    if (*param_1 == 0x47)
        owner = *(int *)(param_1 + 0x65);
    else
        owner = *(unsigned char *)(unsigned)&ai_OwnPlayer;
    for (row = (int *)(unsigned)&full_data_6f58a0; (int)row < (unsigned)((unsigned)full_data_6f58a0 + 0x1c0); row += 0xe) {
        if (row[4] == -1 || d2_GetObjValue(row[4]) == 0)
            continue;
        n = d2_GetObjValue(row[4]);
        if (n == 0)
            continue;
        if (*kind == 0xa)
            sum += (float)n * (gm_GoodsPrice(row[0], owner) * 1.1);
        else
            sum += (float)n * (gm_GoodsPrice(row[0], owner) * *(float *)(param_1 + 0x49));
        if (param_3 == 2 || param_3 == 4)
            gm_GoodsPrice(row[0], param_1[0x65]);
    }
    for (row = (int *)(unsigned)&full_data_6f4f60; (int)row < (unsigned)((unsigned)full_data_6f4f60 + 0x380); row += 0xe) {
        if (row[4] == -1 || d2_GetObjValue(row[4]) == 0)
            continue;
        n = d2_GetObjValue(row[4]);
        if (n == 0)
            continue;
        if (*kind == 0xa)
            sum += (float)n * (gm_GoodsPrice(row[0], owner) * 1.1);
        else
            sum += (float)n * (gm_GoodsPrice(row[0], owner) * *(float *)(param_1 + 0x49));
        if (param_3 == 2 || param_3 == 4)
            gm_GoodsPrice(row[0], param_1[0x65]);
    }
    U_I((unsigned)&full_data_6f22dc) = -sum;
}
