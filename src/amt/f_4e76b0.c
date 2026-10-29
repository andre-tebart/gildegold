/* 0x4e76b0, the applicants for the office of the kind k: the characters (up to n, at most 6 offices looked at) holding an office of the column of the kind k whom
   amt_CanTakeOffice accepts for k, into out[]; returns their number. The AI partner choice (0x4a0240, 0x4a3af0) and the vote dialog (cm.c 0x4dab50) use it */

int amt_GetOfficeApplicants(int k, int n, char **out)
{
    AMTKIND a;
    AMTREC list[6];
    int cnt, m, i;
    char *c;

    if (amt_GetKind(k, &a) == 0)
        return 0;
    m = n;
    if (n >= 6)
        m = 6;
    m = amt_GetColumnOffices(a.col, m, list);
    if (m == 0)
        return 0;
    if (m >= n)
        m = n;
    cnt = 0;
    for (i = 0; i < m; i++) {
        c = gm_FindChar(list[i].owner);
        if (c != 0 && amt_CanTakeOffice(c, k) != 0)
            out[cnt++] = c;
    }
    return cnt;
}
