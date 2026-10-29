/* 0x453580, recomputes the vertex colours (d3_RelightObject) of the objects of obj's scene that d3_ForEachObject2 selects
   from obj with mask 0x240; nothing for a null obj */

void d3_RelightObjects(D3OBJECT *obj)
{
    if (obj)
        d3_ForEachObject2(obj->scene, obj, 0x240, (void (*)(D3OBJECT *))d3_RelightObject);
}
