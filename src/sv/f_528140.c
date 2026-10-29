/* 0x528140, moves the camera (0x411680) to the stored view (0x75fb80 position, 0x75fb90 direction) with the step 0x623814 / 8 */

void sv_Unk528140(void)
{
    d3t_PlaceObject(ai_Unkfc2dec, *(float *)&ai_Unk75fb80, *(float *)&ai_Unk75fb84, *(float *)&ai_Unk75fb88, *(float *)&ai_Unk75fb90, *(float *)&ai_Unk75fb94, *(float *)&ai_Unk75fb98, ai_CameraStep, 8);
}
