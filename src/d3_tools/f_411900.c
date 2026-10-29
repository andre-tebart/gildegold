/* 0x411900, the first object of the default scene whose name matches, searched with mask 0x1e0 (flags bit 1) or 0x60 */

D3OBJECT *d3t_FindObject(char *name, char flags)
{
    d3t_FoundObject = 0;
    if (flags & 2)
        d3_ForEachObject(d3_DefaultScene, 0, 0x1e0, d3t_FindObjectCB, name);
    else
        d3_ForEachObject(d3_DefaultScene, 0, 0x60, d3t_FindObjectCB, name);
    return d3t_FoundObject;
}
