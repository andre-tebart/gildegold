/* 0x53c9b0, plays the logo movies jowood / 4head / intro / intro_addon (<movie path>*.mpg, window 10,10,0xdc,0xdc; handle 0x6f241c) */


void main_Unk53c9b0(void)
{
    char buf[0x104];

    Sleep(400);
    mov_Init();
    sprintf(buf, "%sjowood.mpg", (char *)(unsigned)&full_data_624798);
    U_I((unsigned)&full_data_6f241c) = mov_Open(buf, 0, U_I((unsigned)&main_Instance), 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(U_I((unsigned)&full_data_6f241c));
    Sleep(400);
    sprintf(buf, "%s4head.mpg", (char *)(unsigned)&full_data_624798);
    U_I((unsigned)&full_data_6f241c) = mov_Open(buf, 0, U_I((unsigned)&main_Instance), 10, 10, 0xdc, 0xdc, 0x8a);
    sv_PumpMessages();
    Sleep(100);
    mov_Play(U_I((unsigned)&full_data_6f241c));
    Sleep(400);
    sprintf(buf, "%sintro.mpg", (char *)(unsigned)&full_data_624798);
    U_I((unsigned)&full_data_6f241c) = mov_Open(buf, 0, U_I((unsigned)&main_Instance), 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(U_I((unsigned)&full_data_6f241c));
    sprintf(buf, "%sintro_addon.mpg", (char *)(unsigned)&full_data_624798);
    U_I((unsigned)&full_data_6f241c) = mov_Open(buf, 0, U_I((unsigned)&main_Instance), 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(U_I((unsigned)&full_data_6f241c));
    sv_PumpMessages();
}
