/* 0x40d860, returns the first child of *parent whose name is *name; a script error and 0 when there is none */

int ocmd_GetSubObjectHandle(D3OBJECT **parent, char **name)
{
    char msg[0x200];
    D3OBJECT *obj;

    obj = d3_FindObject((int)*parent, 0x140, 0, *name);
    if (!obj) {
        sprintf(msg, "GetSubObjectHandle: Could not find object '%s'", *name);
        evt_Error(evt_Cur, evt_Cur->pc, msg);
    }
    return (int)obj;
}
