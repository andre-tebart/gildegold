/* 0x5282f0, colours the banner of the building b (+0x61 its model) for a character owner (type 5 / 6 / 7, +0x27): the model's parts are walked (0x438d40, 0x100, the callback 0x528180) with the model's +0x1f0 cleared
   when its flag +0x210 & 1 is not set */

void sv_Unk5282f0(char *b)
{
    char *m;
    char c;
    int save;

    m = *(char **)(b + 0x61);
    if (m != 0 && *(unsigned short *)(b + 0x27) != 0xffff) {
        c = ai_Chars[*(unsigned short *)(b + 0x27)][2];
        if (c == 6 || c == 7 || c == 5) {
            if ((*(unsigned char *)(m + 0x210) & 1) == 0) {
                save = *(int *)(m + 0x1f0);
                *(int *)(m + 0x1f0) = 0;
                d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x100, sv_Unk528180, b);
                *(int *)(*(int *)(b + 0x61) + 0x1f0) = save;
                return;
            }
            d3_ForEachObject(ai_Unk5fb5b0, m, 0x100, sv_Unk528180, b);
        }
    }
}
