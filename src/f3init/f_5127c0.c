/* 0x5127c0, the sound hook of the "sfx_" scene objects: frees the sound at SCENEOBJ.sound (0x4179d0) */


void init_SoundObjectFreeHook(char *o)
{
    if (strncmp(o, "sfx_", 4) == 0 && ((SCENEOBJ *)o)->sound != 0)
        d3snd_Free(((SCENEOBJ *)o)->sound);
}
