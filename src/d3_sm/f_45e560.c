/* 0x45e560, cells of type from enclosed by from/to on all four sides become to @deep behaviour: 8 code hunks from one scheduling difference, the whole asmdiff read (only one real difference: we hoist the parameter 'to' out of [esp+0x20] into esi before the first cell test, 'mov esi,[esp+0x20]' at 45e5af + 'cmp eax,esi', the original reads it lazily at 45e5c7 and compares 'cmp eax,[esp+0x20]' at 45e5be; every value, the check order e, e[-size], e[+size], e[-1], e[+1], and the e->type = to store are identical), plus 2 fill nops; the same operation in another register form, so a source variant is needed, not a meaning fix; readability already readable */

void d3sm_Replace(D3SM *h, int from, int to)
{
    D3SMENTRY *e;
    int x, z;

    for (z = 1; z < h->size - 1; z++) {
        for (x = 1, e = &h->entries[h->size * z + 1]; x < h->size - 1; x++, e++) {
            if (e->type == from
                && (e[-h->size].type == from || e[-h->size].type == to)
                && (e[h->size].type == from || e[h->size].type == to)
                && (e[-1].type == from || e[-1].type == to)
                && (e[1].type == from || e[1].type == to))
                e->type = to;
        }
    }
}
