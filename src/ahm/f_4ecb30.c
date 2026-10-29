/* 0x4ecb30, puts the order p (+8 the id, +0x10 the value) into the first free place of the record rec (6 pairs at +0xc) of the object o (+0x5d the list); 0 when all are in use */

int ahm_AddPlace(int *p, char *rec, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMREC *r = (AHMREC *)rec;
    AHMORDER *m = (AHMORDER *)p;
    int i;
    AHMGOOD *sp;
    int *q;

    q = (int *)(rec + 0xc);
    i = 0;
    do {
        if (*q == 0) {
            sp = (AHMGOOD *)gm_Query(obj->objs, 2, 6, 0, 0xff);
            if (net_Conn.s != INVALID_SOCKET)   /* a network game: the server's id counter wins */
                ai_NextObjectId = m->nextObjectId;
            r->slots[i].id = m->id;
            r->slots[i].v = m->value.f;
            if (sp != 0)
                gm_AddObjekt(sp->objId, p[2], 2);
            gm_RefreshGoodsLists(ai_OwnPlayer);
            return 1;
        }
        i++;
        q += 2;
    } while (i < 6);
    return 0;
}
