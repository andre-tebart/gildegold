/* 0x417840, once per frame */

void d3snd_Update(void)
{
    int i;

    if (!d3snd_List)
        return;
    for (i = 0; i < d3snd_Max; i++) {
        if (!d3snd_List[i].obj)
            continue;
        if (d3snd_List[i].obj->scene != d3_DefaultScene) {
            if (d3snd_List[i].snd) {
                sb_Stop(d3snd_List[i].snd, 0);
                d3snd_List[i].snd->flags |= 8;
                if (!d3snd_List[i].hold) {
                    sb_SetRestart((char *)d3snd_List[i].snd, 0);
                    d3snd_List[i].snd = NULL;
                }
            }
        } else {
            d3snd_UpdateVolume(&d3snd_List[i]);
            d3snd_UpdatePan(&d3snd_List[i]);
            if (d3snd_List[i].snd && !sb_IsPlaying(d3snd_List[i].snd)) {
                d3snd_List[i].idle++;
                if (d3snd_List[i].idle > 500 && !d3snd_List[i].hold) {
                    if (d3snd_List[i].loops)
                        d3snd_Detach(&d3snd_List[i]);
                    else {
                        sb_SetRestart((char *)d3snd_List[i].snd, 0);
                        d3snd_List[i].snd->flags |= 8;
                        d3snd_List[i].snd = NULL;
                    }
                }
            }
        }
    }
}
