/* 0x5bd470, the carrier list of a screen (276 B): the out array (param_2 entries) is set to -1, then every
   carrier 0x501190 (1, 4, *param_1) with the +0x5a bit 1 clear gets the picture 0x4731d0 (0x427 + the byte
   0), the name 0x5176d0 (0x62d1e8) / 0x473650 and the kind text 0x5176d0 (0x60d224) / 0x473650; the window
   list entry 0xf8f080 + id * 0x2ec gets +0x4c = 1, +0x1c0 = 3 and the carrier at +0x2e8; returns the count */

int ai_Unk5bd470(unsigned short *param_1, int param_2, int *param_3)
{
    int n;
    int i;
    int ctrl;
    int text;
    int y;
    unsigned char *car;
    char buf[256];

    n = 0;
    if (param_2 > 0 && param_3 != 0) {
        for (i = 0; i < param_2; i++)
            param_3[i] = -1;
    }
    car = (unsigned char *)gm_BuildQuery(1, 4, *param_1);
    while (car != 0) {
        if ((car[0x5a] & 1) == 0) {
            text = gm_EquipmentValue(car, (int)(char)car[0x5c]);
            y = n * 0x40;
            ctrl = d2_WinOpenPic(0x20, y, *car + 0x427, U_I((unsigned)&d2_LastWindow));
            txt_Format(buf, (char *)(unsigned)&full_data_62d1e8, car);
            d2_CreateWinText(0x80, y, buf, U_I((unsigned)&d2_LastWindow));
            txt_Format(buf, (char *)(unsigned)"%T", text);
            d2_CreateWinText(0x80, y + 0x1b, buf, U_I((unsigned)&d2_LastWindow));
            if (param_3 != 0 && n < param_2) {
                U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
                U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 3;
                U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x2e8) = (int)car;
                param_3[n] = ctrl;
            }
            n++;
        }
        car = (unsigned char *)gm_BuildNext();
    }
    return n;
}
