/* 0x411680, starts an animation that moves the object to pos and turns it to dir (angles). @deep B: read the whole 137-line asmdiff against 0x411680..0x411860: the same statements with another register allocation and scheduling - the three af flag byte updates (|= 0x14, &= ~8, |= 2) are interleaved differently, the d3_UnwrapAngles argument setup and the D3ANIMKEY copies use other registers/slots (key[0].time and the two vec copies swapped), the FPU projection/relative-vector sequence has another stack order; no different constant, callee, condition or field. Readability Q2 d3_Unk1466620/d3_Unk1466630 (globals) and d3anim_CreatePath (the animation creator) is deep work. Needs vargen/try.py */
int d3t_PlaceObject(D3OBJECT *camera, float px, float py, float pz,
                    float dx, float dy, float dz, int unk, char flags)
{
    D3ANIMKEY keys[2];
    D3VECTOR4 rel;
    D3VECTOR rot;
    D3ANIMFLAGS af;
    D3VECTOR dir;
    D3ANIM *anim;

    if (d3t_CamMoving && d3_Camera == camera)
        return 0;
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[1] &= ~8;
    af.b[8] |= 2;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    dir.y = dy;
    dir.z = dz;
    dir.x = dx;
    if (flags & 8) {
        d3_UnwrapAngles(&camera->rotation, &dir, &rot, &dir);
        d3_SetObjectRot(camera, &rot);
    }
    if (camera == d3_Camera) {
        d3_Unk1466630 = dir;
        d3_Unk1466620.x = px;
        d3_Unk1466620.y = py;
        d3_Unk1466620.z = pz;
    }
    rel.x = px - camera->pos.x;
    rel.y = py - camera->pos.y;
    rel.z = pz - camera->pos.z;
    keys[0].time = unk;
    keys[1].rot.x = (dir.x -= camera->rotation.x);
    keys[1].rot.y = (dir.y -= camera->rotation.y);
    dir.z -= camera->rotation.z;
    keys[1].pos.x = rel.x;
    keys[1].pos.y = rel.y;
    keys[1].pos.z = rel.z;
    keys[1].rot.z = dir.z;
    anim = d3anim_CreatePath(camera, af, 2, keys);
    if (camera == d3_Camera) {
        d3t_CamMoving = 1;
        d3t_CamMove = anim;
        anim->flags2d |= 8;
    }
    return 1;
}
