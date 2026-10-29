/* 0x422d60, initializes COM for the movie module, resets the id counter and the window class; returns 1 */

int mov_Init(void)
{
    CoInitialize(NULL);
    mov_NextId = 666;
    mov_Class = 0;
    return 1;
}
