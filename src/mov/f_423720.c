/* 0x423720, finds the movie with the given id; NULL when there is none */

MOVIE *mov_Find(int id)
{
    MOVIE *m = mov_List, *found = NULL;

    if (m != NULL) {
        do {
            if (m->id == id)
                break;
            m = m->next;
        } while (m != NULL);
        found = m;
    }
    return found;
}
