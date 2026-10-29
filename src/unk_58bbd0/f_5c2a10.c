/* 0x5c2a10, the master letter window's profession rows (400 B): 0x475f70 (win, 1 / 2) with the texts
   0x602568 / 0x1949 / 0x602568; seven rows of 0x30 bytes: the record param_2 + i * 8 (+0 the object id,
   +4 / +5 two bytes) gives the picture control 0x4731d0 (0xf, i * 0x30, id + 0xdf, screen), its record
   0xf8f080 + id * 0x2ec (+0x1c0 = 3, +0x2e8 = 0x91aaf0 + id * 0x41, +0x4c = 1, +0x28 = the +5 byte == 0),
   param_4[i] = the control and the slider 0x46a960 (100, 0xc + i * 0x30, 200, 0, 0x64, 0x7b, 0x802,
   screen) into param_3[i] (0x475cb0 (slider, 0, 0x64, the +4 byte), 0x475e40 (slider, the +5 byte == 0));
   a zero record gives -1 in both arrays */

void ai_Unk5c2a10(int param_1, char *param_2, int *param_3, int *param_4)
{
    int i;
    int y;
    int id;
    int ctrl;
    int pic;
    int slider;
    int *p;
    int d;

    d2_SetForm(param_1, 1);
    io_PrintText((unsigned)"$C");
    io_PrintText(0x1949);
    d2_SetForm(param_1, 2);
    io_PrintText((unsigned)"$C");
    p = param_3;
    d = (int)param_4 - (int)param_3;
    y = 0xc;
    for (i = 0; i < 7; i++) {
        id = *(int *)(param_2 + i * 8);
        if (id == 0) {
            *p = -1;
            *(int *)((char *)p + d) = -1;
        } else {
            pic = d2_WinOpenPic(0xf, i * 0x30, id + 0xdf, U_I((unsigned)&d2_LastWindow));
            ctrl = U_I((unsigned)&d2_Objs) + pic * 0x2ec;
            *(int *)(ctrl + 0x1c0) = 3;
            *(int *)(ctrl + 0x2e8) = U_I((unsigned)&ai_Goods) + id * 0x41;
            d2_SetObjEnabled(pic, 1);
            *(int *)(ctrl + 0x4c) = 1;
            *(int *)(ctrl + 0x28) = *(unsigned char *)(param_2 + i * 8 + 5) == 0;
            *(int *)((char *)p + d) = pic;
            slider = d2_WinOpenSlider(100, y, 200, 0, 0x64, 0x7b, 0x802, U_I((unsigned)&d2_LastWindow));
            *p = slider;
            d2_SetObjValue(slider, 0, 0x64, *(unsigned char *)(param_2 + i * 8 + 4));
            d2_SetObjEnabled(slider, *(unsigned char *)(param_2 + i * 8 + 5) == 0);
        }
        p++;
        y += 0x30;
    }
}
