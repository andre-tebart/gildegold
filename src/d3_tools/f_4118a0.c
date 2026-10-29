/* 0x4118a0, places the camera at the pose a zoom target object stores (its 0x5c position and 0x90 angles) */

int d3t_PlaceAtObject(D3OBJECT *obj, int unk)
{
    return d3t_PlaceObjectV(d3_Camera, &D3_POSE(obj)->pos, &D3_POSE(obj)->rot, unk, 8);
}
