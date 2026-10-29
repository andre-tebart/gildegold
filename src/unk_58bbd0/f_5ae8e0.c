/* 0x5ae8e0, whether a transport order belongs to its owner (192 B): the ship / record +0x3b, the owner's
   scene object +0x61 of param_1 (d3_PointToWorld and d3_VecEqual within 1350.0 against the record's +0x34
   object, then its +0x128 field); else the id chain from the +6 field of param_2 is resolved by 0x4fea50
   (out: character, building, object): a character answers whether it is param_1's original record, a
   building follows its own +6, a third object ends it */

int ai_Unk5ae8e0(int param_1, int param_2)
{
    float v[4];
    int rec;
    int obj;
    int id;
    int out;
    int first;

    rec = *(int *)(param_2 + 0x3b);
    if (rec == 0)
        return 0;
    first = param_1;
    obj = *(int *)(first + 0x61);
    if (obj != 0) {
        d3_PointToWorld(obj, obj + 0x4c, v);
        if (d3_VecEqual((struct D3VECTOR *)(*(int *)(rec + 0x34) + 0x4c), (struct D3VECTOR *)v,
                         1350.0f) != 0) {
            if (*(int *)(rec + 0x128) == 0)
                return 1;
        }
    }
    id = *(int *)(param_2 + 6);
    while (id != -1) {
        if (gm_FindById(&param_2, &param_1, &out, id) == 0)
            return 0;
        if (param_2 != 0)
            return param_2 == first;
        if (param_1 != 0)
            id = *(int *)(param_1 + 6);
        if (out != 0)
            return 0;
    }
    return 0;
}
