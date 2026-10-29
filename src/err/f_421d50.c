/* 0x421d50, writes the symbol (and its file) that contains w->addr to the crash log; 0 when nothing
   matched or the walk is over (w->last / w->repeat / w->count fold repeated frames) */

char err_WriteSymbol(ERRWALK *w)
{
    ERRMAP *map = w->map;
    ERRSYM *sym = (ERRSYM *)((char *)map + map->symoff + 0x10);
    unsigned int i;
    DWORD n;

    for (i = 0; i < map->nsyms; i++) {
        if (w->addr >= sym->lo && w->addr <= sym->hi) {
            char unknown[] = "??? ";
            char arrow[] = "--> ";
            char *prefix = NULL;
            char show = err_ShowUnknown;

            if (w->last == 0)
                w->last = sym->lo;
            else if (w->last == w->prev) {
                w->last = sym->lo;
                w->repeat++;
            } else {
                prefix = unknown;
                if (w->count > 0 && w->repeat <= 1) {
                    prefix = arrow;
                    w->last = sym->lo;
                    w->count--;
                } else if (!show)
                    return 0;
            }
            {
                ERRSTR *file = err_GetString(map, sym->file);
                char crlf[] = "\r\n";
                char sp[] = " ";

                if (prefix)
                    WriteFile(w->file, prefix, 4, &n, NULL);
                WriteFile(w->file, sym->name, sym->len - 1, &n, NULL);
                if (file) {
                    WriteFile(w->file, sp, 1, &n, NULL);
                    WriteFile(w->file, file->s, file->len - 1, &n, NULL);
                }
                WriteFile(w->file, crlf, 2, &n, NULL);
            }
            return 1;
        }
        sym = (ERRSYM *)((char *)sym + sym->len + 0x10);
    }
    return 0;
}
