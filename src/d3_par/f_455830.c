/* 0x455830, allocates a particle system for count particles with texture tex, initializes every particle
   and appends the system to the default scene's list (d3_ParTail / d3_ParEnd). Returns 0 when size or
   count is not positive or the texture cannot be loaded. */

/* particle at byte offset j (the original walks the array with an explicit offset, do-while loop) */
#define PAR_AT(ps, j) ((D3PARTICLE *)((char *)(ps)->particles + (j)))

D3PARTSYS *par_Create(int count, char *tex, D3TEXFLAGS flags, float size, void *parent, D3PARTCB cb)
{
    D3PARTSYS *ps = 0;
    D3TEXHANDLE *h;
    int i;
    unsigned long now;
    unsigned int j;

    if (size > 0.0 && (unsigned)count > 0) {
        h = tex_Load(tex, flags, 0, 0);
        if (h) {
            tex_Reload(h, 0);
            ps = mem_Alloc(0x31c, "d3_par:ParticleSystem");
            ps->tex = h;
            ps->texindex = ((char *)h - (char *)d3_Textures) >> 7;
            ps->count = count;
            ps->size = size;
            ps->callback = cb;
            ps->particles = mem_Alloc(count * 0x54, "d3_par:Particles");
            ps->points = mem_Alloc(ps->count * 0x140, "d3_par:ParticlePoints");
            ps->polys = mem_Alloc(ps->count * 0x50, "d3_par:ParticlePolys");
            i = 0;
            ps->v00.x = 0;
            ps->v00.y = 0;
            ps->v00.z = 0;
            ps->v00.w = 0;
            ps->v10.x = 0;
            ps->v10.y = 0;
            ps->v10.z = 0;
            ps->v10.w = 0;
            D3PAR_TIME(ps)->lasttime = 0;
            ps->flags = 0;
            now = d3_Time;
            if (i < ps->count) {
                j = 0;
                do {
                    PAR_AT(ps, j)->pos.x = 0;
                    PAR_AT(ps, j)->pos.y = 0;
                    PAR_AT(ps, j)->pos.z = 0;
                    PAR_AT(ps, j)->vel.x = 0;
                    PAR_AT(ps, j)->vel.y = 0;
                    PAR_AT(ps, j)->vel.z = 0;
                    PAR_AT(ps, j)->time = now;
                    PAR_AT(ps, j)->size = ps->size;
                    PAR_AT(ps, j)->flags &= 0xfe;
                    PAR_AT(ps, j)->frame = 0;
                    PAR_AT(ps, j)->color = 0xffffffff;
                    i++;
                    j += 0x54;
                } while (i < ps->count);
            }
            *((unsigned char *)ps + 0x2fb) |= 8;
            d3_InitObject((D3OBJECT *)ps->obj);
            ps->parent = parent;
            ps->scene = d3_DefaultScene;
            ps->prev = d3_ParTail;
            ps->next = &d3_ParEnd;
            d3_ParTail->next = ps;
            d3_ParTail = ps;
        }
    }
    return ps;
}
