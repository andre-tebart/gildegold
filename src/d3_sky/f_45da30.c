/* 0x45da30, allocates a sky (0xacc bytes, colour white), stores the horizon and fog parameters, initialises its vertex grid and returns it; 0 when the allocation fails @deep readability Q4: *(unsigned long *)&s->color = 0xffffffff needs the dword view of D3COLOR (b,g,r,a): the original is a single mov DWORD [s+0xaa4],0xffffffff, while four typed byte stores and memset(&s->color,0xff,4) both change its code (85 / 66 bytes), and an unsigned long member in D3COLOR raises its alignment from 1 to 4 (layout risk for every embedder); behaviour met */

D3SKY *d3sky_Create(float horizon, float fogstart, float fogscale)
{
    D3SKY *s = mem_Alloc(sizeof(D3SKY), "d3_sky:Create");

    if (s) {
        *(unsigned long *)&s->color = 0xffffffff;
        s->layers = 0;
        s->horizon = horizon;
        s->lasttime = d3_Time;
        s->dt = 0;
        s->fogstart = fogstart;
        s->fogscale = fogscale;
        d3sky_Init(s, 1);
    }
    return s;
}
