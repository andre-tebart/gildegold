/* 0x57c560, the tutorial result string writer (301 B, ROUGH SKELETON): p = 0x4eaa50 (arg 1) (the record looked
   up); 0x521040 (the player user record 0x770b80 + U16(0x6f25b8) * 0x21c, arg 3, the byte 0x6f1bc5): when true
   and p is set, the global text U_I(0x144a2e0) is written (0x521150 (text, 0x100)) and 0 answers; else the
   string built by 0x5176d0 (the 0x200 buffer, 0x4044, the tables 0x144a7a8[arg2->0x10] / 0x144a80c[arg2->0x14])
   and written by 0x521150 (buffer, 1), then 0x4ec490 (arg 1, arg 2) and the rest (the tail is not written yet,
   see build/dis57c560.txt) */

char * ahm_OwnerChar();
int sv_CheckGoodAmount();
int sv_MessageBox();
void txt_Format(char *out, int id0, ...);
int ahm_SendCreateAlchemist();

int ai_Unk57c560(int a, char *rec, int c)
{
    char buf[0x200];
    char *p;

    p = (char *)ahm_OwnerChar(a);
    if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c), c, *(unsigned char *)(unsigned)&ai_OwnPlayer)) {
        if (p)
            sv_MessageBox(U_I((unsigned)&full_data_144a2e0), 0x100);
        return 0;
    }
    txt_Format(buf, 0x4044, U_I((unsigned)&ai_Unk144a7a8 + *(int *)(rec + 0x10) * 4),
                 U_I((unsigned)&ai_Unk144a80c + *(int *)(rec + 0x14) * 4));
    if (!sv_MessageBox(buf, 1))
        return 0;
    /* ROUGH: 0x4ec490 (arg 1, arg 2) and the tail are not written yet */
    return 0;
}
