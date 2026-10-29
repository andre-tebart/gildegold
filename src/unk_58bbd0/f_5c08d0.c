/* 0x5c08d0, the slider percents of a record list (144 B): for every 0xe-dword record with a character and a
   slider (+8) the relation 0x50c550 is set, 0 when one of the kinds 6 / 7 meets another one */

void ai_Unk5c08d0(short *param_1, int param_2, int *param_3)
{
    short *chr;

    if (param_1 == 0 || *param_1 == -1)
        return;
    for (; param_2 != 0; param_2--) {
        chr = (short *)*param_3;
        if (chr != 0 && *chr != -1 && param_3[2] != -1) {
            if (((char)param_1[1] == 6 || (char)param_1[1] == 7) &&
                ((char)chr[1] == 6 || (char)chr[1] == 7))
                d2_SetObjValue(param_3[2], 0, 100, 0);
            else
                d2_SetObjValue(param_3[2], 0, 100, (int)gm_Liking(*param_1, *chr, 1));
        }
        param_3 += 0xe;
    }
}
