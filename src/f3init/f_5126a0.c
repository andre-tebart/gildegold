/* 0x5126a0, the scene callback for the "sfx_" objects (a function start Ghidra missed): starts the sound
   named after the prefix (the name's "_l" / "_r" suffixes decide on looping and a 2000 ms delay) at the
   object's position (d3snd_Attach, volume 0x7f, range the object's sound radius + 50) when sound is on
   (snd_SfxEnabled), then clears the object's state (0x224)
   @deep behaviour: 8 code hunks / 12 of 224 bytes, meaning read whole (the sfx_ prefix, the strcpy of the rest, the _l / _r loop and delay, the sound-on gate, the sample lookup, the attach with the volume 0x7f and the radius +50, the play with loop/delay, the mask clear) - no meaning difference left. This session: Q1 init_SoundObjectHook, Q2 through the canonical names of three callees (d3snd_Attach -> d3snd_Attach, d3snd_Play -> d3snd_Play, sb_GetSample -> sb_GetSample, from d3_snd.c / f3snd.h), Q7 with their prototypes, Q4 through the new SCENEOBJ.soundRange (0x68) and SCENEOBJ.mask (0x224) - 48 -> 8 code hunks. Left (allocation): the frame is 0x108 against the original's 0x104 (one local too many on the stack, name at the same [esp+0x1c]); the original keeps loop in edi where ours uses esi and the sample pointer in esi where ours uses edi (the two are swapped through the three calls). */


void init_SoundObjectHook(char *o)
{
    char name[0x104];
    int loop, delay, h;
    struct SBSAMPLE *s;

    if (strncmp(o, "sfx_", 4) == 0) {
        strcpy(name, o + 4);
        loop = strstr(name, "_l") == 0;
        if (strstr(name, "_r") != 0) {
            delay = 2000;
            loop = 0;
        } else
            delay = 0;
        if (snd_SfxEnabled != 0) {
            s = sb_GetSample(0, name);
            if (s != 0) {
                h = d3snd_Attach((int)s, o, 0x7f, (float)((int)((SCENEOBJ *)o)->soundRange + 0x32));
                if (h != 0)
                    d3snd_Play((struct D3SOUND *)h, 0, loop, delay);
            }
        }
        ((SCENEOBJ *)o)->mask = 0;
    }
}
