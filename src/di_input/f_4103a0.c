/* 0x4103a0 */
HKEY reg_OpenKey(char *name, int mode)
{
    char path[256];
    HKEY key;
    DWORD disp;
    int r;

    sprintf(path, "%s%s", "Software\\Ahead Entertainment\\", name);
    switch (mode) {
    case 1:
        r = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ, &key);
        break;
    case 2:
        r = RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, 0, KEY_WRITE, NULL, &key, &disp);
        break;
    default:
        return (HKEY)-1;
    }
    if (r == 0)
        return key;
    return (HKEY)-1;
}
