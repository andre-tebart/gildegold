/* 0x4236b0, unlinks the movie from mov_List and frees it */

void mov_Free(MOVIE *m)
{
    if (!m)
        return;
    if (m->prev) {
        if (m->next) {
            m->next->prev = m->prev;
            m->prev->next = m->next;
        } else
            m->prev->next = NULL;
    } else if (m->next) {
        m->next->prev = NULL;
        mov_List = m->next;
    } else
        mov_List = NULL;
    free(m);
}
