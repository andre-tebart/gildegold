/* 0x43b6b0, marks obj and its group in the default scene for an update (d3_MarkDirtyCB: state
   bit 2, and with full also the world matrix bit). */

void d3_UpdateObject(D3OBJECT *obj, unsigned char full)
{
    d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, (D3OBJECTCB)d3_MarkDirtyCB, (void *)full);
}
