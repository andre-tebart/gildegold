/* 0x4e6650, amt_SendOfficeMessage of the office a with the character b as its owner (kind 1, the code c); -1 when b is 0 */

int amt_SendOfficeOwner(int a, char *b, int c)
{
    if (b == 0)
        return -1;
    return amt_SendOfficeMessage((char)a, b, 0, 1, (unsigned char)c);
}
