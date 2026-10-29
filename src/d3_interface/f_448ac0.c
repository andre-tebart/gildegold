/* 0x448ac0, the signed horizontal angle between the facing of `obj` (its z axis) and the direction to the
   point `p`, around the vertical axis */

/* 0x448ac0..0x44aa10: d3_obj.c until layout 2026-10-22 (object helpers, hierarchy, light / fog sets, scene
   writer wr_object); joined: its .data (d3o_LightSet / d3o_FogSet = -1 at 0x5fbcfc, then "Unknown Object-Type in
   wr_object()!" 0x5fbe8c) frames d3_interface.c's literals 0x5fbd08..0x5fbe80, one block. */

float d3_YawTo(D3OBJECT *obj, float *p)
{
    D3VECTOR4 a, b, pos;
    float d, e, ang, t;
    float sn, cs, u;

    d3_LocalToWorld(obj, &d3_ZAxis, (D3VECTOR *)&a);
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    a.x = a.x - pos.x;
    a.y = 0;
    b.y = 0;
    a.z = a.z - pos.z;
    b.x = p[0] - pos.x;
    b.z = p[2] - pos.z;
    d3_Normalize(&b);
    d3_Normalize(&a);
    d = a.x * b.x + a.y * b.y + a.z * b.z;
    e = min(max(-1.0f, d), 1.0f);
    ang = (float)acos(e);
    cs = cos(1.5707964f);
    sn = sin(1.5707964f);
    t = cs * a.x + sn * a.z;
    u = cs * a.z - sn * a.x;
    if (t * b.x + b.z * u + a.y * b.y > 0.0f)
        ang = -ang;
    return ang;
}
