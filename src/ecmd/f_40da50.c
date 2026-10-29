/* 0x40da50, sets *obj's position from *x / *y / *z and updates it (d3_SetObjectPos); 0, a NULL object is a
   script error */

int ocmd_SetPos(D3OBJECT **obj, float *x, float *y, float *z)
{
    if (*obj) {
        (*obj)->pos.x = *x;
        (*obj)->pos.y = *y;
        (*obj)->pos.z = *z;
        d3_SetObjectPos(*obj, &(*obj)->pos);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_SetPos: Could not find object to set pos...");
    return 0;
}
