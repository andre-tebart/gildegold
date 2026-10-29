/* 0x4113f0, set the mouse position in dm_P, dm_S and dm_L and on the cursor when not absolute */

void di_SetMousePos(int x, int y)
{
    dm_P.x = (short)x;
    dm_P.y = (short)y;
    dm_S.x = dm_P.x;
    dm_S.y = dm_P.y;
    dm_L.x = dm_P.x;
    dm_L.y = dm_P.y;
    if (di_Absolute == 0)
        SetCursorPos(x, y);
}
