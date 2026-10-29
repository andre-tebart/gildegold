/* 0x41e670 */

DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b)
{
    return (r >> dd_RLoss << dd_RShift) | (g >> dd_GLoss << dd_GShift) | (b >> dd_BLoss << dd_BShift);
}
