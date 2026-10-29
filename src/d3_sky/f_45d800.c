/* 0x45d800, unlinks the layer l from the sky's list (the head or after its predecessor), releases its texture and frees the layer; does nothing without a sky or layer */

void d3sky_DeleteLayer(D3SKY *s, D3SKYLAYER *l)
{
    D3SKYLAYER *p;

    if (s && l) {
        if (s->layers == l)
            s->layers = l->next;
        else {
            for (p = s->layers; p && p->next != l; p = p->next)
                ;
            if (p && p->next == l)
                p->next = l->next;
        }
        tex_Release(l->t);
        mem_Free(l);
    }
}
