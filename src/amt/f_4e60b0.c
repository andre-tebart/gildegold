/* 0x4e60b0, the rate (the float table 0x60ec90, 7 per row) of the office x for the character c (+0x166 his rank, +0x167 the next one): 1 and *out when the
   level of x is within one above the level of +0x167 and above that of +0x166 */

int amt_GetOfficeRate(char *c, int x, float *out)
{
    if (c != 0 && ((AMTCHARVIEW *)c)->id != 0xffff && x != 0 && x < 0x1e && (unsigned char)c[0x166] < 0x1e && (unsigned char)c[0x167] < 0x1e) {
        if (amt_Kinds[(unsigned char)c[0x167]].level + 1 >= amt_Kinds[x].level && amt_Kinds[(unsigned char)c[0x166]].level < amt_Kinds[x].level) {
            *out = amt_Rates[amt_Kinds[(unsigned char)c[0x166]].col * 7 + amt_Kinds[x].col];
            return 1;
        }
    }
    return 0;
}
