/* 0x410430, close a registry key opened by reg_OpenKey */

void reg_CloseKey(HKEY key)
{
    RegCloseKey(key);
}
