/* 0x4bf410, removes mark i */

void rt_RemoveMark(int i)
{
    d2_RemoveObj(rt_Marks[i].obj);
    rt_Marks[i].obj = -1;
    rt_Marks[i].p = 0;
}
