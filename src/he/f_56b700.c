/* 0x56b700, an object callback of the event (o: +0x4c the position, +0x84 .. +0x8c a target): places the object at the target lifted by 1.47 (0x411860 (o, pos, target, 0x46, 0x20)) and counts +0xd0 down */

void he_Unk56b700(int o)
{
    float t[3];

    t[0] = *(float *)(o + 0x84) + 1.47;
    t[1] = *(float *)(o + 0x88);
    t[2] = *(float *)(o + 0x8c);
    d3t_PlaceObjectV(o, o + 0x4c, t, 0x46, 0x20);
    *(unsigned char *)(o + 0xd0) -= 1;
}
