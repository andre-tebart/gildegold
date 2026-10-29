/* 0x452e40, forgets the light records of obj (all, or the one of the object src) */

char d3_LightRemoveRecords(D3OBJECT *obj, void *src)
{
    D3LREC *p, *n;

    if (obj && D3_L(obj)->light) {
        if (!src) {
            for (p = D3_L(obj)->light->list; p; p = n) {
                n = p->next;
                mem_Free(p);
            }
            D3_L(obj)->light->list = 0;
            return 1;
        }
        p = D3_L(obj)->light->list;
        if (p) {
            if (p->obj == src) {
                D3_L(obj)->light->list = p->next;
                mem_Free(p);
                return 1;
            }
            while ((n = p->next, n == 0 || n->obj != src)) {
                p = n;
                if (!n)
                    return 1;
            }
            p->next = n->next;
            mem_Free(n);
        }
    }
    return 1;
}
