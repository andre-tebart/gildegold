
float d3fl_Shade[1024];  /* 0xfb9940, sun shade by -cos(angle) * 1023 */
unsigned long d3fl_ColorTab[3][256];  /* 0xfbac60, 8-bit channel -> 16-bit pixel bits */
D3VECTOR4 d3fl_LightCol;  /* 0xfbb860, color per light-map unit */
unsigned int d3fl_TileN;  /* 0xfbb870, pixels per tile in the texture being drawn */
int d3fl_DstPitch;  /* 0xfbb874, bytes per texture row */
D3VECTOR4 d3fl_SunCol;  /* 0xfbb880, added where the light map has bit 7 */
float d3fl_UV[2][8];  /* 0xfbb8a0 */
D3VECTOR4 d3fl_AxisZ;  /* 0xfbb8e0, world step per map y */
unsigned long d3fl_W16[16 * 16];  /* 0xfbb900 */
D3VECTOR4 d3fl_AxisY;  /* 0xfbbd00, world step per height unit */
int d3fl_SrcPitch;  /* 0xfbbd10, source bytes per row step */
D3VECTOR4 d3fl_Ambient;  /* 0xfbbd20 */
D3VECTOR4 d3fl_Origin;  /* 0xfbbd30, map point (0, 0, 0) in world space */
unsigned long d3fl_W32[32 * 32];  /* 0xfbbd40 */
unsigned long d3fl_W64[64 * 64];  /* 0xfbcd40, blend weights for 64 pixels per tile */
int d3fl_SrcStep;  /* 0xfc0d40, source bytes per pixel step */
D3VECTOR4 d3fl_AxisX;  /* 0xfc0d50, world step per map x */
float d3fl_Jitter[1536];  /* 0xfc0d60 */
int d3fl_WPitch;  /* 0xfc2560, weight bytes per row step */
const char d3fl_Suffix[3][11] = { "", "_high_1", "_high_2" };  /* 0x5ebaa8, "", "_high_1", "_high_2": source texture mip levels */
D3FLBLEND d3fl_Blend = d3fl_BlendC;  /* 0x5fb970 */
D3BOXVERT d3fl_Box[8] = {0};  /* 0x6334b8, result of d3fl_TileBox */
char d3fl_Mips = 0;  /* 0x63373c */
char d3fl_MapShift = 0;  /* 0x63373d, texture shift copied into D3FLMAP.shift (d3fl_Init argument) */
float d3fl_LightMargin = 0;  /* 0x633740, world units added to map->scale when a light's
                                             ground radius is converted to map cells (d3fl_LightCB; no writer) */
unsigned int d3fl_Count = 0;  /* 0x633744 */
D3FLCACHE * d3fl_Cache = 0;  /* 0x63374c */
unsigned char d3fl_DetailShift = 0;  /* 0x633750, 6 - log2(detail scale): */
unsigned char ai_Unk633751 = 0;  /* 0x633751, the byte right after d3fl_DetailShift (f3init.c's detail shift) */
D3VECTOR d3_Unk633b1c = {0};  /* 0x633b1c, point light color */
