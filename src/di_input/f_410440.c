/* 0x410440 */

int reg_GetString(HKEY key, char *name, char *out)
{
    BYTE buf[128];
    DWORD size = 128;
    BYTE *p;
    BYTE c;

    if (RegQueryValueEx(key, name, NULL, NULL, buf, &size) == 0) {
        p = buf;
        do {
            c = *p;
            p[(BYTE *)out - buf] = c;
            p++;
        } while (c);
        return 1;
    }
    return 0;
}
