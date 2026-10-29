/* 0x4e65e0, amt_SendOfficeMessage of the office a as free (kind 3, code 0xff): the owner is gone or not a candidate (0x4e85d0) or lost the election (0x4e7c60).
   The parameter is a char: the caller 0x4e85d0 passes the idx byte without an extension (notes/vc6-matching.md) */

int amt_SendOfficeFree(char a)
{
    return amt_SendOfficeMessage(a, 0, 0, 3, 0xff);
}
