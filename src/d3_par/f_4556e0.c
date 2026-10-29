/* 0x4556e0, unlinks a particle system: fixes the neighbours (or the scene's head / tail when it is the first /
   last system) and, in the default scene, the global list head / tail */

void par_Unlink(D3PARTSYS *ps)
{
    D3SCENE *s = ps->scene;

    if (ps->prev == D3PAR_HEADNODE)
        D3PAR_SCENE_PAR(s)->head = ps->next;
    else
        ps->prev->next = ps->next;
    if (ps->next == &d3_ParEnd)
        D3PAR_SCENE_PAR(s)->tail = ps->prev;
    else
        ps->next->prev = ps->prev;
    if (s == d3_DefaultScene) {
        d3_ParHead = D3PAR_SCENE_PAR(s)->head;
        d3_ParTail = D3PAR_SCENE_PAR(s)->tail;
    }
}
