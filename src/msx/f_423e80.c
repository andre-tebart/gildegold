/* 0x423e80, body of the update thread (called with msx_Lock held about every 50 ms): for every playing
   track it follows the stream position, retires a finished stream and starts the queued track, ramps the
   fade-in / fade-out volume, then advances the master fade; no return value
   @deep B draft, 428/800 bytes: every code hunk is the loop IV base offset (ours &t->fadepos, original &t->volume, all field offsets +8), the same operations spelled differently. The Open work attempts (582 operand-order variants, pointer/for/do-while loop forms, dead volume loads at 6 positions, volume read first in both fade blocks) all left the base unchanged; this is a VC6 base-selection choice, not a meaning error. Q6 added. */

void msx_Update(void)
{
    MSXTRACK *t;
    int i, dur;
    unsigned int elapsed;

    if (msx_Driver) {
        for (i = 0; i < MSX_TRACKS; i++) {
            t = &msx_Tracks[i];
            if (!t->playing)
                continue;
            if (t->stream == 0) {
                t->playing = 0;
                if (t->next) {
                    if (t->next->stream) {
                        sfx_PauseStream(t->next->stream, 0);
                        t->next->playing = 1;
                    }
                    t->next->queued = 0;
                    t->next = NULL;
                }
                continue;
            }
            sfx_GetStreamPos(t->stream, &t->pos);
            if (t->bAmbient)
                continue;
            if (sfx_StreamStatus(t->stream) != 4 && sfx_StreamStatus(t->stream) != 0x10) {
                t->playing = 0;
                t->fademode = 0;
                sfx_CloseStream(t->stream);
                t->stream = 0;
                if (t->next && t->next->stream) {
                    sfx_PauseStream(t->next->stream, 0);
                    t->next->playing = 1;
                    t->next->queued = 0;
                    t->next = NULL;
                }
            }
            if (t->fademode == 1) {
                if (t->pos < t->fadepos)
                    t->pos = t->fadepos;
                if (t->fadevol)
                    dur = t->fadevol * msx_FadeTime / t->volume;
                else
                    dur = msx_FadeTime;
                if (dur == 0 || dur <= t->pos - t->fadepos) {
                    t->fademode = 0;
                    t->curvol = t->volume;
                } else {
                    t->curvol = (t->pos - t->fadepos) * t->volume / dur;
                    if (abs(t->volume - t->curvol) < 2) {
                        t->fademode = 0;
                        t->curvol = t->volume;
                    }
                }
                sfx_SetStreamVolume(t->stream, t->curvol);
            } else if (t->fademode == 2) {
                if (t->pos < t->fadepos)
                    t->pos = t->fadepos;
                dur = t->fadevol * msx_FadeTime / t->volume;
                if (dur == 0 || dur <= t->pos - t->fadepos) {
                    t->fademode = 0;
                    t->curvol = 0;
                    sfx_SetStreamVolume(t->stream, 0);
                    sfx_CloseStream(t->stream);
                    t->stream = 0;
                    t->playing = 0;
                    if (t->next) {
                        sfx_PauseStream(t->next->stream, 0);
                        t->next->playing = 1;
                        t->next->queued = 0;
                        t->next = NULL;
                    }
                } else {
                    t->curvol = (t->fadepos - t->pos + dur) * t->volume / dur;
                    if (t->curvol > 2)
                        sfx_SetStreamVolume(t->stream, t->curvol);
                    else {
                        t->fademode = 0;
                        t->curvol = 0;
                        sfx_SetStreamVolume(t->stream, 0);
                        sfx_CloseStream(t->stream);
                        t->stream = 0;
                        t->playing = 0;
                        if (t->next) {
                            sfx_PauseStream(t->next->stream, 0);
                            t->next->playing = 1;
                            t->next->queued = 0;
                            t->next = NULL;
                        }
                    }
                }
            }
        }
        if (msx_FadeTo >= 0.0f) {
            elapsed = MSX_TIME;
            elapsed -= msx_FadeStart;
            if (elapsed < msx_FadeDuration)
                msx_Fade = (msx_FadeTo - msx_FadeFrom) / msx_FadeDuration * elapsed + msx_FadeFrom;
            else {
                msx_Fade = msx_FadeTo;
                msx_FadeTo = -1.0f;
            }
            msx_SetVolume(msx_Volume);
        }
    }
}
