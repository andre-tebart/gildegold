/* 0x4e8530, 1 when the office r (state 2) of the column col has two valid candidates and its x14 character is in the list of the column */

int amt_Unk4e8530(int col, AMTREC *r)
{
    AMTREC list[6];
    int n, j;
    char *a, *b;

    if (r->state == 2) {
        n = amt_GetColumnOffices(col, 6, list);
        a = gm_FindChar(r->x14);
        b = gm_FindChar(r->owner);
        if (a != 0 && (unsigned char)a[8] > 0 && a[0x1b1] == 0 && b != 0 && (unsigned char)b[8] > 0) {
            for (j = 0; j < n; j++) {
                if (list[j].owner == r->x14)
                    return 1;
            }
        }
    }
    return 0;
}
