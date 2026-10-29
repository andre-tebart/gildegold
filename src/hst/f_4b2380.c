/* 0x4b2380, leaked name (its messages): text k (or a string above 0x4800) "[_SET_n..." / "[_USE_n...":
   returns list n of hst_Lists (_SET empties it first) */

char hst_ListModes[2][5] = { "_SET", "_USE" };  /* 0x601b00, the modes of a list reference: "_SET", "_USE" */

G12ROW *hst_ParseCommandline_1st_Pass(int k)
{
    char buf[0x17d0];
    char err[0x100];
    int set, n;

    set = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(err, 0x100);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    if (*(int *)(buf + 1) == *(int *)hst_ListModes[0]) {
        err[0] = buf[6];
        n = atoi(err);
        set = 1;
    } else if (*(int *)(buf + 1) == *(int *)hst_ListModes[1]) {
        err[0] = buf[6];
        n = atoi(err);
    } else
        return 0;
    if (n > -1 && n < 4) {
        G12ROW *row = &hst_Lists[n];

        if (set)
            ai_Unk4b1ee0((unsigned short)n);
        return row;
    }
    if (n >= 4)
        sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Wrong group index in Label %i", k - hst_TextFirst);
    return 0;
}
