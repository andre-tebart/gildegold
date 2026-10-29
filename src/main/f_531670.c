/* 0x531670, destroys the game window and unregisters its class "Die Gilde" */

void main_DestroyWindow(void)
{
    DestroyWindow(main_Window);
    UnregisterClassA("Die Gilde", (HINSTANCE)main_Instance);
}
