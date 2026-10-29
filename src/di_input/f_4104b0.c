/* 0x4104b0, store value as a REG_SZ string under name */

void reg_SetString(HKEY key, char *name, char *value)
{
    RegSetValueEx(key, name, 0, REG_SZ, (BYTE *)value, strlen(value) + 1);
}
