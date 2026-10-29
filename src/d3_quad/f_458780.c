/* 0x458780, removes obj from the node's list (and from deeper nodes) through d3_ForEachObject */

void d3q_RemoveObject(D3OBJECT *obj, D3QNODE *n)
{
    if (n && obj)
        d3_ForEachObject(d3_DefaultScene, obj, n->mask | 0x280, (D3OBJECTCB)d3q_Remove, n);
}
