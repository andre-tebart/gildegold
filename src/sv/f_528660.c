/* 0x528660, a callback collecting the names o that contain "sp_WIMPEL" into the list p (p[0] the count, then the pointers); 1 while below 0x20 */

char sv_Unk528660(char *o, int *p)
{
    if (strstr(o, "sp_WIMPEL") != 0) {
        p[p[0] + 1] = (int)o;
        p[0]++;
    }
    return p[0] < 0x20;
}
