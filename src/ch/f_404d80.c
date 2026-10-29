/* 0x404d80 */

void ch_SetCamera(CHARACTER *ch, int view)
{
    D3VECTOR rot;
    D3VECTOR4 pos;

    ch_GetCamera(ch, view, &pos, &rot);
    d3_SetObjectRot(d3_Camera, &rot);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
}
