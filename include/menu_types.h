/* The option dword menu's profile reader fills (ai_GfxDetail, ai_GfxFloor, ai_GfxResolution,
   ai_GamePanel ..), for the units that declare those options without the rest of menu.h: types
   only. menu.h includes it. */
#ifndef MENU_TYPES_H
#define MENU_TYPES_H

typedef union OPTIONBYTES {
    int word;                               /* the whole dword */
    unsigned char byte[4];                  /* the four option bytes */
} OPTIONBYTES;

/* 0x6f3600 .. 0x6f3608 of MENUOPTIONS: the "Sound" options of the profile (the reader 0x5d5640), the master_vol /
   sfx_vol / msx_vol / speech_vol bytes and the msx_freq dword. The byte fields are bitfields on a char base
   type: the original's compiler loads the packed master + sfx dword as a whole and extracts the
   second byte with `mov dl, ah` - a plain unsigned char field would be a separate byte load. */
typedef struct SNDOPTIONBYTES {
    unsigned char masterVolume:8;   /* +0x0, "master_vol" */
    unsigned char sfxVolume:8;      /* +0x1, "sfx_vol" */
    unsigned char msxVolume:8;      /* +0x2, "msx_vol" */
    unsigned char voiceVolume:8;    /* +0x3, "speech_vol" */
    union {
        int msxFreq;                /* +0x4, "msx_freq" (the low byte is used) */
        unsigned char msxFreqByte[4]; /* +0x4, the same as the four bytes the reader stores */
    };
} SNDOPTIONBYTES;

/* The option values the profile keeps (the reader 0x5d5640 and the writer 0x5d4c70, the "Game" / "Gfx" /
   "Sound" sections of 0x75e480), one object of 0xb0 bytes at 0x6f35c0 (a common of menu's group): the 48
   screen tuning floats at 0x6f35d0 and the 64-byte town name at 0x6f361c sit off the 32-byte alignment LINK
   gives a common of their size, the eight option bytes 0x6f35c4 .. 0x6f35cb are packed, and the users read the
   detail byte as a dword (lc's ai_ShowVgObjects 0x58f9d7 from another unit, menu's ai_SaveOptions): fields of
   one 32-aligned record. The reader stores the camera / mouse speed as single bytes; the run time reads them
   as integers. The old names are the field macros below. */
typedef struct MENUOPTIONS {
    OPTIONBYTES gfxSet;                     /* +0x00, byte 0 gfx_set */
    unsigned char gfxDetail;                /* +0x04, the "details" key */
    unsigned char gfxTextureScale;          /* +0x05, the "texture_scale" key */
    unsigned char gfxLodHandling;           /* +0x06, the "lod_handling" key */
    unsigned char gfxShadowDetail;          /* +0x07, the "shadow_detail" key */
    unsigned char gfxFloor;                 /* +0x08, the "floor_mipmapping" key */
    unsigned char gfxCameraLimits;          /* +0x09, the "camera_limits" key */
    unsigned char gfxFloorLod;              /* +0x0a, the "floor_lod" key */
    unsigned char gfxCharDetail;            /* +0x0b, the "character_detail" key */
    OPTIONBYTES gfxFog;                     /* +0x0c, byte 0 fog_plane */
    float screenTuning[0xc];                /* +0x10, brightness r, g, b, a, contrast r, g, b, a, gamma r, g, b, a
                                               (0 .. 1, the keys are the value * 100) */
    SNDOPTIONBYTES sound;                   /* +0x40, the "Sound" section */
    int speed;                              /* +0x48, the "speed" key: the game speed setting (0x2ee = 4.9375) */
    unsigned char cameraSpeed;              /* +0x4c, the "camera_speed" key */
    int mouseSpeed;                         /* +0x50, the "mouse_speed" key (the input base is this - 0x40) */
    int scrollSpeed;                        /* +0x54, the "scroll_speed" key */
    OPTIONBYTES gameSwitches;               /* +0x58, byte 0 invert_mouse, 1 nachtwaechter, 2 show_cursor_txt,
                                               3 show_geb_info */
    char townName[64];                      /* +0x5c, the "stadt" key (written as a string) */
    int historie;                           /* +0x9c, the "historie" key */
    int mission;                            /* +0xa0, the "mission" key */
    int netMission;                         /* +0xa4, the "net_mission" key */
    OPTIONBYTES gamePanel;                  /* +0xa8, byte 0 panel_mode, 1 help_events, 2 difficulty, 3 hints */
    OPTIONBYTES panelHelp;                  /* +0xac, the "panel_help" key */
} MENUOPTIONS;
extern MENUOPTIONS ai_Options;              /* 0x6f35c0 */
#define ai_GfxSet (ai_Options.gfxSet)
#define ai_GfxDetail (ai_Options.gfxDetail)
#define ai_GfxTextureScale (ai_Options.gfxTextureScale)
#define ai_GfxLodHandling (ai_Options.gfxLodHandling)
#define ai_GfxShadowDetail (ai_Options.gfxShadowDetail)
#define ai_GfxFloor (ai_Options.gfxFloor)
#define ai_GfxCameraLimits (ai_Options.gfxCameraLimits)
#define ai_GfxFloorLod (ai_Options.gfxFloorLod)
#define ai_GfxCharDetail (ai_Options.gfxCharDetail)
#define ai_GfxFog (ai_Options.gfxFog)
#define ai_ScreenTuning (ai_Options.screenTuning)
#define ai_SoundOptions (ai_Options.sound)
#define ai_OptionSpeed (ai_Options.speed)
#define ai_OptionCameraSpeed (ai_Options.cameraSpeed)
#define ai_OptionMouseSpeed (ai_Options.mouseSpeed)
#define ai_OptionScrollSpeed (ai_Options.scrollSpeed)
#define ai_GameSwitches (ai_Options.gameSwitches)
#define ai_TownName (ai_Options.townName)
#define ai_OptionHistorie (ai_Options.historie)
#define ai_OptionMission (ai_Options.mission)
#define ai_OptionNetMission (ai_Options.netMission)
#define ai_GamePanel (ai_Options.gamePanel)
#define ai_OptionPanelHelp (ai_Options.panelHelp)

#endif
