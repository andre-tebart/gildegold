/* 0x531750, the exit handler (registered with 0x4220b0 by 0x531ba0): module shutdown, window shutdown, engine shutdown, window class, and restores the screen saver setting (0x531b80 (1)) when it was switched off */

void main_Exit(void)
{
    main_ShutdownModules();
    main_ShutdownScreen();
    main_ShutdownEngine();
    main_DestroyWindow();
    if (main_PlatformNT == 0 && main_ScreenSaverOff != 0)
        main_SetScreenSaver(1);
}
