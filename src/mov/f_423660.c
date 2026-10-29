/* 0x423660, allocates a zeroed MOVIE and appends it to mov_List; NULL when malloc fails */

MOVIE *mov_Alloc(void)
{
    MOVIE *m, *p;

    m = malloc(sizeof(MOVIE));
    if (m) {
        memset(m, 0, sizeof(MOVIE));
        if (mov_List) {
            for (p = mov_List; p->next; p = p->next)
                ;
            p->next = m;
            m->prev = p;
            return m;
        }
        mov_List = m;
    }
    return m;
}
