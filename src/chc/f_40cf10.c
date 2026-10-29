/* 0x40cf10, SetCharacterToDummy(ch, dummy): puts the character at the dummy's world position, facing the dummy's direction; 1 after an error, else 0 */

int chc_SetCharacterToDummy(CHARACTER **ch, D3OBJECT **dummy)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;

    if (*ch && *dummy) {
        d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
        ch_SetPos(*ch, (D3VECTOR *)&pos, 1);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(*dummy, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot((*ch)->obj, &rot);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "SetCharacterToDummy(): invalid character or dummy");
    return 1;
}
