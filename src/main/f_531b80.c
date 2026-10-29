/* 0x531b80, switches the screen saver (SPI_SETSCREENSAVEACTIVE 0x11) on / off */

void main_SetScreenSaver(unsigned on)
{
    SystemParametersInfoA(0x11, on, 0, 2);
}
