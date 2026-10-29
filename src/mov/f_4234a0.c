/* 0x4234a0, returns the next event of the movie's graph with its two parameters, stopping after EC_COMPLETE
   or 0x0e; 0 when no movie has that id or no event is left */

long mov_GetEvent(int id, long *p1, long *p2)
{
    long ev;
    MOVIE *m;

    m = mov_Find(id);
    ev = 0;
    if (m) {
        while (SUCCEEDED(m->event->lpVtbl->GetEvent(m->event, &ev, p1, p2, 0))) {
            m->event->lpVtbl->FreeEventParams(m->event, ev, *p1, *p2);
            if (ev == EC_COMPLETE || ev == 0x0e)
                break;
        }
    }
    return ev;
}
