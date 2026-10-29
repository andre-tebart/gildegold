/* 0x4bc7c0, sets the plant at x / z (a new record if none) to type (0 removes it); fall: autumn model @deep behaviour: the remaining 35 code hunks are allocation only - t in [esp+0x20] vs [esp+0x24], esi/eax as zero registers, the second loop rotated with a duplicated state test where the original tests once at the top, q->obj = 0 as a register vs the immediate, the q->state = 1 block inline vs out of line on the return path; meaning read against the disassembly and equal (both searches, veg_NextId post-increment, fall==0 with type==0 delete, veg_LoadModel(q, fall), x/z/size). Q2: the field +0x10 is only written -1 and never read in the exe, no evidence for a role, kept unk10 */

G15PLANT *veg_SetPlant(G15PLANT *p, int x, int z, int type, int fall)
{
    G15TREE *t;
    G15PLANT *q;
    int i;

    t = veg_FindType(type);
    q = 0;
    if (p != 0) {
        for (i = 0; i < 0x40; i++) {
            if (p[i].x == x && p[i].z == z && p[i].state != -1)
                goto found;
        }
        i = 0;
        while (p[i].state != -1) {
            if (++i >= 0x40)
                return 0;
        }
        p[i].id = veg_NextId++;
found:
        q = &p[i];
        q->type = type;
        if (fall == 0) {
            q->unk10 = -1;
            if (type == 0) {
                q->state = -1;
                if (q->obj != 0) {
                    d3if_Delete((struct D3OBJECT *)q->obj);
                    q->obj = 0;
                }
            } else
                q->state = 1;
        } else {
            if (q->obj != 0) {
                d3if_Delete((struct D3OBJECT *)q->obj);
                q->obj = 0;
            }
            veg_LoadModel(q, fall);
        }
        q->x = x;
        q->z = z;
        if (t != 0)
            q->size = t->size;
        else
            q->size = 0;
    }
    return q;
}
