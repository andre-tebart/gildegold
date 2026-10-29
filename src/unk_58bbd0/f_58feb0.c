/* 0x58feb0, drops an object onto the height of a target (78 B): the position is set (0x43b790) and the object
   dropped (0x412a70); when its y is below the target's +4, the position is set again */

void ai_Unk58feb0(int obj, int target)
{
    float p[4];

    d3_SetObjectPos(obj, target);
    d3t_DropObject(obj, 1, 0);
    d3_PointToWorld(obj, obj + 0x4c, p);
    if (p[1] < *(float *)(target + 4))
        d3_SetObjectPos(obj, target);
}
