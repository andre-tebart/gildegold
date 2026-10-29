/* 0x40d940, creates the object *name at the world position of the dummy *dummy, with the dummy's rotation, and
   relights it; 0 when *dummy is NULL or the creation fails */

int ocmd_CreateObjectAtDummy(char **name, D3OBJECT **dummy)
{
    D3VECTOR4 pos;
    D3OBJECT *obj;

    if (!*dummy)
        return 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    obj = d3_CreateObject(0, (D3VECTOR *)&pos, &(*dummy)->rotation, *name);
    if (obj)
        d3_RelightObject(obj);
    return (int)obj;
}
