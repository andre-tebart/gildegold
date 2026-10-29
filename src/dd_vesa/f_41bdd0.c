/* 0x41bdd0, restores all lost surfaces; nonzero on success (or when there is nothing to restore) */

HWND dd_DevCombo = 0;  /* 0x6311f8 */
HWND dd_DrvCombo = 0;  /* 0x6311fc */
int dd_LastDriver = -1;  /* 0x5f3c48 */
LPDIRECTDRAW dd_DDBase = 0;  /* 0x631200 */
LPDIRECTDRAW4 dd_DD = 0;  /* 0x631204 */
LPDIRECTDRAWSURFACE4 dd_BackBuffer = 0;  /* 0x631208 */
int dd_Page = 0;  /* 0x63120c */
LPDIRECTDRAWSURFACE4 dd_FrontBuffer = 0;  /* 0x631210 */
LPDIRECTDRAWCLIPPER lpClipper = 0;  /* 0x631214 */
LPDIRECT3D3 dd_D3D3 = 0;  /* 0x631218 */
int dd_BitPlanes = 0;  /* 0x63121c */
char dd_HasMMX = 0;  /* 0x631220 */
char dd_TripleBuffer = 0;  /* 0x631221 */
char dd_Cleared = 0;  /* 0x631222 */
char dd_ZCleared = 0;  /* 0x631223 */
char dd_DlgFlag = 0;  /* 0x631224 */
char dd_Has800x600 = 0;  /* 0x631225 */
char dd_Has1024x768 = 0;  /* 0x631226 */
char dd_Has1152x864 = 0;  /* 0x631227 */

/* txt_DlgTitle: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DlgLabel104: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DlgLabel105: view of txt_Ptrs (build/mk_txt_views.py) */
int dd_BytesPerPixel;  /* 0x1463568 */
ULONG dd_Ref = {0}; /* 0x146356c */
unsigned char dd_RBits = {0}; /* 0x1463570 */
HWND dd_Window;  /* 0x1463574 */
unsigned char dd_GShift;  /* 0x1463578 */
unsigned char dd_RShift;  /* 0x1463579 */
int dd_LockPitch;  /* 0x146357c */
unsigned char dd_GBits = {0}; /* 0x1463580 */
/* dd_GLoss: view of dd_GBits (tools/data_views.py) */
void *dd_MemBuff;  /* 0x1463584 */
int dd_Pitch = {0}; /* 0x1463588 */
int dd_MemSize;  /* 0x146358c */
void *dd_Surface = {0}; /* 0x1463590 */
int dd_Height;  /* 0x1463594 */
int dd_Width;  /* 0x1463598 */
unsigned int dd_BMask;  /* 0x146359c */
void *dd_MemBottom;  /* 0x14635a0 */
int dd_PixelFormat;  /* 0x14635a4 */
unsigned int dd_Unk14635a8;  /* 0x14635a8 */
int dd_Mode;  /* 0x14635ac */
int dd_NumDevices;  /* 0x14635b0 */
RECT dd_ClipRect;  /* 0x14635c0 */
D3DDEVINFO d3d_Devices[8] = {0}; /* 0x14635e0 */
int dd_LockBytes;  /* 0x1464fa0 */
unsigned int dd_RMask;  /* 0x1464fa4 */
int d3d_NumDevices = {0}; /* 0x1464fa8 */
int dd_MemPitch;  /* 0x1464fac */
unsigned int dd_GMask;  /* 0x1464fb0 */
int dd_LockBits;  /* 0x1464fb4 */
unsigned int dd_Unk1464fb8;  /* 0x1464fb8 */
unsigned char dd_BShift;  /* 0x1464fbc */
unsigned char dd_BBits = {0}; /* 0x1464fbd */
/* dd_Unk1464fc0: view of dd_BBits (tools/data_views.py) */
unsigned char dd_BLoss;  /* 0x1464fc4 */
HBITMAP dd_Bitmap;  /* 0x14650e0 */
unsigned char dd_RLoss;  /* 0x14650e4 */
DDDEVICE dd_Devices[8];  /* 0x1465100 */
char dd_Unk1464fe0[0x100];  /* 0x1464fe0 */
int dd_PitchPixels = {0}; /* 0x14661a0 */
long d3d_hr;  /* 0x14661a4, result of the last checked D3D call */

char dd_Restore(void)
{
    if (dd_Mode && dd_DD)
        return dd_DD->lpVtbl->RestoreAllSurfaces(dd_DD) == DD_OK;
    return 1;
}
