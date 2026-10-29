/* 0x4591a0 @deep Q4: the UNK64 macro (a float view over the int at 0x64) and the ((float *)s)[25] read of the same slot, needs the slot struct typed; behaviour byte */

/* the other files see D3OBJECT / D3LINFO */
#define D3_ENVMAPCOORDS(o, p) d3_EnvMapCoords((XOBJ *)(o), (XPART *)(p))

/* 0x4591a0, blend weights of the two key frames around the current time of an animation slot */
#define UNK64(s) (*(float *)&(s)->time)

void d3_AnimBlendWeights(D3ANIMSLOT *s, float *a, float *b)
{
    D3ANIMDATA *d = s->data;
    int n;
    float t;
    volatile float r;

    if (s->flags.b[1] & 2) {
        t = s->subFrame * (1.0f / d->frames[s->nextFrame].count);
        if (s->flags.b[1] & 4) {
            n = d->nframes;
            if (n <= 2 && !(0 < s->nextFrame)) {
                t = ((float)cos(t * 3.1415927f) + 1.0f) * 0.5f;
                t = 1.0f - t;
            } else if (!(0 < s->nextFrame)) {
                t = (float)cos(t * 1.5707964f);
                t = 1.0f - t;
            } else if (s->frame >= n - 1) {
                t = (float)cos((t + 1.0f) * 1.5707964f) + 1.0f;
                t = 1.0f - t;
            }
        }
        r = t - (1.0f / d->frames[s->nextFrame].count) * ((float *)s)[25];
        /* the non-volatile read is needed: it makes VC6 load 1.0f first (fld 1.0f; fsub [r]) */
        *a = 1.0f - *(float *)&r;
        *b = r;
    } else {
        t = s->subFrame * (1.0f / d->frames[s->frame].count);
        if (s->flags.b[1] & 4) {
            n = d->nframes;
            if (n <= 2 && s->frame + 1 >= n - 1) {
                t = ((float)cos(t * 3.1415927f) + 1.0f) * 0.5f;
                t = 1.0f - t;
            } else if (s->nextFrame >= n - 1) {
                t = (float)cos((t + 1.0f) * 1.5707964f) + 1.0f;
                t = 1.0f - t;
            } else if (!(0 < s->frame)) {
                t = (float)cos(t * 1.5707964f);
                t = 1.0f - t;
            }
        }
        t = (1.0f / d->frames[s->frame].count) * ((float *)s)[25] + t;
        r = 1.0f - t;
        *a = t;
        *b = r;
    }
}
