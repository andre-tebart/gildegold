/* 0x52e600, a != 0: collects the widgets (0x2ec records of the table 0xf8f080) whose +0x40 points into a visible form record of 0xfb1158 (stride 0x2ac) and that are not yet flagged +0x38, flags them and
   remembers their index (0x6f1c90, count 0x6f23ec); a == 0: clears the flags again */

int gm_Unk504e00();                     /* 0x504e00 */
int ai_Unk4ce170();                     /* 0x4ce170 */

void sv_DisableVisibleWidgets(int a)
{
    int i, n, *p, w;
    short idx;

    if (a != 0) {
        SV_I((unsigned)&full_data_6f23ec) = 0;
        p = (int *)((unsigned)&ai_Unkfb1030 + 0x190);
        w = SV_I((unsigned)&d2_Objs);
        do {
            if (p[-4] != 0 && *p != 0) {
                idx = 0;
                n = 0;
                do {
                    if (*(int *)(n + 0x18 + w) != 0 && *(int **)(n + 0x40 + w) == p - 0x68 && *(int *)(n + 0x38 + w) == 0) {
                        ((unsigned short *)(unsigned)&full_data_6f1c90)[SV_I((unsigned)&full_data_6f23ec)] = idx;
                        SV_I((unsigned)&full_data_6f23ec)++;
                        *(int *)(n + 0x38 + w) = 1;
                        w = SV_I((unsigned)&d2_Objs);
                    }
                    n += 0x2ec;
                    idx++;
                } while (n < 0x8c400);
            }
            p += 0xab;
        } while ((int)p < (unsigned)((unsigned)ai_Unkfb1030 + 0x81d0));
        return;
    }
    i = 0;
    if (0 < SV_I((unsigned)&full_data_6f23ec)) {
        do {
            n = ((unsigned short *)(unsigned)&full_data_6f1c90)[i];
            i++;
            *(int *)(SV_I((unsigned)&d2_Objs) + 0x38 + n * 0x2ec) = 0;
        } while (i < SV_I((unsigned)&full_data_6f23ec));
    }
}
