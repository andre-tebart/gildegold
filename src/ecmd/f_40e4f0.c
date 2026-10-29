/* 0x40e4f0, releases the animation textures of every object whose name contains *pattern; 0 */

int ocmd_DetachAnimFromAll(char **pattern)
{
    OCMDANIM a;

    strcpy(a.pattern, *pattern);
    strcpy(a.file, "");
    a.n = 0;
    a.rnd = 0;
    if (a.pattern[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, (D3OBJECTCB)ocmd_DetachAnimCB, &a);
    return 0;
}
