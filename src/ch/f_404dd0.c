/* 0x404dd0 */

void ch_MoveCamera(CHARACTER *ch, int view, int time)
{
    D3VECTOR rot;
    D3VECTOR4 pos;

    if (ch) {
        ch_GetCamera(ch, view, &pos, &rot);
        d3anim_FreePath(d3_Camera);
        d3t_CamMoving = 0;
        d3t_CamMove = 0;
        d3t_PlaceObjectV(d3_Camera, (D3VECTOR *)&pos, &rot, time, 0x28);
    }
}
