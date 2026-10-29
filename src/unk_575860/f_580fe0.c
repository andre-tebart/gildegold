/* 0x580fe0, a scene check (49 B): 0x56f170 (the record + 1); when it answers non-zero, 0x521150 (0x144a19c,
   0); 1 when 0x56f170 answered 0, else 0 */

int he_Unk56f170(); int sv_MessageBox();

int ai_Unk580fe0(char *rec)
{
    int r;

    r = he_Unk56f170(U_I(rec + 1));
    if (r)
        sv_MessageBox(U_I((unsigned)&full_data_144a19c), 0);
    return r == 0;
}
