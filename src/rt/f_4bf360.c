/* 0x4bf360, gives mark m the picture of its point's index (0x7bb + n) @deep Q4 only: the one raw access reads D2OBJ.pic (+8) through rt.h's char* d2_Objs view; a typed field needs d2.h in rt.c, whose declarations conflict with rt.h's views (d2_WinOpenPic int vs short), the recorded rt.h type debt */

void rt_SetMarkPic(RTMARK *m)
{
    int n;

    n = rt_PointIndex(rt_EditRoute, m->p);
    if (*(int *)(d2_Objs + m->obj * 0x2ec + 8) != n + 0x7bb)
        d2_SetObjPic(m->obj, n + 0x7bb);
}
