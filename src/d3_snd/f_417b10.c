/* 0x417b10, volume from the distance to the camera; opens the sound when it gets audible */

void d3snd_UpdateVolume(D3SOUND *s)
{
    D3VECTOR4 pos;
    float dx, dy, dz, dist, t;
    int lin, quad, vol;

    if (!s || !s->obj)
        return;
    d3_PointToWorld(s->obj, &s->obj->pos, (D3VECTOR *)&pos);
    dx = pos.x - d3_Camera->pos.x;
    dy = pos.y - d3_Camera->pos.y;
    dz = pos.z - d3_Camera->pos.z;
    dist = sqrt(dx * dx + dy * dy + dz * dz);
    dist = dist * D3SND_INCH;
    t = s->range - dist;
    if (t > 0.0f) {
        lin = (int)(t / s->range * s->volume);
        quad = (int)(s->volume - s->volume * dist * dist / (s->range * s->range));
        vol = (int)((quad + (lin > 0 ? lin : 0)) * 0.5f);
    } else
        vol = 0;
    if (vol > 5) {
        if (s->snd) {
            sb_SetVolume(s->snd, vol);
            if (sb_IsPlaying(s->snd) || s->loops)
                return;
            s->idle = 0;
            if (s->delay)
                s->snd->flags &= ~8;
            else
                d3snd_Play(s, 0, 0, 0);
        } else {
            s->snd = (SNDHANDLE *)sb_GetSample(0, s->name);
            if (!s->snd)
                return;
            sb_SetRestartC((char *)s->snd, 1);
            s->snd->loops = s->loops;
            s->snd->delay = s->delay;
            sb_SetVolume(s->snd, vol);
            sb_SetLoops((struct SBSAMPLE *)s->snd,s->loops);
            d3snd_Play(s, 0, s->loops, s->delay);
        }
    } else if (s->snd) {
        sb_SetVolume(s->snd, vol);
        sb_Stop(s->snd, 0);
        if (!s->hold) {
            sb_SetRestartC((char *)s->snd, 0);
            s->snd = NULL;
        }
    }
}
