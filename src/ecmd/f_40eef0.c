/* 0x40eef0, sets / clears the blocked bit of *obj's stateFlags from *on; a script error (0) for a NULL object,
   else 1 */

int ocmd_SetBlocked(D3OBJECT **obj, int *on)
{
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetBlocked: Illegal object...");
        return 0;
    }
    ((OCMDOBJBITS *)&(*obj)->stateFlags)->blocked = *on;
    return 1;
}
