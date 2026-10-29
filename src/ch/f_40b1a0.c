/* 0x40b1a0, frees the walk paths, the characters and the action queue */

void ch_Close(void)
{
    int i;

    if (ch_PathMap)
        ch_MapFree(ch_PathMap);
    if (ch_Queue) {
        for (i = 0; i < CH_QUEUE; i++) {
            if (ch_Queue[i].type == '-' && ch_Queue[i].u.walk.path) {
                void **p = &ch_Queue[i].u.walk.path;

                mem_Free(*p);
                *p = 0;
            }
        }
    }
    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i])
            ch_Delete(ch_List[i]);
    if (ch_Queue) {
        mem_Free(ch_Queue);
        ch_Queue = 0;
    }
}
