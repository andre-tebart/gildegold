/* 0x51ad60, the action queue entry n (0x631050), NULL for -1 */


int ls_ActionAt(int n)
{
    if (n == -1)
        return 0;
    return (int)ls_ActionQueue + n * 0x194;
}
