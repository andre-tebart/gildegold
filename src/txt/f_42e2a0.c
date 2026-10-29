/* 0x42e2a0, frees the text buffers of all loaded files. */

void txt_FreeFiles(void)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (txt_Files[i].buf) {
            mem_Free(txt_Files[i].buf);
            txt_Files[i].buf = NULL;
            memset(&txt_Files[i], 0, sizeof(TXTFILE));
        }
    }
}
