/* 0x4ac440, "hmt_CreateBinaryFile": reads the persons (p_%i) and the events (k_%i) of historical.ini next to the
   exe and writes them to historical.dfn (0x4ac8a0); 0 or -1
   @deep code hunks 227 (131 ours / 135 original instructions, calls 11 = 11): every hunk is the same statements in another source shape - the two atoi checks compile as test eax,eax / jg with the out-of-line return -1 (or eax,-1) blocks of the original against our cmp eax,0 / jle with one shared return, the record loops walk the 0x2c-byte records with a byte offset in the original, and 'historical.ini' is an inlined strcat whose word/byte loads take other registers; no missing statement, constant or call left to fix */

int hmt_CreateBinaryFile(void)
{
    HMTFILE h;
    char sec[32];
    char buf[256];
    char path[260];
    int i, n;

    h.np = 0;
    h.nk = 0;
    h.p = 0;
    h.k = 0;
    GetModuleFileNameA(main_Instance, path, 0x104);
    strrchr(path, '\\')[1] = 0;
    strcat(path, "historical.ini");
    GetPrivateProfileStringA("main", "kcnt", ch_EmptyName, buf, 0x100, path);
    h.nk = atoi(buf);
    if (h.nk <= 0)
        return -1;
    GetPrivateProfileStringA("main", "pcnt", ch_EmptyName, buf, 0x100, path);
    h.np = atoi(buf);
    if (h.np <= 0)
        return -1;
    h.k = mem_Alloc(h.nk * sizeof(HMTREC), "hmt_CreateBinaryFile");
    if (h.k != 0) {
        h.p = mem_Alloc(h.np * sizeof(HMTREC), "hmt_CreateBinaryFile");
        if (h.p != 0) {
            n = 0;
            memset(h.k, 0, h.nk * sizeof(HMTREC));
            memset(h.p, 0, h.np * sizeof(HMTREC));
            for (i = 0; i < h.nk; i++) {
                h.k[i].idx = n++;
                sprintf(sec, "k_%i", i);
                GetPrivateProfileStringA(sec, "begin", "-1", buf, 0x100, path);
                h.k[i].begin = atoi(buf);
                GetPrivateProfileStringA(sec, "vname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.k[i].vname, buf, 0xf);
                GetPrivateProfileStringA(sec, "nname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.k[i].nname, buf, 0xf);
                GetPrivateProfileStringA(sec, "end", "-1", buf, 0x100, path);
                h.k[i].end = atoi(buf);
            }
            for (i = 0; i < h.np; i++) {
                h.p[i].idx = n++;
                sprintf(sec, "p_%i", i);
                GetPrivateProfileStringA(sec, "begin", "-1", buf, 0x100, path);
                h.p[i].begin = atoi(buf);
                GetPrivateProfileStringA(sec, "vname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.p[i].vname, buf, 0xf);
                GetPrivateProfileStringA(sec, "nname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.p[i].nname, buf, 0xf);
                GetPrivateProfileStringA(sec, "end", "-1", buf, 0x100, path);
                h.p[i].end = atoi(buf);
            }
            if (hmt_WriteBinaryFile(&h) == 0) {
                mem_Free(h.k);
                h.k = 0;
                mem_Free(h.p);
                return 0;
            }
        }
        if (h.k != 0) {
            mem_Free(h.k);
            h.k = 0;
        }
    }
    if (h.p != 0)
        mem_Free(h.p);
    return -1;
}
