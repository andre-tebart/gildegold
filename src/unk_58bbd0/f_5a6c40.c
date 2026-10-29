/* 0x5a6c40, four map controls (102 B): 0x4731d0 (0, 0, the id, the screen) / (0, 0x1a2, the id + 1) /
   (0, 0x78, the id + 2) / (0x243, 0x78, the id + 3) placed by 0x472f50 */

void ai_Unk5a6c40(int param_1, int param_2)
{
    d2_RaiseObject(d2_WinOpenPic(0, 0, param_2, param_1));
    d2_RaiseObject(d2_WinOpenPic(0, 0x1a2, param_2 + 1, param_1));
    d2_RaiseObject(d2_WinOpenPic(0, 0x78, param_2 + 2, param_1));
    d2_RaiseObject(d2_WinOpenPic(0x243, 0x78, param_2 + 3, param_1));
}
