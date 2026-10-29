/* 0x531ba0, the first init: registers the exit handler (0x4220b0 (0x531750, 0x44ee40, the window, 0x10)), sets 0x4226a0 (48000) / 0x42ea90 (0x80), creates "<path>gamedata\Screenshots" and starts the
   project gfx path (0x431490) */

int main_Init(void)
{
    char dir[260];

    err_Init((int)main_Exit, d3io_NoopCallback, main_Window, 0x10);
    mem_Init(48000);
    tr_startup(0x80);
    sprintf(dir, "%sgamedata\\Screenshots", (char *)(unsigned)&main_GfxPath);
    CreateDirectoryA(dir, 0);
    vfs_Init((char *)(unsigned)&main_GfxPath, 0);
    tb_Init(0xe, 0);
    return 1;
}
