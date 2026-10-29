/* 0x40da10, deletes *obj (d3if_Delete); a NULL object is only a script error, so 1 is returned either way */

int ocmd_KillObject(D3OBJECT **obj)
{
    if (*obj) {
        d3if_Delete(*obj);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_KillObject: Could not find object to kill...");
    return 1;
}
