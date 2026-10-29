/* 0x42c9c0, reads the #include lines of a text definition file and loads each included file. */

int txt_LoadIncludes(char *file)
{
    FILE *f;
    char line[200];
    char inc[128][512];
    int n = 0;
    int i;
    char *p;
    char *q;

    txt_Count = 0;
    p = strrchr(file, '\\');
    if (p) {
        memcpy(txt_Dir, file, p - file + 1);
        txt_Dir[p - file + 1] = 0;
    } else
        strcpy(txt_Dir, "");
    f = fopen(file, "rt");
    if (!f) {
        MessageBox(NULL, "Could not open Textfiledefinition!", "Error", MB_ICONEXCLAMATION);
        mem_Free(txt_Buffer);
        txt_Buffer = NULL;
        return 0;
    }
    do {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), f);
        if (!strstr(line, "//") && (p = strstr(line, "#include"))) {
            p = strchr(p, '"');
            if (!p) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
            p++;
            q = strchr(p, '"');
            if (!q) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
            *q = 0;
            *strrchr(p, '.') = 0;
            strcpy(inc[n++], p);
            if (n > 128) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
        }
    } while (!feof(f));
    fclose(f);
    for (i = 0; i < n; i++)
        if (!txt_LoadTextFile(inc[i]))
            return 0;
    txt_Loaded = 1;
    return 1;
}
