
char msx_Path[0x100];  /* 0x14611a0 */
MSXTRACK msx_Dummy;  /* 0x14612a0, returned when there is no driver */
CRITICAL_SECTION msx_Lock;  /* 0x14613e0 */
float msx_Fade = 1.0f;  /* 0x5f74a8, master volume factor 0..1 */
float msx_FadeFrom = -1.0f;  /* 0x5f74ac */
float msx_FadeTo = -1.0f;  /* 0x5f74b0, < 0 = no master fade running */
unsigned int msx_FadeDuration = 5000;  /* 0x5f74b4 */
int msx_Volume = 127;  /* 0x5f74b8 */
int msx_AmbVolume = 127;  /* 0x5f74bc */
int msx_FadeTime = 10000;  /* 0x5f74c0, track fade time */
LARGE_INTEGER msx_Freq = {0};  /* 0x6323a8 */
char msx_Msg[0x100] = {0};  /* 0x6323b0, debug message (not printed) */
char msx_VolMsg[0x100] = {0};  /* 0x6324b0 */
unsigned int msx_FadeStart = 0;  /* 0x6325b0 */
int msx_DelayPeak = 0;  /* 0x6325bc */
int msx_Quit = 0;  /* 0x6325c0, ends the update thread */
int msx_Driver = 0;  /* 0x6325c4, music */
int msx_AmbDriver = 0;  /* 0x6325c8, ambient */
MSXTRACK msx_Tracks[MSX_TRACKS] = {0};  /* 0x6325d0 */
