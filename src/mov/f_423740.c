/* 0x423740, finds the movie that was opened with the given window; NULL when there is none */

MOVIE *mov_FindWnd(HWND hwnd)
{
    MOVIE *m = mov_List, *found = NULL;

    if (m != NULL) {
        do {
            if (m->hwnd == hwnd)
                break;
            m = m->next;
        } while (m != NULL);
        found = m;
    }
    return found;
}
