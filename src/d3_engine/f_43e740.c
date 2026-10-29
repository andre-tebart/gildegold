/* 0x43e740, sets (fn != 0) or clears an event handler of an object */

D3EVRESOLVE d3e_Resolve = d3e_DefaultResolve;  /* 0x5fb920 */
D3PARTSYS d3_ParEnd;  /* 0xfb9280, particle system list end marker */
/* d3_ParTail: view of d3_ParEnd (tools/data_views.py) */
struct D3PARTSYS * d3_ParHead;  /* 0xfb98f4, particle list head (D3PARTSYS * in d3_par.h) */
/* d3_Unkfb9928: view of d3_Mirror (tools/data_views.py) */
char * d3_AnimPath = {0}; /* 0xfba940 */
float d3_ZFar;  /* 0xfc2564 */
char d3_Unkfc2580[0x168] = {0}; /* 0xfc2580, the animation list head sentinel (the next / prev fields at +0x160 / +0x164 are d3_AnimListHead / d3_Unkfc26e4, header macros) */

float d3_VOfs;  /* 0xfc26ec */
D3OBJECT d3_ObjEnd;  /* 0xfc2700, list end marker */
/* d3_TopLast is d3_ObjEnd.prev (0xfc28f4 = 0xfc2700 + 0x1f4): a macro in d3_engine.h */
float d3_ScaleY;  /* 0xfc2944 */
float d3_FogScale;  /* 0xfc2948 */
float d3_Dist2;  /* 0xfc294c */
D3VECTOR4 d3_Frustum[5] = {0}; /* 0xfc2960, corner directions of the view, then (0, 0, 1) */
float d3_FogStart1 = {0}; /* 0xfc29bc */
unsigned int d3_RadixCount[256];  /* 0xfc29c0 */
int d3_Unkfc2dc0;  /* 0xfc2dc0, texture bytes used this frame */
int d3_VertCount;  /* 0xfc2dc4 */
D3SORTENT * d3_SortPtr;  /* 0xfc2dc8 */
unsigned int d3_SortCount;  /* 0xfc2dcc */
float d3_InvZFar;  /* 0xfc2dd4, 1 / d3_ZFar */
float d3_FogEnd;  /* 0xfc2dd8 */
float d3_Unkfc2ddc;  /* 0xfc2ddc */
D3SORTENT * d3_SortBuf;  /* 0xfc2de0 */
int d3_Unkfc2df0;  /* 0xfc2df0 */
int d3_Unkfc2df4;  /* 0xfc2df4 */
char d3_Unkfc2e00[0x200];  /* 0xfc2e00, the head sentinel of the stock object list; its next field at +0x1fc (0xfc2ffc) is d3_StockListHead (a header macro, not a separate object) */
int d3_Unkfc3000;  /* 0xfc3000 */
float d3_FogStart;  /* 0xfc300c */
char d3_ClipInfo[64][0x68];  /* 0xfc3020 */
float d3_FovX;  /* 0xfc4a20 */
/* d3_Unkfc4c3c: view of d3_StockHead (tools/data_views.py) */
/* d3_FovY: view of d3_StockHead (tools/data_views.py) */
D3CVERT * d3_ClipList[2][0x80];  /* 0xfc4c60, polygon vertex lists (ping-pong) */
int d3_ClipNone;  /* 0xfc5060 */
unsigned char d3_Unkfc5064;  /* 0xfc5064 */
unsigned char d3_Unkfc5065;  /* 0xfc5065 */
D3VECTOR4 d3_Planes[6];  /* 0xfc5068, side planes of the view frustum (normal, distance); the code writes 6 */
D3SORTENT * d3_RadixDst;  /* 0xfc50c8 */
D3OBJECT d3_TopHead;  /* 0xfc5100, list start marker */
/* d3_TopFirst is d3_TopHead.next (0xfc52f0 = 0xfc5100 + 0x1f0): a macro in d3_engine.h */
float d3_ViewY;  /* 0xfc5328 */
float d3_ZScale;  /* 0xfc532c */
float d3_FogStart2;  /* 0xfc5330, squared */
float d3_MaxZ;  /* 0xfc5338 */
float d3_FogEnd2 = {0}; /* 0xfc5340 */
float d3_MinZ;  /* 0xfc5344 */
float d3_ScaleX;  /* 0xfc5348, projection */
D3CVERT d3_ClipPool[128];  /* 0xfc5360, vertices made by the clipper */
int d3_TriCount;  /* 0xfc7b60 */
float d3_ZOfs;  /* 0xfc7b64 */
unsigned char d3_Noise[256][256];  /* 0xfc7b80 */
int d3_Unkfe73a4;  /* 0xfe73a4 */
float d3_ViewX;  /* 0xfe73a8 */
float d3_UOfs;  /* 0xfe73ac */
float d3_ZNear;  /* 0xfe73b4 */
float d3_SavedZFar;  /* 0xfe73bc, d3_ZFar while the fog is off */
D3SORTENT * d3_RadixSrc;  /* 0xfe73c0 */
float d3_ViewH;  /* 0xfe73c4 */
float d3_ViewW;  /* 0xfe73c8 */
D3INITCFG d3_Cfg;  /* 0xfe73e0 */
/* d3_SortMax: view of d3_Cfg (tools/data_views.py) */
/* d3_Unkfe7400: view of d3_Cfg (tools/data_views.py) */
/* d3_Unkfe75a0: view of d3_AnimHead (tools/data_views.py) */
int d3_DrawnVerts;  /* 0xfe75b0, vertices drawn this frame (d3_DrawSorted resets it) */
int d3_TextureSwitches;  /* 0xfe75b4 */
float d3_HalfH;  /* 0xfe75b8 */
float d3_Unkfe75bc;  /* 0xfe75bc */
D3SORTENT * d3_SortTmp;  /* 0xfe75c0 */
D3TRIFUNC d3_TriFuncs[6];  /* 0xfe75e0, software triangle fillers by mode */
float d3_HalfW;  /* 0xfe75f8 */
float d3_CenterX;  /* 0xfe75fc */
float d3_Unkfe7600;  /* 0xfe7600 */
float d3_CenterY;  /* 0xfe7604 */
/* d3_ShadowMode: view of d3d_Config (tools/data_views.py) */
/* d3d_Caps98d: view of d3d_UsedCard (tools/data_views.py) */
char d3_Unk633438 = 0;  /* 0x633438 */
int d3_FrameCount = 0;  /* 0x63343c */
int d3_MirrorObject = 0;  /* 0x63344c, the object the frame's mirror was built for (0 none) */
unsigned char d3_Hardware = 0;  /* 0x633450 */
unsigned char d3_EngineReady = 0;  /* 0x633451, engine up: set at the end of d3_Init, cleared in d3_Shutdown */
unsigned int d3_ClipOut = 0;  /* 0x633454, vertices in the clipped polygon */
unsigned int d3_ClipIn = 0;  /* 0x633458, vertices in the polygon to clip */
unsigned char d3_D3DActive = 0;  /* 0x63345c, a Direct3D device was created (d3d_Init / d3d_CreateDevice) */
int d3_ClipExtraTris = 0;  /* 0x63346c */
int d3_Unk633470 = 0;  /* 0x633470, frame statistics */
int d3_Unk633474 = 0;  /* 0x633474 */
int d3_Unk633478 = 0;  /* 0x633478 */
int d3_Unk63347c = 0;  /* 0x63347c */
int d3_Unk633480 = 0;  /* 0x633480 */
int d3_Unk633484 = 0;  /* 0x633484 */
int d3_Unk633488 = 0;  /* 0x633488 */
unsigned int d3_Fps60 = 0;  /* 0x63348c, frames per second (two windows) */
unsigned int d3_Fps10 = 0;  /* 0x633490 */
int d3_Unk633494 = 0;  /* 0x633494, objects tested / drawn */
int d3_Unk633498 = 0;  /* 0x633498 */
unsigned long d3_FogColor = 0;  /* 0x6334a0 */
char d3_FogOn = 0;  /* 0x6334a4 */
unsigned char d3_FogByDistance = 0;  /* 0x6334a5, the byte right after d3_FogOn: when set, d3_Render clamps the fog end to the
                                        terrain's far distance (0x43e200); the scene setups of f3init.c / cm.c switch it off / on */
unsigned int d3_Fps60Time = 0;  /* 0x6334a8 */
unsigned int d3_Fps60Frames = 0;  /* 0x6334ac */
unsigned int d3_Fps10Time = 0;  /* 0x6334b0 */
unsigned int d3_Fps10Frames = 0;  /* 0x6334b4 */
int d3if_MegaCam = 0;  /* 0x6338c4 */
int d3_Busy = 0;  /* 0x633b04, nesting guard of d3_Render/d3_Animate */
unsigned int d3_LightKeysCulled = 0;  /* 0x633b08, light keys this frame spent on culled lights */
unsigned int d3_LightKeysVisible = 0;  /* 0x633b0c, light keys this frame spent on visible lights */
unsigned int d3_LightGeneration = 0;  /* 0x633b10, lightmap generation, D3MESH::lightGeneration holds the one drawn */
char d3_LightsChanged = 0;  /* 0x633b14 */

char d3e_RegisterEvent(D3OBJECT *obj, int id, D3EVHANDLER fn, char *name)
{
    int i;

    if (!obj)
        return 0;
    if (id) {
        if (fn) {
            if (!D3_X(obj)->events)
                D3_X(obj)->events = mem_Alloc(D3E_NEVENTS * sizeof(D3EVENT), "d3e:RegisterEvent(eventtable)");
            ((D3EVENT *)D3_X(obj)->events)[id].name[0x7f] = 0;
            strncpy(((D3EVENT *)D3_X(obj)->events)[id].name, name, 0x7f);
            ((D3EVENT *)D3_X(obj)->events)[id].fn = fn;
            return 1;
        }
        if (D3_X(obj)->events && ((D3EVENT *)D3_X(obj)->events)[id].fn) {
            ((D3EVENT *)D3_X(obj)->events)[id].fn = 0;
            for (i = 0; i < D3E_NEVENTS; i++)
                if (((D3EVENT *)D3_X(obj)->events)[i].fn)
                    break;
            if (i == D3E_NEVENTS) {
                mem_Free(D3_X(obj)->events);
                D3_X(obj)->events = 0;
            }
        }
    }
    return 1;
}
