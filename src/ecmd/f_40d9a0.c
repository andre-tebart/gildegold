/* 0x40d9a0, loads the group *name at the world position of the dummy *dummy with the dummy's angles and relights
   its objects; 0 when *dummy is NULL */

int ocmd_CreateObjectGroupAtDummy(char **name, D3OBJECT **dummy)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *obj;

    if (!*dummy)
        return 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    obj = d3_LoadGroup(*name, 0, 0);
    d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    d3_WorldAngles(*dummy, &rot);
    d3_SetObjectRot(obj, &rot);
    if (obj)
        d3_RelightObjects(obj);
    return (int)obj;
}
