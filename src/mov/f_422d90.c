/* 0x422d90, releases and frees every movie of the list, then uninitializes COM */

void mov_Exit(void)
{
    while (mov_List) {
        mov_Release(mov_List);
        mov_Free(mov_List);
    }
    CoUninitialize();
}
