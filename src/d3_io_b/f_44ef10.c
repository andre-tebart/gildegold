/* 0x44ef10, parses the chunks of one table level; 0x2f / 0x2b end it, 0x28 runs its end handler */

void bgi_Parse(void *f, BGIENT *t, int n, BGIRD *c)
{
    int id, i;
    BGIENT *e;
    void (*h)(void *, BGIRD *);

    id = bgi_ReadId(f);
    if (id != 0x27) {
        while (1) {
            if (id == 0x2f || id == 0x2b)
                break;
            if (id == 0x28) {
                i = bgi_Find(0x28, t, n);
                if (i != 0x27 && (h = (void (*)(void *, BGIRD *))t[i].fn))
                    h(f, c);
                break;
            }
            i = bgi_Find(id, t, n);
            if (i != 0x27) {
                h = (void (*)(void *, BGIRD *))t[i].fn;
                e = &t[i];
                if (!h || e->n) {
                    if (!e->n) {
                        bgi_Depth++;
                        bgi_Parse(f, t, n, c);
                    } else {
                        bgi_Depth++;
                        bgi_Parse(f, (BGIENT *)e->fn, e->n, c);
                    }
                } else {
                    h(f, c);
                }
            }
            id = bgi_ReadId(f);
            if (id == 0x27) {
                bgi_Depth--;
                return;
            }
        }
    }
    bgi_Depth--;
}
