/* 0x43e4d0, animates the objects of the default scene (and of all other scenes) @deep one code hunk at 0x43e5c9 (6 vs 6 instructions, same ops up to the temp register): original keeps the 0x6334a8 temp in esi and reuses esi for tb_Delay, ours loads tb_Delay into esi early and uses edi for the temp with inc/sub scheduled after - register allocation only, no missing/extra statement, constant, callee, type or field in the 96 instructions; statement orders fr,n,dt / n,fr,dt / fr,dt,n / dt,n,fr and a no-fr spelling (tb_Delay in both rates) tried, all worse or equal; Q2 cleared by the renames */

void d3_Animate(unsigned int mask, char anim, char tick, char all)
{
    unsigned long now;
    unsigned int flags, n, dt, fr;
    int i;
    D3OBJECT *o;

    if (d3_Busy < 0)
        d3_Busy = 0;
    if (d3_EngineReady && !d3_Busy && d3_Camera) {
        d3_Busy = 1;
        if (anim) {
            now = d3_Time;
            ts_UpdateScroll(now);
            flags = mask | 0x181;
            d3_ForEachObject(d3_DefaultScene, 0, flags, (D3OBJECTCB)d3anim_StepObject, (void *)now);
            if (all) {
                for (i = 0; i < D3_MAX_SCENES; i++) {
                    if (i != d3_CurrentScene && (o = ((D3SCENELISTS *)&d3_Scenes[i])->first) && o != &d3_ObjEnd) {
                        do {
                            d3_ForEachObject(d3_DefaultScene, o, flags, (D3OBJECTCB)d3anim_StepObject, (void *)(now | 0x80000000));
                            o = o->next;
                        } while (o != &d3_ObjEnd);
                    }
                }
            }
            fr = tb_Delay;
            n = ++d3_Fps60Frames;
            dt = now - d3_Fps60Time;
            if (dt > 60 && dt > 0) {
                d3_Fps60Frames = 0;
                d3_Fps60Time = now;
                d3_Fps60 = n * 1000 / (dt * fr);
            }
            n = ++d3_Fps10Frames;
            dt = now - d3_Fps10Time;
            if (dt > 10 && dt > 0) {
                d3_Fps10Frames = 0;
                d3_Fps10Time = now;
                d3_Fps10 = n * 1000 / (dt * fr);
            }
        }
        d3_EndFrame(tick);
        d3_Busy--;
    }
}
