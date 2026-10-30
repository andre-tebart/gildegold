/* 0x449960, makes obj a child of parent (0: top level) keeping its world position; light keys too */

void err_Message(char *msg);                                                                     /* 0x4222b0 */
void d3_SetFog(float start, float end, D3FOGCOL color);  /* 0x43a6f0, d3_engine.c; @original: this unit passes the colour struct, d3_engine.c / the header take the dword */

char d3_SetParent(D3OBJECT *obj, D3OBJECT *parent)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3MATRIX m;
    D3VECTOR4 kpos[7];
    D3MATRIX km[7];
    D3OBJECT *c;
    int i;

    if (obj == parent || !obj)
        return 0;
    if (parent) {
        for (c = parent->child; c; c = c->next)
            if (c == obj)
                return 1;
        if (d3_FindObject((int)obj, 0x1ff, (int)parent, 0))
            return 0;
    }
    if (D3_X(obj)->type >= 5 && D3_X(obj)->light) {
        rot.x = D3_X(obj)->rot.x;
        rot.y = D3_X(obj)->rot.y;
        rot.z = D3_X(obj)->rot.z;
        for (i = 0; i < 7; i++) {
            d3_PointToWorld(obj, &D3O_LIGHT(obj)->k[i].pos, (D3VECTOR *)&kpos[i]);
            d3_SetObjectRot(obj, &D3O_LIGHT(obj)->k[i].rot);
            d3_WorldMatrixD(obj, 0, 1, &km[i]);
        }
        d3_SetObjectRot(obj, &rot);
    }
    d3_PointToWorld(obj, &D3_X(obj)->pos, (D3VECTOR *)&pos);
    d3_WorldMatrixD(obj, 0, 1, &m);
    d3_Unlink(obj);
    if (!parent)
        d3_LinkTop(obj);
    else
        d3_AddChild(parent, obj);
    d3_SetWorldPosRot(obj, &pos, &m);
    if (D3_X(obj)->type >= 5 && D3_X(obj)->light) {
        for (i = 0; i < 7; i++)
            d3_WorldToParent(obj, &kpos[i], &km[i], &D3O_LIGHT(obj)->k[i].pos, &D3O_LIGHT(obj)->k[i].rot);
    }
    return 1;
}
