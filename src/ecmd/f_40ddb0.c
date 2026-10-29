/* 0x40ddb0, reloads *obj from the model *name (d3if_Reload); 1, a NULL object is a script error */

int ocmd_ReplaceObject(D3OBJECT **obj, char **name)
{
    if (*obj) {
        d3if_Reload(*obj, *name);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_ReplaceObject: Could not find object to replace...");
    return 1;
}
