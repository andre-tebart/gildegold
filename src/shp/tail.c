#pragma optimize("", on)

/* d2_ShpLightTable: view of d2_ClipTop (tools/data_views.py) */
unsigned int d2_LastAnimTick = 0;  /* 0x6f0464, tick of the last animation step */
int d2_ShpX = 0;  /* 0xf8a108 */
int d2_ShpY = 0;  /* 0xf8a10c */
int d2_ShpRowX = 0;  /* 0xf8a110 */
int d2_ShpRowGap = 0;  /* 0xf8a114 */
unsigned short d2_HighBitMask;  /* 0xf8a11c, mask of the half dark colours */
unsigned char d2_Palette[256][4] = {0};  /* 0xf8a120, palette */
D2ANIM d2_Anims[16] = {0};  /* 0xf8a520 */
int d2_LightTableExternal = 0;  /* 0xf8a630 */
unsigned int d2_Mode5Colour = 0;  /* 0xf8a638, colour twice, 16 bit */
unsigned int d2_BlitH = 0;  /* 0xf8a63c */
unsigned int d2_HighBitMask32 = 0;  /* 0xf8a640 */
unsigned char d2_BlitN = 0;  /* 0xf8a648 */
D2KEY d2_EndMarkerColour = { 0, 255 };  /* 0x5fed74 */
D2KEY d2_StartMarkerColour = { 0xff, 0 };  /* 0x5fed78 */
int d2_SurfPitch = 800;  /* 0x5fed84 */
