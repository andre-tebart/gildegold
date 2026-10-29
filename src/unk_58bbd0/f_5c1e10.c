/* 0x5c1e10, the menu callback of 0x5c1df0 (846 B, ROUGH SKELETON): 0x4e6fc0 (param_1, 6, the local 0x8c
   record buffer) first, then the list rows; the body is left as a comment. The frame is 0xac, the tail at
   0x5c2159 */

int ai_Unk5c1e10(int param_1)
{
    char buf[0x8c];
    int n;

    n = amt_GetColumnOffices(param_1, 6, buf);
    if (n <= 0)
        return 0;
    /* ROUGH SKELETON: the list rows of the records and the row builder */
    return 0;
}
