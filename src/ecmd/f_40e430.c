/* 0x40e430, attaches the animation *file, looped *n times, to every object whose name contains *pattern
   (ocmd_AttachAnimCB; *rnd starts each at a random frame); 0 */

int ocmd_AttachAnimLoopedToAll(char **pattern, char **file, int *n, int *rnd)
{
    OCMDANIM a;

    strcpy(a.pattern, *pattern);
    strcpy(a.file, *file);
    a.n = *n;
    a.rnd = *rnd;
    if (a.pattern[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)ocmd_AttachAnimCB, &a);
    return 0;
}
