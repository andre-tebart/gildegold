/* 0x47ce30, frees animation n */

void d2_FreeAnim(int n)
{
    memset(&d2_Anims[n], 0, 0x11);
}
