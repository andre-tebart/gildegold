/* 0x425ce0, shuts the sample library down: unloads every sample bank, frees the sample slot array and
   closes the sound driver; side effects: the sb_ globals go back to their empty state. Safe to call twice. */

void sb_Exit(void)
{
    sb_UnloadAll();
    if (sb_Samples) {
        mem_Free(sb_Samples);
        sb_Samples = NULL;
    }
    if (sb_Driver) {
        sfx_CloseDigOutput(sb_Driver);
        sb_Driver = 0;
    }
}
