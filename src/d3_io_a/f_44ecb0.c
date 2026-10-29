/* 0x44ecb0, ends an object: its polygons and points count for the following objects */

void agi_EndObject(void *unused, AGICTX *c)
{
    c->polybase += c->npoly;
    c->npoly = 0;
    c->ptbase += c->npts;
    c->npts = 0;
}
