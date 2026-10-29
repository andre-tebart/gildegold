/* 0x4b1f30, marks all lists of hst_Lists used and empty and frees every text file under "historie\" */

void ai_Unk4b1f30(void)
{
    int i, j;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 1;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    for (i = 0; i < 0x80; i++) {
        if (_strnicmp(txt_Files[i].name, "historie\\", 9) == 0)
            txt_FreeFile(txt_Files[i].name);
    }
}
