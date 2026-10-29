/* 0x417dd0, sets the hold flag: a held slot restarts instead of being unlinked when its sound stops; a new
   hold first starts the sound */

void d3snd_SetHold(D3SOUND *s, int hold)
{
    if (hold)
        d3snd_Play(s, 0, 0, 0);
    s->hold = hold;
}
