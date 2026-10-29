/* 0x589bc0, a screenshot to disk (198 B): 0x52e330 (0x631090) and the pump 0x5309a0 (0, U_I (0x760638)); the
   name 0x62aa58 with the counter 0x6f2674 (incremented) in 0x5de652; the image 0x4223c0 (the screen 0xfb9072 *
   0xfb9070 * 3, 0x62aa4c), 0x41dec0 (), 0x478c50 (0, 0, 0xfb9072, 0xfb9070, U_I (0x1463590), 0, 0, the image),
   0x41e050 (), 0x401b90 (the name, 0xfb9072, 0xfb9070, the image), 0x422620 (the image) and 0x52e330
   (U_I (0x144adb4)) */

void * mem_Alloc(int size, char *tag); char dd_Lock(void); void d2_Convert16ToRgb(); void dd_Unlock(); unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data); void mem_Free(void *p);

void ai_Unk589bc0(void)
{
    char name[0x40];
    char *img;
    int n;

    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
    n = U_I((unsigned)&full_data_6f2674);
    U_I((unsigned)&full_data_6f2674) = n + 1;
    sprintf(name, (char *)(unsigned)&full_data_62aa58, n);
    img = (char *)mem_Alloc(*(short *)(unsigned)&d2_ScreenW * *(short *)(unsigned)&d2_ScreenH * 3, (char *)(unsigned)&full_data_62aa4c);
    dd_Lock();
    d2_Convert16ToRgb(0, 0, *(short *)(unsigned)&d2_ScreenW, *(short *)(unsigned)&d2_ScreenH, U_I((unsigned)&dd_Surface), 0, 0, img);
    dd_Unlock();
    bmp_Save24(name, *(short *)(unsigned)&d2_ScreenW, *(short *)(unsigned)&d2_ScreenH, img);
    mem_Free(img);
    sv_SetStatusText((char *)U_I((unsigned)&full_data_144adb4));
}
