/* 0x40dab0, sets *obj's angles from *x / *y / *z in degrees (OCMD_DEG) and marks it to be rerotated (state |= 4);
   0, a NULL object is a script error */

int ocmd_SetAngle(D3OBJECT **obj, float *x, float *y, float *z)
{
    if (*obj) {
        (*obj)->rotation.x = *x * OCMD_DEG;
        (*obj)->rotation.y = *y * OCMD_DEG;
        (*obj)->rotation.z = *z * OCMD_DEG;
        (*obj)->state |= 4;
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_SetAngle: Could not find object to set the angle...");
    return 0;
}
