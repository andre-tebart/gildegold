/* 0x4ecbc0, removes the place of the id p[2] from the record rec and from the list of the object o; 0 when there is none */

int ahm_RemovePlace(int *p, char *rec, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMREC *r = (AHMREC *)rec;
    int i;
    char *sp;
    int *q;

    q = (int *)(rec + 0xc);
    i = 0;
    do {
        if (*q == p[2]) {
            sp = gm_Query(obj->objs, 2, 6, 0, 0xff);
            if (sp != 0)
                gm_RemoveObject(sp + 0x14, p[2]);
            r->slots[i].id = 0;
            r->slots[i].v = 0.0f;
            return 1;
        }
        i++;
        q += 2;
    } while (i < 6);
    return 0;
}
