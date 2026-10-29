/* 0x455760 */

void par_Delete(D3PARTSYS *ps)
{
    D3SCENE *s = ps->scene;

    if (s == d3_DefaultScene) {
        ps->next->prev = ps->prev;
        ps->prev->next = ps->next;
        if (ps == D3PAR_SCENE_PAR(s)->head)
            D3PAR_SCENE_PAR(s)->head = ps->next;
        if (ps == D3PAR_SCENE_PAR(s)->tail)
            D3PAR_SCENE_PAR(s)->tail = ps->next;
    } else
        par_Unlink(ps);
    if (ps->tex)
        tex_Release(ps->tex);
    mem_Free(ps->particles);
    ps->particles = 0;
    mem_Free(ps->points);
    ps->points = 0;
    mem_Free(ps->polys);
    ps->polys = 0;
    mem_Free(ps);
}
