/* 0x423530, releases and frees the movie; returns 0 when no movie has that id */

int mov_Close(int id)
{
    MOVIE *m;

    m = mov_Find(id);
    if (m) {
        mov_Release(m);
        mov_Free(m);
        return 1;
    }
    return 0;
}
