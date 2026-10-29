/* 0x4bdca0, 0x4bdc90 for each mark */

void rt_ReadMarkValues(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1)
            rt_MarkValue(m);
    }
}
