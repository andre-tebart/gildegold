
int d3d_ZBufferCaps;  /* 0xfe7608, memory caps for the z buffer surface: */
D3DZBUFCTX d3d_UsedZBufferFormat;  /* 0xfe7620 */
D3DCONFIG d3d_Config = {0}; /* 0xfe7670 */
D3DCARD d3d_UsedCard;  /* 0xfe7680 */
D3DTLVERTEX d3d_PrecacheBuffer[3];  /* 0xfe7a20 */
DWORD d3d_MinMipSize = 4;  /* 0x5f942c, smallest mip dimension kept (4) */
float d3d_ViewW = -1.0f;  /* 0x5f9430, last viewport set, -1 = none */
float d3d_ViewH = -1.0f;  /* 0x5f9434 */
float d3d_ViewX = -1.0f;  /* 0x5f9438 */
float d3d_ViewY = -1.0f;  /* 0x5f943c */
float d3d_Near = -1.0f;  /* 0x5f9440, last projection set */
float d3d_Far = -1.0f;  /* 0x5f9444 */
unsigned char d3d_zbuffer = 1;  /* 0x5f9448, z buffer enabled (render state cache) */
void * d3d_VertexBuffer = 0;  /* 0x633410 */
LPDIRECT3D3 d3d_D3D = {0};  /* 0x633414 */
struct IDirect3DDevice3 * d3d_Device = 0;  /* 0x633418 */
LPDIRECT3DVIEWPORT3 d3d_Viewport = {0};  /* 0x63341c */
LPDIRECTDRAWSURFACE4 d3d_ZBuffer = {0};  /* 0x633420 */
/* d3d_ZEnabled: view of d3_MipSkip (tools/data_views.py) */
/* d3d_balpha: view of d3_MipSkip (tools/data_views.py) */
/* d3d_glow: view of d3_MipSkip (tools/data_views.py) */
unsigned char d3d_colorkey = 0;  /* 0x633430 */
unsigned char d3d_alphatest;  /* 0x633431 */
unsigned char d3d_fog = 0;  /* 0x633432 */
char d3d_StatesSet = 0;  /* 0x633433 */
char d3d_Mipmaps = 0;  /* 0x633434, mipmapping usable: d3d_UsedCard.info.maxMipFilter > 1 */
