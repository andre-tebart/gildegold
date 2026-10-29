/* 0x5344f0, puts a person (the template model name n of the list 0x1440d80, index k) at the dummy "n" of the scene object named a: the dummy position (0x458a20), a character object (0x403390 (b, pos)) that
   looks along the dummy's heading (0x458810 / 0x4539f0, 0x43b860), walks ("bewegung/gehen", speed 0.7, 0x1a0), is registered as a status text plate (0x52e1c0); returns the new object or 0 */

int main_Unk5344f0(int a, int b, int k)
{
    int o, p;
    float dir[3], pos[4], loc[3];

    o = d3_FindObject(0, 0x100, 0, a);
    if (o == 0)
        return 0;
    d3_PointToWorld(o, o + 0x4c, pos);
    p = ch_Create(b, pos);
    if (p != 0) {
        dir[0] = 0;
        dir[1] = 0.0f;
        dir[2] = 0;
        d3_DirToWorld(o, (char *)(unsigned)&ai_Unk5ebb74, loc);
        dir[1] = d3_VecAngle((char *)(unsigned)&ai_Unk5ebb74, loc);
        d3_SetObjectRot(*(int *)(p + 0x34), dir);
        ch_PlaceOnGround(p, 0);
        *(int *)(*(int *)(p + 0x34) + 0x200) = p;
        d3_UpdateObject(*(int *)(p + 0x34), 1);
        ch_PreloadAnims(p, 1, "bewegung/gehen");
        *(int *)(*(int *)(p + 0x34) + 0x48) = 0;
        *(int *)(p + 0x1a0) = 0x3f333333;
        *(int *)(p + 0x23c) = -1;
        *(int *)(p + 0x2c) = 1;
        *(char *)(p + 0x240) = 4;
        sv_RegisterNamePlate(*(int *)(p + 0x34), U_I((unsigned)&full_data_1440d80 + k * 4), 0);
    }
    return p;
}
