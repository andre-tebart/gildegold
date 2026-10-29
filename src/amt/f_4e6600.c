/* 0x4e6600, amt_SendOfficeMessage of the office of the character c (amt_FindOffice) with the rival x (kind 2, code 1); -1 when c has no office or x is 0 */

int amt_SendOfficeCandidates(char *c, char *x)
{
    AMTKIND k;
    AMTREC o;

    if (amt_FindOffice(c, &k, &o) == 0)
        return -1;
    if (x == 0)
        return -1;
    return amt_SendOfficeMessage(o.idx, c, x, 2, 1);
}
