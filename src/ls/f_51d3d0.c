/* 0x51d3d0, reads the object list 0x7e3f34 (after 0x4fe830 cleared it): the count, then each record in the pieces of the writer 0x519bf0; the count is kept in 0x6f1bdc. Returns 1 */


int ls_ReadObjects(int f)
{
    int n, i;
    char *r;

    gm_ClearObjects();
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        r = (char *)ai_Objects + i * 0x43;
        LS_R(r, 2)
        LS_R(r + 2, 4)
        LS_R(r + 6, 4)
        LS_R(r + 10, 4)
        LS_R(r + 0xe, 4)
        LS_R(r + 0x12, 1)
        LS_R(r + 0x13, 1)
        LS_R(r + 0x1c, 0x1f)
    }
    f3gm_ObjectCount = n;
    return 1;
}
