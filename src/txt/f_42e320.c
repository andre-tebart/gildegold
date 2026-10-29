/* 0x42e320, frees the text buffer of one file. */

void txt_FreeFile(char *name)
{
    int i = txt_FindFile(name);

    if (i != -1) {
        mem_Free(txt_Files[i].buf);
        txt_Files[i].buf = NULL;
    }
}
