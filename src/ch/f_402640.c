/* 0x402640, puts the character at the object's position, turned the way the object faces */

void ch_PlaceAtObject(CHARACTER *ch, D3OBJECT *obj)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;

    if (ch) {
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(obj, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot(ch->obj, &rot);
        ch_SetPos(ch, (D3VECTOR *)&pos, 1);
    }
}
