/* 0x40ef40, sets the two transient bits of *obj's stateFlags from *mode; a script error (0) for a NULL object,
   else 1 */

int ocmd_SetTransient(D3OBJECT **obj, int *mode)
{
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetTransient: Illegal object...");
        return 0;
    }
    ((OCMDOBJBITS *)&(*obj)->stateFlags)->transient = *mode;
    return 1;
}
