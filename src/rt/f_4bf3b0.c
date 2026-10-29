/* 0x4bf3b0, 0x4bf360 for each mark */

void rt_UpdateMarkPics(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1)
            rt_SetMarkPic(m);
    }
}
