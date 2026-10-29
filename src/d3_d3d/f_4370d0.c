/* 0x4370d0, D3D device restore after the window is activated (main.c's WM_ACTIVATEAPP loop calls
   it after dd_Restore): a stub that always reports success, so the loop never waits; returns 1 */

char d3d_Restore(void)
{
    return 1;
}
