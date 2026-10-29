/* 0x43cb80 */

void d3_AddChild(D3OBJECT *parent, D3OBJECT *obj)
{
    D3_OBJBITS(obj).top = !parent;
    if (parent) {
        if (parent->child) {
            d3_AppendSibling(parent->child, obj);
            obj->parent = parent;
        } else {
            parent->child = obj;
            obj->parent = parent;
        }
    } else
        d3_LinkTop(obj);
}
