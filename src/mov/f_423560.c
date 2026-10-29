/* 0x423560, releases every COM interface of the movie */

#define MOV_RELEASE(p) if (p) { (p)->lpVtbl->Release(p); (p) = NULL; }

void mov_Release(MOVIE *m)
{
    MOV_RELEASE(m->iface[3]);
    MOV_RELEASE(m->iface[4]);
    MOV_RELEASE(m->iface[2]);
    MOV_RELEASE(m->iface[1]);
    MOV_RELEASE(m->iface[0]);
    MOV_RELEASE(m->video);
    MOV_RELEASE(m->audio);
    MOV_RELEASE(m->window);
    MOV_RELEASE(m->control);
    MOV_RELEASE(m->event);
    MOV_RELEASE(m->graph);
}
