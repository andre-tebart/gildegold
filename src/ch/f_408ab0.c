/* 0x408ab0, plays a step sound for the ground type when the walk animation reaches a step frame */

void ch_WalkSound(CHACTION *a, int ground)
{
    CHARACTER *ch = a->ch;
    void *snd;
    char name[0x80];

    if (ch->slot && ch_SndLib && d3_DefaultScene == ch->scene && ch_CurrentGroup == ch->group
        && ch->slot->frame != a->u.walk.lastframe) {
        if (ch->slot->frame == 4 || ch->slot->frame == 12 || ch->slot->frame == 18
            || ch->slot->frame == 24) {
            if ((ch->kind & 1) && ch_Unk631060 < 3 && d3t_SceneIndex(ch->scene)) {
                if (ch->scene->map) {
                    strcpy(name, "Normal_s");
                    if (ground == 6 || ground == 11)
                        strcpy(name, "Stein_s");
                    else if (ground == 3)
                        strcpy(name, "Erde_s");
                    else if (ground == 4)
                        strcpy(name, "Wiese_s");
                    else if (ground == 8)
                        strcpy(name, "Stein_s");
                    else if (ground == 10)
                        strcpy(name, "Pfuetze_s");
                    snd = sb_GetSample(ch_SndLib, name);
                } else
                    snd = sb_GetSample(ch_SndLib, "normal_s");
                if (snd) {
                    d3snd_PlayAt(snd, ch->obj, 40, 500.0f);
                    ch_Unk631060++;
                }
            }
            a->u.walk.lastframe = ch->slot->frame;
        }
    }
}
