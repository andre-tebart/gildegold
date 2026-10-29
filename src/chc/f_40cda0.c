/* 0x40cda0, LookAtCharacter(ch, target): turns the character towards the target character (yaw from d3_YawTo, unwrapped against its current rotation) through action 7; 1 after an error, else 0 */

int chc_LookAtCharacter(CHARACTER **ch, CHARACTER **target)
{
    D3VECTOR rot;
    D3VECTOR b;
    D3VECTOR a;
    float angle;

    if (*ch && *target) {
        angle = d3_YawTo((*ch)->obj, &(*target)->obj->pos);
        rot.x = 0.0f;
        rot.y = angle;
        rot.z = 0.0f;
        d3_UnwrapAngles(&rot, &(*ch)->obj->rotation, &a, &b);
        rot.x += (*ch)->obj->rotation.x;
        rot.y += (*ch)->obj->rotation.y;
        rot.z += (*ch)->obj->rotation.z;
        ch_AddAction(*ch, 7, (int)(angle * 57.295776f), 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "LookAtCharacter(): invalid dest- or targetcharacter");
    return 1;
}
