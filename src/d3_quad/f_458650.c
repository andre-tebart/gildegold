/* 0x458650, sets the zone of the objects in the nodes that the camera box touches */

void d3q_Mark(D3QNODE *n, D3OBJECT *cam, int zone)
{
    unsigned int r, i;
    D3QITEM *it;
    D3BOXVERT *box;

    if (n) {
        box = d3q_BoxCorners(n, &cam->pos.x, &cam->matrix._11);
        r = d3_ClipBoxU(box, 0, 0);
        if (r & 0x40) {
            for (it = n->first; it; it = it->next) {
                if (it->obj->material)
                    D3Q_MAT(it->obj)->zone = zone;
            }
        } else if (r & 0x3f) {
            for (i = 0; i < 4; i++) {
                if (n->child[i])
                    d3q_Mark(n->child[i], cam, zone);
            }
        }
    }
}
