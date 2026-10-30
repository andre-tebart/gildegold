/* 0x5d5c40, applies the graphics options to the scene: the shadow detail (ai_GfxDetail.byte[3]) picks the
   character shadow flag ch_ShadowEnabled and the shadow texture size (0x20 / 0x40), a changed size resets / frees
   every shadow and updates the scene, the map's detail level is set (init_SetDetailLevel), the LOD handling
   (byte[2]) selects d3io_Lod.mode and reloads the meshes with the scene deselected, d3_ApplyGraphicsOptions gets
   the floor mipmapping, the texture scale (2 - byte[1]) and the changed flag, the fog scale follows ai_GfxFog
   while no building is shown, the bodies of the town scene's characters get the shadow bit (+0x211 bit 3),
   ch_SetLowPoly gets the character detail (ai_GfxFloor.byte[3]), the camera limit pair of the camera_limits byte
   (ai_GfxFloor.byte[1]) is copied to sv_CameraHeightNear .. sv_CameraAngleFar, the game state re-places the
   camera, the mipmaps follow the floor setting and the scene is selected around the build-site check and the
   red lights. Returns 0
   @deep behaviour from the ROUGH stub rewritten against the whole original 0x5d5c40..0x5d5e8b (the stub was 16 B): the shadow detail (ai_GfxDetail.byte[3]) picks ch_ShadowEnabled + the 0x20 / 0x40 size and the changed flag, a changed size resets / frees the shadows (mask 0x40) and clamps d3s_TexSize to d3s_MaxTexSize before d3_UpdateAll(0); init_SetDetailLevel(ai_MapRefs, 0); the LOD handling (byte[2]) picks d3io_Lod (2 / 4), deselects the scene, reloads the meshes (0x40, 3) and reselects; d3_ApplyGraphicsOptions(floor byte, 0, 2 - texture_scale, changed, 0); the fog scale 1.0 - fog*0.01 (double constants) only while neither ai_PlayerBuilding nor ai_FallbackBuilding is set; the town-scene character bodies get the shadow bit (+0x211 bit 3, the xor idiom); ch_SetLowPoly(character_detail); the camera limit pair of the camera_limits byte (table 0x6237d0, 6 pairs, the pair +3) into sv_CameraHeightNear / sv_CameraAngleNear / ...Far; sv_PlaceCamera when ai_GameState; d3fl_SetMips on a floor change; then select the scene, ai_CheckBuildSites, d3_SetRLights(detail != 0), reselect. Q1/Q2/Q5/Q7 cleared (INIT_CHARACTER + ch_List, INIT_LOD union for the whole-dword d3io_Lod, INIT_MAPREF via data_views' ai_MapRefs, prototypes for all 13 callees). Left: 6 hunks / 98 differing instructions before this session, all codegen shape: the camera limit copy - ours did float copies (fld/fstp) from (0x623800 - 0x30) where the original copies the four dwords from the table symbol 0x6237d0; that table was missing from the C, so the copies read whatever the linker put before sv_CameraHeightNear (the camera angle became 8.2e20 and the whole 3D view culled blue); it is now sv_CameraLimits in sv.c and the copies come from it. Still open: the changed flag / lod / scene registers (slots hunk [esp+4]/[esp+8]). Renames: ch_Unk5f122c -> ch_ShadowEnabled, sv_Unk525dd0 -> sv_PlaceCamera, ai_Unk623800/04/08/0c -> sv_CameraHeightNear / sv_CameraAngleNear / sv_CameraHeightFar / sv_CameraAngleFar, ai_Unk6f22d4 / d8 -> sv_CameraHeightFactor / sv_CameraAngleFactor. */

/* the character list this unit walks for the shadow bit: ch.h's CHARACTER (only the fields used here; ch.h
   cannot be included in this unit) */
typedef struct INIT_CHARACTER {
    char pad000[0x34];              /* 0x00 .. 0x33 */
    unsigned char *obj;             /* 0x34, CHARACTER.obj: the body (its +0x211 bit 3 is the shadow bit) */
    char pad038[0x50];              /* 0x38 .. 0x87 */
    void *scene;                    /* 0x88, CHARACTER.scene */
} INIT_CHARACTER;
extern INIT_CHARACTER *ch_List[0x300];   /* 0x1468ae0, ch.h's CHARACTER *ch_List[CH_MAX] */
extern unsigned int d3s_TexSize;         /* 0xfb9260, d3s.c */
extern unsigned int d3s_MaxTexSize;      /* 0xfb91dc, d3s.c */
extern float d3o_FogScale;               /* 0x5fbd04, d3_interface.h */
extern float sv_CameraAngleFactor;               /* 0x6f22d8, sv.c */
extern int ai_GameState;       /* 0x6f25ac */
extern int d3_CurrentScene;              /* 0x633444 */
extern D3SCENE *d3_DefaultScene;         /* 0x5fb5b0 */
extern D3SCENE d3_Scenes[];              /* 0xfd7ba0 */
extern char d3fl_Mips;                   /* 0x63373c, d3_fl.h */
extern char ch_ShadowEnabled;            /* 0x5f122c, the shadow flag ch.c copies into the bodies' +0x211 bit 3 */
extern float sv_CameraLimits[12];        /* 0x6237d0, the camera limit table (height / angle pairs, sv.c) */
extern float sv_CameraHeightNear;        /* 0x623800, the camera height at the near / far blend (sv.c) */
extern float sv_CameraAngleNear;         /* 0x623804, the camera angle at the near / far blend */
extern float sv_CameraHeightFar;         /* 0x623808 */
extern float sv_CameraAngleFar;          /* 0x62380c */
void d3s_ResetShadows(struct D3OBJECT *obj);    /* 0x45ce00, d3s.c */
void d3s_FreeShadows(struct D3OBJECT *obj);     /* 0x45ccd0, d3s.c */
void d3_UpdateAll(unsigned char full);          /* 0x43b6e0, d3_engine.c */
char d3if_SelectScene(int index, char keep);    /* 0x4478e0, d3_interface.c */
void d3_ReloadMeshes(unsigned int mask, int type, void (*progress)(int a, int i, int n));  /* 0x43e090, d3_engine.c */
void d3_ApplyGraphicsOptions(int a, unsigned char b, unsigned char c, unsigned char mode, int e); /* 0x43c4e0 */
void ch_SetLowPoly(unsigned char mode);         /* 0x402af0, ch.c */
void d3fl_SetMips(char a);                      /* 0x43f930, d3_fl.c */
void ai_CheckBuildSites(void);                  /* 0x58fb60, unk_58bbd0.c */
void d3_SetRLights(unsigned char on);           /* 0x43e180, d3_engine.c */
void init_SetDetailLevel(int *s, char f);       /* 0x513590, f3init.c */
void sv_PlaceCamera(float f);                   /* 0x525dd0, sv.c */
char d3_ForEachObject2(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, void (*cb)(struct D3OBJECT *obj)); /* 0x438e60, d3_engine.h */
/* the LOD mask at 0x5fbf78 as this unit stores it (the whole dword, d3_int_io.h's D3IOLOD bits) */
typedef union INIT_LOD {
    unsigned int all;               /* the whole dword, which is what the original compares and stores */
    struct {
        unsigned int mode : 7;      /* 1: all three LODs, 2: the best one found */
        unsigned int shadow : 1;    /* the "_s" shadow variant */
    } bits;
} INIT_LOD;
extern INIT_LOD d3io_Lod;          /* 0x5fbf78 */

int ai_ApplyGfxOptions(void)
{
    unsigned char level, changed;
    unsigned size;
    int lod, scene, i;
    INIT_CHARACTER *c;
    float *lim;

    changed = 0;
    level = ai_GfxDetail.byte[3];
    size = d3s_TexSize;
    if (level == 0) {
        ch_ShadowEnabled = 1;
        size = 0x20;
    } else if (level == 1) {
        ch_ShadowEnabled = level;
        size = 0x20;
        changed = 1;
    } else if (level == 2) {
        ch_ShadowEnabled = 0;
        size = 0x40;
        changed = 1;
    }
    if (size != d3s_TexSize) {
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_ResetShadows);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_FreeShadows);
        d3s_TexSize = size;
        if (size >= d3s_MaxTexSize)
            d3s_TexSize = d3s_MaxTexSize;
        d3_UpdateAll(0);
    }
    init_SetDetailLevel((int *)ai_MapRefs, 0);
    lod = 0;
    if (ai_GfxDetail.byte[2] == 0)
        lod = 2;
    else if (ai_GfxDetail.byte[2] == 1)
        lod = 4;
    if (lod != (int)d3io_Lod.all) {
        scene = d3_CurrentScene;
        d3io_Lod.all = lod;
        d3if_SelectScene(0, 0);
        d3_ReloadMeshes(0x40, 3, 0);
        d3if_SelectScene(scene, 0);
    }
    d3_ApplyGraphicsOptions(ai_GfxFloor.byte[0], 0, 2 - ai_GfxDetail.byte[1], changed, 0);
    if (ai_PlayerBuildingB == 0 && ai_FallbackBuildingB == 0)
        d3o_FogScale = 1.0 - (double)ai_GfxFog.byte[0] * 0.01;
    for (i = 0; i < 0x300; i++) {
        c = ch_List[i];
        if (c != 0 && c->scene == d3_Scenes)
            c->obj[0x211] = ((ch_ShadowEnabled << 3 ^ c->obj[0x211]) & 8) ^ c->obj[0x211];
    }
    ch_SetLowPoly(ai_GfxFloor.byte[3]);
    lim = sv_CameraLimits;
    i = ai_GfxFloor.byte[1];
    sv_CameraHeightNear = lim[i * 2];
    sv_CameraAngleNear = lim[i * 2 + 1];
    sv_CameraHeightFar = lim[i * 2 + 6];
    sv_CameraAngleFar = lim[i * 2 + 7];
    if (ai_GameStateB != 0)
        sv_PlaceCamera(sv_CameraAngleFactor);
    if (d3fl_Mips != ai_GfxFloor.byte[0])
        d3fl_SetMips(ai_GfxFloor.byte[0]);
    scene = d3_CurrentScene;
    d3if_SelectScene(1, 0);
    ai_CheckBuildSites();
    d3_SetRLights(ai_GfxDetail.byte[0] != 0);
    d3if_SelectScene(scene, 1);
    return 0;
}
