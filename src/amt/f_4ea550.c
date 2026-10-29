/* 0x4ea550, changes the opinion (cm_SendRelations, by v) of the owners of the offices of the district d (1 .. 7) of the
   character c, except c's own office */

void amt_ChangeDistrictOpinion(char *c, int d, int v)
{
    AMTREC list[6];
    int n, i;

    if (d >= 1 && d <= 7) {
        n = amt_GetColumnOffices(d, 6, list);
        if (n > 0) {
            for (i = 0; i < n; i++) {
                if (list[i].owner != AMT_NETID(c) && list[i].owner != -1)
                    cm_SendRelations(AMT_NETID(c), list[i].owner, v, 1);
            }
        }
    }
}
