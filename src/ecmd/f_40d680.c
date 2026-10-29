/* 0x40d680 */
int ocmd_GetObjectHandle(char **name)
{
    char msg[0x200];
    D3OBJECT *obj;

    obj = d3_FindObject(0, 0x140, 0, *name);
    if (!obj) {
        sprintf(msg, "GetObjectHandle: Could not find object '%s'", *name);
        evt_Error(evt_Cur, evt_Cur->pc, msg);
        return 0;
    }
    return (int)obj;
}
