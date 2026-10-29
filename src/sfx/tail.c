
SFXDRIVER * sfx_Drivers[SFX_DRIVERS] = {0};  /* 0x6332f0 */
HSAMPLE * sfx_Samples[SFX_DRIVERS] = {0};  /* 0x633330 */
HSTREAM * sfx_Streams[SFX_DRIVERS] = {0};  /* 0x633370 */
int sfx_Initialized = 0;  /* 0x6333b0 */
int sfx_NumSamples = 0;  /* 0x6333b4, samples per driver (DIG_MIXER_CHANNELS) */
