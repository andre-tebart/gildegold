/* 0x4179f0, pan from the angle between the camera's z axis and the direction to the object */

void d3snd_UpdatePan(D3SOUND *s)
{
    D3VECTOR4 pos;
    D3VECTOR4 dir;
    D3VECTOR4 rel;
    D3MATRIX m;

    if (s && s->obj && s->snd) {
        d3_MatTranspose(&d3_Camera->matrix, &m);
        dir.x = d3_ZAxis.x * m._11 + d3_ZAxis.y * m._21 + d3_ZAxis.z * m._31;
        dir.y = d3_ZAxis.x * m._12 + d3_ZAxis.y * m._22 + d3_ZAxis.z * m._32;
        dir.z = d3_ZAxis.x * m._13 + d3_ZAxis.y * m._23 + d3_ZAxis.z * m._33;
        d3_PointToWorld(s->obj, &s->obj->pos, (D3VECTOR *)&pos);
        rel.x = pos.x - d3_Camera->pos.x;
        rel.y = pos.y - d3_Camera->pos.y;
        rel.z = pos.z - d3_Camera->pos.z;
        sb_SetPan(s->snd, (int)(sin(d3_VecAngle((D3VECTOR *)&rel, (D3VECTOR *)&dir)) * 63.5 + 63.0));
    }
}
