/* 0x447f70, d3_ForEachObject callback of d3if_MoveObject: moves one object of the tree to the new scene and
   deletes the particle systems attached to it */

char d3if_MoveCB(D3OBJECT *obj, D3IFMOVE *m)
{
    D3PARTSYS *p, *next;

    d3_ForEachObject((D3SCENE *)m->from, 0, 7, (D3OBJECTCB)d3_LightRemoveRecords, obj);
    obj->scene = (D3SCENE *)m->to;
    for (p = m->from->par1; p != &d3_ParEnd; p = next) {
        next = p->next;
        if (p->parent == obj)
            par_Delete(p);
    }
    return 1;
}
