/* 0x4136d0, moves the camera along the n objects named in the varargs (an empty name is skipped)
   within time; each object marked by bit 0 of objectFlags contributes its position and angles as a key */

int d3t_MoveCameraPath(int time, int n, ...)
{
    int count = 0;
    D3OBJECT *objs[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   /* 12 zeros: the implicit rest is stored separately */
    D3ANIMKEY keys[64];
    char name[96];
    D3ANIMFLAGS af;
    D3OBJECT *o;
    int i, k, step;
    char **args = (char **)&n;

    if (d3t_CamMoving)
        return 0;
    D3_ZERO(keys, sizeof(keys));
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[8] |= 2;
    af.b[1] |= 8;
    for (i = 0, k = 0; i < n; i++) {
        strcpy(name, args[++k]);
        if (name[0]) {
            if (!(objs[count] = d3_FindObject(0, 0x160, 0, name)))
                return 0;
            count++;
        }
    }
    step = time / count;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    keys[0].time = step;
    k = 1;
    for (i = 0; i < count; i++) {
        if (o = objs[i]) {
            if (o->objectFlags & 1) {
                keys[k].pos.x = D3_POSE(o)->pos.x;
                keys[k].pos.y = D3_POSE(o)->pos.y;
                keys[k].pos.z = D3_POSE(o)->pos.z;
                keys[k].rot.x = D3_POSE(o)->rot.x;
                keys[k].rot.y = D3_POSE(o)->rot.y;
                keys[k].rot.z = D3_POSE(o)->rot.z;
                keys[k].pos.x -= d3_Camera->pos.x;
                keys[k].pos.y -= d3_Camera->pos.y;
                keys[k].pos.z -= d3_Camera->pos.z;
                keys[k].rot.x -= d3_Camera->rotation.x;
                keys[k].rot.y -= d3_Camera->rotation.y;
                keys[k].rot.z -= d3_Camera->rotation.z;
            }
            keys[k - 1].time = step;
            k++;
        }
    }
    keys[k].time = 0;
    d3t_CamMove = d3anim_CreatePath(d3_Camera, af, count + 1, keys);
    d3t_CamMoving = 1;
    return 1;
}
