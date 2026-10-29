/* 0x40ee10, puts the camera at the position and angles of the camera dummy *name (d3t_FindObject flag 2); a
   script error when there is no such dummy; 0
   @deep Q2 only left, byte 0x40ee10: the body reads dummy->unk05c as the camera position and dummy->unk090 as its angles (d3_SetObjectPos / d3_SetObjectRot of d3_Camera), while the same D3OBJECT slots are read as D3LOBJ.color / D3IFLIGHT.color (0x5c) and D3LOBJ.range/power/atten (0x90) by the light code (d3s.c 0x45b8d0, d3_engine.c d3_CloneObject, d3_interface.c scene serialization). A d3.h field rename is project-wide and the typed union view of D3OBJECT is the recorded codegen hazard of d3_CloneObject (notes/units/d3_engine.c), so the name needs the shared-header decision -> match-deep. Q6 description added; callers/callees read (d3t_FindObject flag 2, d3_SetObjectPos/Rot), no other evidence for a per-object role name. */

int ocmd_SetCameraToDummy(char **name)
{
    D3OBJECT *dummy;
    char msg[0x80];

    dummy = d3t_FindObject(*name, 2);
    if (dummy) {
        d3_SetObjectPos(d3_Camera, &dummy->unk05c);
        d3_SetObjectRot(d3_Camera, &dummy->unk090);
        return 0;
    }
    sprintf(msg, "SetCameraToDummy(): Could not find Cameradummy '%s'!", *name);
    evt_Error(evt_Cur, evt_Cur->pc, msg);
    return 0;
}
