/* cmfight.c -- the fight of a raid / brawl scene (0x4cd9d0..0x4d8b50): weapons and their records, bombs and
   grenades, the portraits and character info windows, the orders and the fight AI, the actions (hit, bombs, death),
   the loot and the fight dialogs, the fight loop. Split from cm (data layout s12): its own .data block
   ai_WeaponRecordData 0x60c7c0 .. cm_FightButton 0x60cb68 after cm's last string ("%s %s" of cm_LobbyAddChat), then its
   string run (first user ai_FindWeaponRecord); its .bss 0x6f0ce0..0x6f0d68; its common group 0x930380..0x935da0
   (cm_PointList .. cm_CharInfoWins, read only by these functions) between the scene's group and cm's. The sync
   values sent by address are block locals (cm_AddFieldValues in cm stays unknown here; byte both ways).
   Open: summary: the leaf ai_LedSelectBits counts as writing ai_PickedObject in the original (cm_UpdatePointer
   reloads it after each call), which its text does not give; it holds a dead `if (0) ai_PickedObject = 0;` with
   an @original comment (cm_UpdatePointer 142 -> 36 ins and its padded size, size queue s19); the original's own
   form of that write is not known (an external copy gave only 49, a write of another global nothing). The other
   such case, cm_RunCutOrders reloading x->scene->obj after cm_CharObjDistXZ, was a unit boundary: cmorders.
   unit 0x004cd9d0..0x004d76a0; boundaries in decl/text.dcl */
#include "d2_types.h"
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "net.h"
#include "cm.h"
#include "cmfight.h"
#include "d2.h"
#include "d3.h"
struct D3SKYLAYER; struct D3SKY; struct D3SKYLAYER *d3sky_CreateLayer(struct D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset, float vscale, float voffset);  /* 0x45d880 */
#include "d3_tools.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "menu_types.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];


#define G17_I(m, o) (*(int *)&(m).data[o])
#define G17_S(m, o) (*(short *)&(m).data[o])
/* the block at 0x936420 is 0x24 bytes: the count then the eight player states at +4 (one object, so the
   0x24-byte memset / memcpy in 0x4cd580 and 0x4cdxx cannot run over a neighbour that LINK placed there) */
#define cm_LobbyListIds (*(int (*)[8])((char *)cm_LobbyList + 4))  /* 0x936424, the eight player states (a view) */
#define cm_LobbyPlayers ((CMLOBBYPLAYER *)cm_LobbyRecords)  /* 0x9364e0, the eight lobby player records (the CMJOIN cm_JoinChar follows at +0x2c0) */
#define ai_WeaponRecords ((AIWEAPON *)ai_WeaponRecordData)  /* 0x60c7c0, the ten 0x5c-byte weapon records ai_FindWeaponRecord searches (a view) */

#include "f3init.h"
#include "sv.h"
#include "ahm.h"
#include "ai.h"
#include "ch.h"
#include "d3_anim.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3_io_t.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "err.h"
#include "f3snd.h"
#include "io_text.h"
#include "ls.h"
#include "main.h"
#include "mem.h"
#include "msx.h"
#include "rd.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "lc.h"
#include "shp.h"
#include "f3gm_types.h"
#include "f3gm.h"
#include "he.h"
#include "amt.h"
#include "am_types.h"
#include "mission.h"
#include "gv.h"
#include "poker.h"
#include "gfx.h"
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
#define d3sm_PickK ((int (*)(D3SM *, int, int, int *, int *))d3sm_Pick)  /* 0x45f2b0 (d3_sm.c: char d3sm_Pick(D3SM *, int, int, int *, int *)): @original this unit's callers read an int result (byte-matched cm_UpdatePointer 0x4cf73a: and eax, 0xff after the call) */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define cm_LongMsgCut (*(CMCUTINFO *)&cm_LongMsg)  /* 0xa68d40, cm_LongMsg (cm.h: unsigned char[]) as the cutscene record message 0x26 joins */
#define ai_TimeCopyT (*(AITIME *)&ai_TimeCopy)  /* 0x75e130, ai_TimeCopy (cm.h: int[4]) as the AITIME copy it holds */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the player slots */
#define CM_KEEPER_FLAG 0x800000     /* flags1c8 bit 23 (he.c HE_LOVER_FLAG: has a keeper; f3gm.c ORD_F1C8_ANEW): message 0x19
                                       may not set it on a character that has it */
typedef struct CMCUTTYPEMSG {      /* 0x7c bytes, the data of the system message 0x11 cm_RunSceneParty sends (cm_SendExSysMessage) */
    int party;                      /* 0x00, the party's id (G28REC.id) */
    char text[0x78];                /* 0x04, "cut_type:%i" with the scene kind */
} CMCUTTYPEMSG;
typedef struct CMNOTE {             /* 0x8f8 bytes, a long message (AIMSG) with its texts right after it, as cm_SendPartyNote builds it */
    AIMSG head;                     /* 0x00 */
    char text[0x800];               /* 0xf8, the text, then the second one (textFlags 0x10) */
} CMNOTE;
typedef struct CMFIELD {            /* the head of one field of a field block (G17FIELDS.buf); its values follow at 0x04 */
    signed char size;               /* 0x00, a value's size: 1, 2 or 4 bytes */
    signed char n;                  /* 0x01, the values */
    unsigned short offset;          /* 0x02, the field's offset in the record */
    int values[1];                  /* 0x04, the values */
} CMFIELD;
typedef struct AIJOBTAIL {          /* AIJOB as message 0x1d sends and takes its data block: the record from 0xac */
    char pad00[0xac];
    char data[0xa0];                /* 0xac, the long message (cm_SetLongMsg 0xa0, cm_LongMsg) */
} AIJOBTAIL;
typedef struct CMNEWPARTYMSG {      /* 8 bytes, the data of message 0x27 (its handler cm_ExNewParty) */
    int partyId;                    /* 0x00, the id the new scene party gets (ai_NextPartyId) */
    int seed;                       /* 0x04, the party's random seed (G28REC.seed) */
} CMNEWPARTYMSG;
typedef struct CMJOINPARTYMSG {     /* 8 bytes, the data of message 0x29 (its handler cm_ExAddPartyMember) */
    int partyId;                    /* 0x00, the scene party record (cm_PartyById) */
    int member;                     /* 0x04, the member's netId (cm_AddPartyMember) */
} CMJOINPARTYMSG;
typedef struct CMWALKPATHMSG {      /* 0x24 bytes, the data of message 0x4f (its handler cm_ExWalkPath; no sender in
                                       the sources, the path cells of a walk) */
    char name[0x18];                /* 0x00, the character's object name ("sp_<id>") */
    int from;                       /* 0x18, the first cell written */
    int to;                         /* 0x1c, the cell past the path (the action's new path length) */
    unsigned char cells[4];         /* 0x20, up to two cells, x and z bytes each */
} CMWALKPATHMSG;
typedef struct CMPOSMSG {           /* 0x10 bytes read, the data of message 0x32 (its handler cm_ExChrSetPos) */
    int charId;                     /* 0x00, the character */
    D3VECTOR pos;                   /* 0x04, its new position (ch_SetPos) */
} CMPOSMSG;
typedef struct CMGRENADE {          /* 0x10 bytes, one of the 32 thrown bombs of cm_Grenades */
    D3OBJECT *obj;                  /* the object "ob_BOMBE", 0 = a free slot */
    D3ANIM *anim;                   /* 0x04, its flight path (d3anim_CreatePath) */
    int x, y;                       /* 0x08, the target cell */
} CMGRENADE;
typedef struct CMBOMB {             /* 8 bytes, one of the 32 bombs of cm_Bombs */
    D3OBJECT *obj;                  /* the bomb object "ob_BOMBE", 0 = a free slot */
    unsigned long time;             /* d3_Time when it was laid */
} CMBOMB;
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define cm_FightersT (*(AICHAR (*)[32])&cm_Fighters)  /* 0x930600, cm_Fighters (char[], with the 0x1000 bytes after the 32 records) as its AICHAR records */
#define ai_SceneCharsT (*(AICHAR *(*)[32])&ai_SceneChars)  /* 0x7603a0, ai_SceneChars (f3init.h: int[32]) as the AICHAR pointers it holds */
#define d2_LastWindowPtrT (*(struct D2WIN **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) as the window record */
typedef struct CMAMOUNTROW {        /* 0x38 bytes, one row of the special-amount list (ai_AmountRowStatus / ai_AmountRow read it
                                       as ints; menu.c GYPSYROW) */
    AICHAR *rec;                    /* 0x00, the character the row shows */
    int ctl;                        /* 0x04, the row's control object (d2_Objs index, ai_AmountRow) */
    int pad08[3];
    int y;                          /* 0x14 */
    int pad18[5];
    int x2c;                        /* 0x2c */
    int x30;                        /* 0x30 */
    int pad34;
} CMAMOUNTROW;

/* dcp unit split: the declarations of cm before the moved functions */
#define CM_CHR(c) ((CHARACTER *)(c)->model)  /* AICHAR.model (ai.h: AITASK *) as the CHARACTER it is */
typedef struct CMSPYREPORT {            /* an AIOBJ of kind 0xca (a spy report) as this unit reads it: the report's words
                                           0x1c .. 0x37 (HESPYREPORT) after the object's head */
    char pad1c[0x1c];
    HESPYREPORT report;
} CMSPYREPORT;

char ai_WeaponRecordData[920] = { 84, 1, 0, 0, 120, 5, 20, 0, 1, 4, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0, 86, 1, 0, 0, 95, 10, 15, 0, 1, 4, 1, 0, 0, 0, -96, 66, 5, 107, 97, 109, 112, 102, 47, 115, 116, 114, 101, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 17, 0, 0, 0, 0, 0, 0, 88, 1, 0, 0, 80, 20, 40, 0, 1, 4, 1, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 104, 105, 101, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 17, 0, 0, 0, 0, 0, 0, 114, 1, 0, 0, 110, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 17, 0, 0, 0, 0, 0, 0, 110, 1, 0, 0, 100, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 17, 0, 0, 0, 0, 0, 0, 116, 1, 0, 0, 100, 0, 30, 1, 2, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 17, 0, 0, 2, 0, 0, 0, 118, 1, 0, 0, 90, 0, 30, 1, 1, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 17, 0, 0, 2, 0, 0, 0, 94, 1, 0, 0, 60, 5, 20, 1, 1, 3, 0, 0, 0, 0, -56, 67, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 112, 105, 115, 116, 111, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106, 17, 0, 0, 1, 0, 0, 0, 96, 1, 0, 0, 70, 10, 30, 1, 1, 3, 0, 0, 0, 0, 22, 68, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 97, 114, 109, 98, 114, 117, 115, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, 17, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, -106, 2, 8, 0, 1, 5, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 99, 104, 108, 97, 103, 101, 110, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0 };  /* 0x60c7c0, the ten 0x5c-byte records ai_FindWeaponRecord searches (G12REC10, hst.c); the last has id 0, the fallback */
float cm_DefenseFactor = 1.0f;  /* 0x60cb58 */
int cm_FightOwner = -1;  /* 0x60cb5c */
int cm_FightView = -1;  /* 0x60cb60 */
int cm_FightTimeBar = -1;  /* 0x60cb64 */
int cm_FightButton = -1;  /* 0x60cb68 */
char * cm_PointList[32];  /* 0x930380 */
CMPORTRAIT cm_Portraits[6];  /* 0x930400 */
G18POP cm_DamagePops[16];  /* 0x9304c0 */
char cm_Fighters[21376];  /* 0x930600 */
struct D3OBJECT *cm_FlagObjects[32];  /* 0x935980 */
CMGRENADE cm_Grenades[32];  /* 0x935a00, the thrown bombs */
CMBOMB cm_Bombs[32];  /* 0x935c00, the 32 bomb records (object, time) */
float cm_FightCellScale;  /* 0x935d00 */
int cm_CharInfoWins[32];  /* 0x935d20 */
char * cm_HoverObject = 0;  /* 0x6f0ce0 */
int cm_FightPaused = 0;  /* 0x6f0ce4, fight paused */
int cm_FightDefender = 0;  /* 0x6f0ce8 */
int cm_FightAttacker = 0;  /* 0x6f0cec */
char * cm_PickMarker = 0;  /* 0x6f0cf0 */
int cm_FightWinner = 0;  /* 0x6f0cf4 */
int cm_FightSettled = 0;  /* 0x6f0cf8 */
int cm_FightLootValue = 0;  /* 0x6f0cfc */
int cm_UnreadBss = 0;  /* 0x6f0d00, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
char * cm_FightRecord = 0;  /* 0x6f0d04 */
int cm_SelectedCount = 0;  /* 0x6f0d08 */
struct SBBANK *cm_FightSounds = 0;  /* 0x6f0d0c */
struct SBBANK *cm_CrossbowSounds = 0;  /* 0x6f0d10 */
struct SBBANK *cm_BombSounds = 0;  /* 0x6f0d14 */
struct SBBANK *cm_DaggerSounds = 0;  /* 0x6f0d18 */
struct SBBANK *cm_PistolSounds = 0;  /* 0x6f0d1c */
struct SBBANK *cm_SwordSounds = 0;  /* 0x6f0d20 */
int cm_AttackerClickVoices = 0;  /* 0x6f0d24 */
int cm_DefenderClickVoices = 0;  /* 0x6f0d28 */
int cm_AttackerAttackVoices = 0;  /* 0x6f0d2c */
int cm_DefenderAttackVoices = 0;  /* 0x6f0d30 */
int cm_AttackerOrderVoices = 0;  /* 0x6f0d34 */
int cm_DefenderOrderVoices = 0;  /* 0x6f0d38 */
int cm_AttackerTauntVoices = 0;  /* 0x6f0d3c */
int cm_DefenderTauntVoices = 0;  /* 0x6f0d40 */
int cm_AttackerPainVoices = 0;  /* 0x6f0d44 */
int cm_DefenderPainVoices = 0;  /* 0x6f0d48 */
int cm_FightAmbient = 0;  /* 0x6f0d4c */
int cm_AttackerRole = 0;  /* 0x6f0d50 */
int cm_DefenderRole = 0;  /* 0x6f0d54 */
int cm_FlagObjectCount = 0;  /* 0x6f0d58 */
int cm_BootsObject = 0;  /* 0x6f0d5c */
int cm_PointCount = 0;  /* 0x6f0d60 */
int cm_FightForm = 0;  /* 0x6f0d64 */

/* 0x4cd9d0, the weapon record (AIWEAPON) of the ten at ai_WeaponRecords with the goods item id id, 0: none */

char *ai_FindWeaponRecord(int id)
{
    int i = 0;
    AIWEAPON *p;

    for (p = ai_WeaponRecords; (int)p < (int)(ai_WeaponRecords + 10); p++) {
        if (p->id == id)
            return (char *)(ai_WeaponRecords + i);
        i++;
    }
    return 0;
}
/* 0x4cda00, the first weapon record (ai_FindWeaponRecord) of the kind 5 items of the character c's inventory */

char *cm_CharWeaponAny(char *c)
{
    AIOBJ *q;
    char *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = ai_FindWeaponRecord(q->id);
        if (r != 0)
            return r;
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cda50, as cm_CharWeaponAny; a record of kind 1 / 2 whose item has nothing left (AIOBJ.usesLeft) is not taken */

char *cm_CharWeapon(char *c)
{
    AIOBJ *q;
    AIWEAPON *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = (AIWEAPON *)ai_FindWeaponRecord(q->id);
        if (r != 0) {
            if ((r->kind == 1 || r->kind == 2) && q->usesLeft <= 0)
                return ai_FindWeaponRecord(0);
            return (char *)r;
        }
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cdac0, the inventory entry (AIOBJ, not its weapon record) of the search cm_CharWeapon does, 0: none */

char *cm_CharWeaponItem(char *c)
{
    AIOBJ *q;
    AIWEAPON *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = (AIWEAPON *)ai_FindWeaponRecord(q->id);
        if (r != 0) {
            if ((r->kind == 1 || r->kind == 2) && q->usesLeft <= 0)
                return 0;
            return (char *)q;
        }
    }
    return 0;
}
/* 0x4cdb20, gives up every weapon item of the character c's inventory: sends each as sold to nobody (cm_SendExSellObject, buyer -1) */

void cm_DiscardWeapons(char *c)
{
    AICHAR *ch = (AICHAR *)c;
    AIOBJ *q;
    int id;

    for (q = (AIOBJ *)gm_Query(ch->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        id = q->id;
        if (ai_FindWeaponRecord(id) != 0)
            cm_SendExSellObject(-1, ch->netId, id, q->count, ai_OwnPlayer, 0);
    }
}
/* 0x4cdb80, after a fight, of a character c with the flag 0x20000000: with the chance c->b1c5 in 10 its weapons are
   taken (cm_DiscardWeapons), b1c5 is negated and the owner of its building (a character of kind 6 / 7) gets the news
   0x1bca naming the weapon; returns 1. Else b1c5 becomes 1, the flag is cleared; returns 0 */

char cm_FightWeaponLoss(char *ch)
{
    char buf[0x800];
    char v = 1;    /* the value sent for b1c5: 1, or the negated chance */
    char neg;
    AICHAR *c = (AICHAR *)ch;
    AICHAR *owner;
    AIWEAPON *w;

    if (c != 0 && (c->flags1c8 & 0x20000000) != 0) {
        neg = c->b1c5;
        if (util_Random(10) < (unsigned char)neg) {
            v = -neg;
            cm_DiscardWeapons((char *)c);
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValues(1, 1, (int)((char *)c - cm_FieldObject) + 0x1c5, &v);
            cm_SendFieldDeltas();
            if (c->bld != 0 && c->bld->owner != 0xffff) {
                owner = &ai_Chars[c->bld->owner];
                if (owner->kind == 6 || owner->kind == 7) {
                    w = (AIWEAPON *)cm_CharWeapon((char *)c);
                    if (w->id != 0) {
                        txt_Format(buf, 0x1bca, w->id * 2 + 0x9a8, c->id, c->bld);
                        he_ShowNews(owner->netId, -1, buf, 0, 0x5f2, 0);
                    }
                }
            }
            return 1;
        }
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValues(1, 1, (int)((char *)c - cm_FieldObject) + 0x1c5, &v);
        cm_SendFieldDeltas();
        cm_SendSetField(c->netId, 0x1c8, 4, 0, 0x20000000);
    }
    return 0;
}
/* 0x4cdcf0, the fighting strength of the character c: its health ratio times its skill 3 times a factor of the
   weapon it carries (cm_CharWeapon's item: 0x158 1.0, 0x154 0.5, 0x156 / 0x16e / 0x172 0.8, 0x174 / 0x176 0.9,
   0x15e / 0x160 0.7, another 0.3; none 0.5) */

float cm_FightStrength(char *c)
{
    float f = 0.5f;
    float skill;
    AIWEAPON *w;

    skill = gm_GetSkill(c, 3);
    w = (AIWEAPON *)cm_CharWeapon(c);
    if (w != 0) {
        switch (w->id) {
        case 0x158:
            f = 1.0f;
            break;
        case 0x154:
            f = 0.5f;
            break;
        case 0x156:
        case 0x16e:
        case 0x172:
            f = 0.8f;
            break;
        case 0x174:
        case 0x176:
            f = 0.9f;
            break;
        case 0x15e:
        case 0x160:
            f = 0.7f;
            break;
        default:
            f = 0.3f;
        }
    }
    return gm_HealthRatio(c) * skill * f;
}
/* 0x4cde20, puts the weapon the character c carries (cm_CharWeapon) into its scene character's right hand as the
   model "ob_<GOOD NAME>" (unless it holds that already); without a weapon the hand is emptied */

void cm_UpdateHandObject(char *ch)
{
    char buf[0x80];
    AICHAR *c = (AICHAR *)ch;
    AIWEAPON *w;
    AIGOOD *g;

    w = (AIWEAPON *)cm_CharWeapon(ch);
    if (w != 0 && w->id != 0) {
        g = &ai_Goods[w->id];
        if (c->scene == 0)
            return;
        sprintf(buf, "ob_%s", strupr(g->name));
        if (c->scene->rightHand != 0 && strcmpi(c->scene->rightHand->name, buf) == 0)
            return;
        if (c->scene->rightHand != 0)
            ch_SetHandObject(c->scene, 2, 0);
        ch_SetHandObject(c->scene, 2, buf);
        return;
    }
    if (c->scene != 0 && c->scene->rightHand != 0)
        ch_SetHandObject(c->scene, 2, 0);
}
/* 0x4cdf00, the record of the character c in the 16 records (0x2c bytes, id at +0) of rec + 0x94, else a free one
   (id -1), 0: none */

int *cm_CutEntrySlot(char *rec, char *c)
{
    CMCUTINFO *r = (CMCUTINFO *)rec;
    int id = ((AICHAR *)c)->netId;
    int i;

    for (i = 0; i < 16; i++) {
        if (r->entries[i].charId == id)
            return &r->entries[i].charId;
    }
    for (i = 0; i < 16; i++) {
        if (r->entries[i].charId == -1) {
            memset(&r->entries[i], 0, sizeof(CMCUTENTRY));
            r->entries[i].charId = ((AICHAR *)c)->netId;
            r->entries[i].state = 0;
            return &r->entries[i].charId;
        }
    }
    return 0;
}
/* 0x4cdf80, the entry (CMCUTENTRY) of the character c in the first cm_FightRecord[0x30] cutscene records
   (cm_CutInfos), 0: none */

int *cm_FightCutEntry(char *c)
{
    int n = (unsigned char)cm_FightRecord[0x30];
    CMCUTINFO *r;
    int i, j;

    for (i = 0; i < n; i++) {
        r = &cm_CutInfos[i];
        for (j = 0; j < 16; j++) {
            if (r->entries[j].charId == ((AICHAR *)c)->netId)
                return &r->entries[j].charId;
        }
    }
    return 0;
}
/* 0x4cdfe0, 1 when the cell (x, y) of the height map d3_DefaultScene->hmap is inside and passable */

int cm_CellUsable(int x, int y)
{
    D3SM *g = (D3SM *)d3_DefaultScene->hmap;
    int w = g->size;
    int type;

    if (x > w || x < 0 || y < 0 || y > w || x >= w || y >= w)
        return 0;
    type = g->entries[w * y + x].type;
    if (type == 0 || type == 0xd)
        return 0;
    return 1;
}
/* 0x4ce040, the nearest usable cell (cm_CellUsable) of the current scene's height map around the cell (a, b), in
   diamonds of the distance d = 0 .. 0x3f, rows top to bottom: *ox, *oy, returns 1; 0: none
   @deep near-byte sweep s22: left the w load chain (ours hmap in ecx, the original eax -> eax) and mov
   edx,ebp placed after it. climb round 1: nothing; w as a statement / via a D3SM local = control; w - 1 as
   w: 12; ylo / xlo first: far worse. */

int cm_FindNearCell(int a, int b, int *ox, int *oy)
{
    int w = ((D3SM *)d3_DefaultScene->hmap)->size;
    int d, y, x, k, ylo, yhi, xlo, xhi;

    for (d = 0; d < 0x40; d++) {
        yhi = w - 1 < b + d ? w - 1 : b + d;
        ylo = b - d < 0 ? 0 : b - d;
        for (y = ylo; y <= yhi; y++) {
            k = d - abs(b - y);
            xhi = w - 1 < k + a ? w - 1 : k + a;
            xlo = a - k < 0 ? 0 : a - k;
            for (x = xlo; x <= xhi; x++) {
                if (cm_CellUsable(x, y) != 0) {
                    *ox = x;
                    *oy = y;
                    return 1;
                }
            }
        }
    }
    return 0;
}
/* 0x4ce130, the fighter (an AICHAR, 0x21c bytes) with the netId id among the 32 of cm_Fighters, 0 when none */

char *cm_FindFighter(int id)
{
    int i = 0;
    char *p;

    for (p = cm_Fighters + 4; (int)p < (int)(cm_Fighters + 4 + 32 * 0x21c); p += 0x21c) {
        if (((AICHAR *)(p - 4))->id != 0xffff && ((AICHAR *)(p - 4))->netId == id)
            return cm_Fighters + i * 0x21c;
        i++;
    }
    return 0;
}
/* 0x4ce170, the select bits of the fighter c: 0x800 when its side leader (bld in a fight) is the character idx,
   or when gm_AllowAll is set */

int ai_LedSelectBits(int idx, char *c)
{
    int r = 0;

    if ((AICHAR *)((AICHAR *)c)->bld == (AICHAR *)&ai_Chars[idx])
        r = 0x800;
    if (gm_AllowAll != 0)
        r = 0x800;
    if (0)
        ai_PickedObject = 0;   /* @original dead code: VC6 counts this leaf as writing ai_PickedObject (cm_UpdatePointer reloads it after each call); no code */
    return r;
}
/* 0x4ce1b0, the distance of the character c's model to the object o (x / z) */

float cm_CharObjDistXZ(char *c, char *o)
{
    D3VECTOR4 v, d;

    v.x = ((D3OBJECT *)o)->pos.x;
    v.z = ((D3OBJECT *)o)->pos.z;
    d.x = ((AICHAR *)c)->scene->obj->pos.x;
    d.z = ((AICHAR *)c)->scene->obj->pos.z;
    d.x = v.x - d.x;
    d.z = v.z - d.z;
    return (float)sqrt(d.z * d.z + d.x * d.x);
}
/* 0x4ce200, the distance in the x / z plane of the character c's body to the vertex of cell (x, y) (the frame holds two 16-byte vectors) */

float cm_CharCellDistXZ(char *c, int x, int y)
{
    D3VECTOR4 v, d;
    D3VECTOR b;

    d3sm_GetVertex((struct D3SM *)d3_DefaultScene->hmap, x, y, (D3VECTOR *)&v);
    v.y = 0;
    b.x = ((AICHAR *)c)->scene->obj->pos.x;
    b.z = ((AICHAR *)c)->scene->obj->pos.z;
    d.x = v.x - b.x;
    d.z = v.z - b.z;
    return (float)sqrt(d.z * d.z + d.x * d.x);
}
/* 0x4ce270, 1 when the two characters are not in the same building (+0x16c) */

int cm_OtherBuilding(char *a, char *b)
{
    return ((AICHAR *)a)->bld != ((AICHAR *)b)->bld;
}
/* 0x4ce290, a bomb object "ob_BOMBE" at the position p in a free slot of the 32 (object, time) of cm_Bombs */

CMBOMB *cm_LayBomb(D3VECTOR *p)
{
    D3VECTOR rot;
    int i;
    CMBOMB *b;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    for (i = 0; i < 32; i++) {
        if (cm_Bombs[i].obj == 0) {
            b = &cm_Bombs[i];
            b->obj = d3_CreateObject(0, p, &rot, "ob_BOMBE");
            cm_Bombs[i].time = d3_Time;
            return b;
        }
    }
    return 0;
}
/* 0x4ce300, updates the bombs of cm_Bombs: a bomb older than 0x15e ms explodes (the sound "Explosion_s", smoke, earth
   and explosion effects at its position) and hurts every scene character within 170.0 by a random weapon damage (0x174)
   scaled by its life value and the distance; returns the number of used slots */

int cm_UpdateBombs()
{
    int cnt;
    AIWEAPON *w;
    CMBOMB *rec;
    AICHAR *c;
    CHARACTER *ch;
    D3OBJECT *bo;
    struct { float x, y, z; int w; } a, b;   /* @original the pad word is an int: its 0 stores use the cached zero register */
    D3COLOR col;
    float dist;
    int i, j, t, amt;

    cnt = 0;
    w = (AIWEAPON *)ai_FindWeaponRecord(0x174);
    for (i = 0; i < 32; i++) {
        rec = &cm_Bombs[i];
        if (rec->obj == 0)
            continue;
        cnt++;
        if (d3_Time <= rec->time + 0x15e)
            continue;
        a.x = -243.0f;
        a.y = -90.0f;
        a.z = 190.0f;
        a.w = 0;
        b.x = -50.0f;
        b.y = 0.0f;
        b.z = 38.0f;
        b.w = 0;
        sb_PlaySample(cm_BombSounds, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        col.r = 0xc0;
        col.g = 0xc0;
        col.b = 0xc0;
        col.a = 0x3f;
        a.x = rec->obj->pos.x;
        a.y = rec->obj->pos.y;
        a.z = rec->obj->pos.z;
        d3t_CreateSmoke(0, (D3VECTOR *)&a, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, (D3VECTOR *)&b, 0x190, 6, col);
        d3t_CreateEarth(0, (D3VECTOR *)&a, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, col);
        d3t_CreateExplosion(0, (D3VECTOR *)&a, 0x1e, 120.0f, 80.0f, 20.0f, 60.0f, 0x3c);
        for (j = 0; j < 32; j++) {
            if (ai_SceneCharsT[j] == 0)
                continue;
            ch = ai_SceneCharsT[j]->scene;
            c = ai_SceneCharsT[j];
            if (c->health == 0)
                continue;
            bo = rec->obj;
            a.x = bo->pos.x - ch->obj->pos.x;
            a.y = bo->pos.y - ch->obj->pos.y;
            a.z = bo->pos.z - ch->obj->pos.z;
            dist = (float)sqrt(a.z * a.z + a.y * a.y + a.x * a.x);
            if (dist < 170.0f) {
                t = (int)((cm_Random(w->dmgMax - w->dmgMin) + w->dmgMin) * (c->f01c * 0.01));
                amt = (int)(t * (1.0 - dist * 0.0058823530562222f));
                ai_SceneCharsT[j]->v024 -= amt;
                if ((int)ai_SceneCharsT[j]->bld == cm_FightAttacker)
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 1);
                else
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 2);
                if (cm_CheckDeath(c) == 0)
                    ch_QueueAnim(ch, "kampf/bauch_greifen", 1);
            }
        }
        d3if_Delete(rec->obj);
        rec->obj = 0;
    }
    return cnt;
}
/* 0x4ce5d0, throws a bomb: the first free slot of cm_Grenades gets the object "ob_BOMBE" at p (60.0 higher) and a
   4-key path animation (up to half way + 50.0, then down) to the vertex of the cell (x, y); 0 when all 32 are used */

CMGRENADE *cm_ThrowGrenade(D3VECTOR *p, int x, int y)
{
    D3ANIMKEY keys[4];
    D3VECTOR4 mid;
    D3VECTOR4 pos;
    D3VECTOR rot;
    D3ANIMFLAGS af;
    D3VECTOR4 d;
    struct D3SM *g;
    int i;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    g = (struct D3SM *)d3_DefaultScene->hmap;
    for (i = 0; i < 32; i++) {
        if (cm_Grenades[i].obj != 0)
            continue;
        pos.x = p->x;
        pos.y = p->y + 60.0;
        pos.z = p->z;
        cm_Grenades[i].obj = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, "ob_BOMBE");
        cm_Grenades[i].obj->objectFlags |= 4;
        d3sm_GetVertex(g, x, y, (D3VECTOR *)&d);
        d.x -= p->x;
        d.y -= p->y;
        d.z -= p->z;
        memset(keys, 0, sizeof(keys));
        mid.x = d.x * 0.5;
        mid.y = d.y * 0.5;
        mid.z = d.z * 0.5;
        mid.y += 50.0;
        D3_ZERO(&af, sizeof(af));
        af.b[1] |= 0x14;
        af.b[8] |= 2;
        af.b[1] |= 8;
        keys[1].pos.x = mid.x;
        keys[1].pos.y = mid.y;
        keys[1].pos.z = mid.z;
        keys[1].rot.x = 3.1415927f;
        keys[1].rot.y = 3.1415927f;
        keys[1].rot.z = 3.1415927f;
        d.y -= 60.0;
        keys[2].pos.x = d.x;
        keys[2].pos.y = d.y;
        keys[2].pos.z = d.z;
        keys[2].rot.x = 3.1415927f;
        keys[2].rot.y = 3.1415927f;
        keys[2].rot.z = 3.1415927f;
        keys[3].pos.x = d.x;
        keys[3].pos.y = d.y;
        keys[3].pos.z = d.z;
        keys[3].rot.x = 3.1415927f;
        keys[3].rot.y = 3.1415927f;
        keys[3].rot.z = 3.1415927f;
        keys[0].time = 0x12;
        keys[1].time = 0x12;
        keys[2].time = 1;
        cm_Grenades[i].anim = d3anim_CreatePath(cm_Grenades[i].obj, af, 4, keys);
        cm_Grenades[i].x = x;
        cm_Grenades[i].y = y;
        return &cm_Grenades[i];
    }
    return 0;
}
/* 0x4ce830, updates the thrown bombs of cm_Grenades: one whose path has ended (flag 0x20 of D3ANIM.flags2d) explodes
   (smoke, earth and explosion effects at its position), lowers the terrain at its cell by 5 and clears the cell's
   type there (the map rebuilt), sounds "Explosion_s" and hurts every scene character within 100.0 of the cell's
   vertex by a random weapon damage (0x176) scaled by its life value and the distance; returns the number of used
   slots */

int cm_UpdateGrenades()
{
    int cnt;
    AIWEAPON *w;
    struct D3SM *g;
    CMGRENADE *rec;
    AICHAR *c;
    CHARACTER *ch;
    struct { float x, y, z; int w; } a, b;   /* @original the pad word is an int: its 0 stores use the cached zero register */
    D3COLOR col;
    float dist;
    int i, j, t, amt;

    g = (struct D3SM *)d3_DefaultScene->hmap;
    cnt = 0;
    w = (AIWEAPON *)ai_FindWeaponRecord(0x176);
    for (i = 0; i < 32; i++) {
        rec = &cm_Grenades[i];
        if (rec->obj == 0)
            continue;
        cnt++;
        if (!(rec->anim->flags2d & 0x20))
            continue;
        a.x = -243.0f;
        a.y = -90.0f;
        a.z = 190.0f;
        a.w = 0;
        b.x = -50.0f;
        b.y = 0.0f;
        b.z = 38.0f;
        b.w = 0;
        col.r = 0xc0;
        col.g = 0xc0;
        col.b = 0xc0;
        col.a = 0x3f;
        a.x = rec->obj->pos.x;
        a.y = rec->obj->pos.y;
        a.z = rec->obj->pos.z;
        d3t_CreateSmoke(0, (D3VECTOR *)&a, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, (D3VECTOR *)&b, 0x190, 6, col);
        d3t_CreateEarth(0, (D3VECTOR *)&a, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, col);
        d3t_CreateExplosion(0, (D3VECTOR *)&a, 0x1e, 70.0f, 50.0f, 20.0f, 60.0f, 0x3c);
        /* @original the cell index reads x through the table and y through rec: written this way VC6 keeps the
           index as one value for both arrays */
        if (((D3FLMAP *)d3_Terrain)->types[cm_Grenades[i].x + rec->y * ((D3FLMAP *)d3_Terrain)->size] != 0) {
            ((D3FLMAP *)d3_Terrain)->height[cm_Grenades[i].x + rec->y * ((D3FLMAP *)d3_Terrain)->size] -= 5;
            ((D3FLMAP *)d3_Terrain)->types[rec->x + rec->y * ((D3FLMAP *)d3_Terrain)->size] = 0;
            d3fl_MakeDivide((D3FLMAP *)d3_Terrain);
            d3fl_MakeLight((D3FLMAP *)d3_Terrain);
        }
        sb_PlaySample(cm_BombSounds, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        for (j = 0; j < 32; j++) {
            if (ai_SceneCharsT[j] == 0)
                continue;
            ch = ai_SceneCharsT[j]->scene;
            c = ai_SceneCharsT[j];
            if (c->health == 0)
                continue;
            d3sm_GetVertex(g, rec->x, rec->y, (D3VECTOR *)&a);
            a.x -= ch->obj->pos.x;
            a.y -= ch->obj->pos.y;
            a.z -= ch->obj->pos.z;
            dist = (float)sqrt(a.z * a.z + a.y * a.y + a.x * a.x);
            if (dist < 100.0f) {
                t = (int)((cm_Random(w->dmgMax - w->dmgMin) + w->dmgMin) * (c->f01c * 0.01));
                amt = (int)(t * (1.0 - dist * 0.01f));
                ai_SceneCharsT[j]->v024 -= amt;
                if ((int)ai_SceneCharsT[j]->bld == cm_FightAttacker)
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 1);
                else
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 2);
                if (cm_CheckDeath(c) == 0)
                    ch_QueueAnim(ch, "kampf/arm_greifen", 1);
            }
        }
        d3if_Delete(rec->obj);
        rec->obj = 0;
    }
    return cnt;
}
/* 0x4ceb70, the game record the object c stands for (its owner) */

int cm_ObjectOwner(char *c)
{
    return (int)((D3OBJECT *)c)->owner;
}
/* 0x4ceb80, waits for the event id to end, then stops it */

void cm_WaitEventStop(int id)
{
    char *e;

    if (cm_SkipEvents != 0)
        return;
    if (sv_RunFrame(0, 0x7d6c6) != 0) {
        do {
            if (dm_L.rup != 0 || kb_Last != 0)
                break;
            if (evt_FindById(id) == 0)
                return;
        } while (sv_RunFrame(0, 0x7d6c6) != 0);
    }
    e = (char *)evt_FindById(id);
    if (e != 0)
        evt_Stop(e);
    di_ClearButtons();
}
/* 0x4cec00, closes the 32 windows of cm_CharInfoWins */

void cm_CloseCharInfoWins()
{
    int *p;

    for (p = cm_CharInfoWins; (int)p < (int)(cm_CharInfoWins + 0x20); p++) {
        if (*p != -1) {
            d2_RemoveWindow(*p);
            *p = -1;
        }
    }
}
/* 0x4cec30, the info window of the character c: a window 16 pixels around the screen box of its body (at least 110
   wide; pictures 0x7b0 / 0x7b1 in its corners) with the value bar of skill 3 and the health slider (0..100), in a free
   slot of cm_CharInfoWins; first the record of c in a (cm_CutEntrySlot) is looked up or taken */

void cm_OpenCharInfoWin(int a, char *c)
{
    D3RECT r;
    int x0, y0, w, h, i, hp, bar;

    if (d3if_ScreenBox(((AICHAR *)c)->scene->obj, &r) == 0)
        return;
    x0 = r.left - 0x10;
    y0 = r.top - 0x10;
    w = r.right - r.left + 0x20;
    if (w <= 0x6e)
        w = 0x6e;
    h = r.bottom - r.top + 0x20;
    for (i = 0; i < 0x20; i++) {
        if (cm_CharInfoWins[i] != -1)
            continue;
        cm_CutEntrySlot((char *)a, c);
        cm_CharInfoWins[i] = d2_CreateWindow(x0, y0, w, h, 0x10);
        d2_WinOpenPic(0, 0, 0x7b0, d2_LastWindow);
        d2_WinOpenPic(w - 0x18, 0, 0x7b1, d2_LastWindow);
        d2_Wins[d2_LastWindow].font = 0x54;
        sv_ValueBar(4, 4, gm_SkillPoints(c, 3), 0x4df);
        hp = (int)(gm_HealthRatio(c) * 100.0);
        bar = d2_WinOpenSlider(0, 0xe, 0x40, 0, 0x64, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(bar, 0, 0x64, hp);
        return;
    }
}
/* 0x4ced80, refreshes the character info windows: closes them, then opens one for every selected fighter (a: the
   fight record the cut entries go to) */

void cm_UpdateCharInfoWins(int a)
{
    char *p;

    cm_CloseCharInfoWins();
    for (p = cm_Fighters; (int)p < (int)(cm_Fighters + 0x4380); p += 0x21c) {
        if (p[0x188] != 0)
            cm_OpenCharInfoWin(a, p);
    }
}
/* 0x4cedc0, shows the damage v over the character c as a floating number (v relative to its life value f01c, in
   percent) in a free slot of the 16 of cm_DamagePops (kind 1 / 2 drawn with signs, else plain) */

void cm_ShowDamage(char *c, int v, int kind)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].v != 0)
            continue;
        cm_DamagePops[i].v = (int)((double)v / (((AICHAR *)c)->f01c * 0.01));
        cm_DamagePops[i].c = (AICHAR *)c;
        cm_DamagePops[i].t = 64.0f;
        cm_DamagePops[i].kind = kind;
        return;
    }
}
/* 0x4cee20, shows / moves the damage numbers of cm_ShowDamage over their characters (created on first show, kind 1 /
   2 between sign glyphs); each rises half a pixel a step and is removed after 128 steps; a character off screen hides
   its number and ends the pass
   @deep near-byte sweep s22: left the ScreenBox call's two temps swapped (original: c chain in eax, &box
   in ecx). climb round 1: nothing; the body under if (v != 0) instead of continue = control. */

void cm_UpdateDamagePops()
{
    char buf[0x40];
    D3RECT box;
    int i, x, y0;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].v == 0)
            continue;
        if (d3if_ScreenBox(cm_DamagePops[i].c->scene->obj, &box) == 0) {
            if (cm_DamagePops[i].h != -1)
                d2_EnableObject(cm_DamagePops[i].h, 0);
            return;
        }
        x = box.left + 0x10;
        y0 = box.top - 0x30;
        if (cm_DamagePops[i].h == -1) {
            if (cm_DamagePops[i].kind == 1)
                sprintf(buf, "%c-%i%c", 0x5c, cm_DamagePops[i].v, 0x5d);
            else if (cm_DamagePops[i].kind == 2)
                sprintf(buf, "%c-%i%c", 0x5e, cm_DamagePops[i].v, 0x5f);
            else
                sprintf(buf, "-%i", cm_DamagePops[i].v);
            cm_DamagePops[i].h = d2_CreateScreenText(x, (int)(y0 + cm_DamagePops[i].t), buf);
        } else
            d2_MoveObj(x, (int)(y0 + cm_DamagePops[i].t), cm_DamagePops[i].h);
        if (cm_DamagePops[i].h != -1)
            d2_EnableObject(cm_DamagePops[i].h, 1);
        cm_DamagePops[i].t = (float)(cm_DamagePops[i].t - 0.5);
        if (cm_DamagePops[i].t <= 0.0f) {
            d2_RemoveObj(cm_DamagePops[i].h);
            cm_DamagePops[i].v = 0;
            cm_DamagePops[i].h = -1;
            cm_DamagePops[i].c = 0;
        }
    }
}
/* 0x4cef80, clears the 16 damage numbers of cm_DamagePops (no screen text, no value, no character) */

void cm_InitDamagePops()
{
    int i;

    for (i = 0; i < 16; i++) {
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
}
/* 0x4cefa0, removes all damage numbers of cm_DamagePops (their screen texts too) */

void cm_ClearDamagePops()
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].h != -1)
            d2_RemoveObj(cm_DamagePops[i].h);
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
}
/* 0x4cefe0, closes the fight view (cm_FightView) and marks it closed (-1) */

void cm_CloseFightView()
{
    if (cm_FightView != -1) {
        d2_CloseView(cm_FightView);
        cm_FightView = -1;
    }
}
/* 0x4cf000, the per-tick update of the scene characters (ai_SceneChars): on the fight owner's machine, every 0x15e
   ticks, a character whose health is 2..4 loses one point and the damage of weapon 0x16e (the poison, reasoned) is
   sent for it (health 1 is reset to 100); a character holding weapon 0x172 is shown to the others only near them
   (faded by the distance to the nearest one in another building, rest 255) or half-transparent when cm_CutChar
   sees it (0x800); a fighter in cutscene state 2 turns towards its partner; ends with the damage numbers
   @deep deep s44: 41 -> 8 ins, size right. Kept: n = cm_Random(..) + w->dmgMin as one statement (the add
   into eax, 41 -> 33); ang *= 0.25; as its own statement before body = g->obj (fld ang / fmul / fadd
   rotation.y, the fpu hunk); no goto turn: the stateFlags |= 2 written in each arm, the found part under
   if (found != 0) (the scratch rotation from 1095 on). Left (8 ins): the original pushes c->health (xor
   edx / mov dl,[esi+8] / push edx) after the _ftol of the damage, ours before the fild (c2dag: the IL has
   the push before the _ftol call node and the scheduler keeps it; with dmg as its own statement the
   scheduler still hoists it). Tried: dmg local, (int)c->health, ai_SceneCharsT[i]->health, b, the product
   spellings (/ 100.0, c->f01c * n, (float)n: same code; (double), 0.01f, 0.01 * n: fpu hunks), cm_Random
   inline in the call (worse, health loaded before the call); climb 3 rounds from this text: nothing.
   prior: Read whole against 0x4cf000..0x4cf370: the same calls with the same arguments (VecEqual c then o,
   SetObjectAlpha with the info pointer and the D3TEXFLAGS by value, SetObjectRotXYZ with the float angles,
   fabs(ang) > 0.08), constants (255 - 255 * (best * 0.01f)) and stores. Left: the hunk at +0x77..+0xa1, a
   scheduling difference: the original loads c->health (xor edx,edx; mov dl,[esi+8]; push edx) after the
   _ftol of the damage, ours before the fild (xor ecx,ecx; mov cl,[esi+8]; push ecx), with the qword temp
   at [esp+0x20] vs [esp+0x24] by that push; tried v as a separate statement and inline, n int / unsigned:
   the same code. Besides: regs hunks and an fpu hunk (fld rotation.y before / fadd after the fmul 0.25). */

void cm_UpdateSceneChars()
{
    AICHAR *c, *d, *o;
    AIWEAPON *w;
    CMCUTENTRY *e;
    CHARACTER *g;
    D3OBJECT *body;
    D3TEXFLAGS col;
    int i, j, fl, found;
    unsigned int n;
    float best, dist, ang;
    unsigned char b;

    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && d3_Time % 0x15e == 0) {
        for (i = 0; i < 32; i++) {
            c = ai_SceneCharsT[i];
            if (c == 0)
                continue;
            b = c->health;
            if (b > 4 || b <= 0)
                continue;
            b--;
            c->health = b;
            if (b == 1) {
                c->health = 100;
                continue;
            }
            w = (AIWEAPON *)ai_FindWeaponRecord(0x16e);
            n = cm_Random(w->dmgMax - w->dmgMin) + w->dmgMin;
            cm_SendDamage(c->netId, (int)(n * c->f01c * 0.01), c->health);
        }
    }
    for (i = 0; i < 32; i++) {
        c = ai_SceneCharsT[i];
        if (c == 0)
            continue;
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w != 0 && w->id == 0x172) {
            fl = 0;
            if (cm_CutChar != 0)
                fl = ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c);
            if (fl & 0x800) {
                col.all = 0;
                col.b[2] = 0;
                col.b[0] = 0x80;
                d3if_ShowTree(c->scene->obj, 1);
                c->scene->obj->stateFlags &= 0xfd;
                d3_SetObjectAlpha(c->scene->obj, c->scene->obj->info, col);
                c->scene->obj->stateFlags |= 2;
            } else {
                best = 999999.0f;
                found = 0;
                for (j = 0; j < 32; j++) {
                    d = ai_SceneCharsT[j];
                    if (d != 0 && cm_OtherBuilding((char *)c, (char *)d) != 0 && d->health != 0) {
                        dist = cm_CharObjDistXZ((char *)c, (char *)d->scene->obj);
                        if (dist < 100.0f) {
                            found = 1;
                            if (dist < best)
                                best = dist;
                        }
                    }
                }
                d3if_ShowTree(c->scene->obj, found);
                if (found != 0) {
                    col.all = 0;
                    col.b[2] = 0;
                    col.b[0] = (unsigned char)(int)(255.0f - 255.0f * (best * 0.01f));
                    c->scene->obj->stateFlags &= 0xfd;
                    d3_SetObjectAlpha(c->scene->obj, c->scene->obj->info, col);
                    c->scene->obj->stateFlags |= 2;
                }
            }
        }
        e = (CMCUTENTRY *)cm_FightCutEntry((char *)c);
        if (e != 0 && e->state == 2 && e->turn != 0) {
            o = (AICHAR *)cm_FindFighter(e->stateArg);
            if (o != 0) {
                if (d3_VecEqual(&c->scene->obj->pos, &o->scene->obj->pos, 10.0f) == 0) {
                    ang = d3_YawTo(c->scene->obj, &o->scene->obj->pos.x);
                    g = c->scene;
                    if (g->actions == 0 || (g->actions->type != 0x2d && g->actions->type != 0x3a)) {
                        if (fabs(ang) > 0.08) {
                            ang *= 0.25;
                            body = g->obj;
                            d3_SetObjectRotXYZ(body, body->rotation.x, body->rotation.y + ang, body->rotation.z);
                        }
                    }
                }
            }
        }
    }
    cm_UpdateDamagePops();
}
/* 0x4cf370, draws the scroll arrows (sprite ids 0 .. 7 of the sheet d2_GetPic(0x52c)): the mouse hit tests and
   whether the camera's preset views d3_Camera->views[1] .. [4] are set (their position not d3_Origin) pick the
   pressed (2) or the free (1) look; the top and bottom arrows sit 75 screen units in from the edge (the top one's
   sprite 30 units tall) */

void cm_DrawScrollArrows()
{
    int up = 0, e = 0, a = 0, b = 0;
    int t;

    t = d2_GetPic(0x52c);
    if (sv_BorderViewX == -1) {
        up = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            up = 2;
    } else if (sv_BorderViewX == 1) {
        e = 1;
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            e = 2;
    } else if (sv_BorderViewX == 0) {
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            e = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            up = 1;
    }
    if (sv_BorderViewY == -1) {
        a = 1;
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            a = 2;
    } else if (sv_BorderViewY == 1) {
        b = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            b = 2;
    } else if (sv_BorderViewY == 0) {
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            a = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            b = 1;
    }
    d2_SetClipRect(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h);
    if (a == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 525.0f - 30.0f), d2_ScreenSurf, t, 0);
    else if (a == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 525.0f - 30.0f), d2_ScreenSurf, t, a);
    if (e == 2)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, e);
    else if (e == 1)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 3);
    if (up == 2)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 4);
    else if (up == 1)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 5);
    if (b == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 75.0f), d2_ScreenSurf, t, 6);
    else if (b == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 75.0f), d2_ScreenSurf, t, 7);
}
/* 0x4cf670, the mouse pick: the object under the cursor (ai_PickedObject; with characters selected and nothing picked,
   the ground cell under the cursor is looked up, else the pick marker is hidden) and the cursor look d2_PointerLook:
   by the weapon of the selected characters cm_CutChar sees (0x160 -> 0x18 .. 0x176 -> 0x25; more than one: 0x23) over
   a character it does not see (or with key 0x36 down), "sp_ESCAPE" 0x1c, "sp_CONQUER" 0x1b, "WARE" 0xe;
   cm_HoverObject keeps the object the look was decided for */

void cm_UpdatePointer()
{
    int i, n = 0, cur = 0x23;
    AICHAR *c;
    AIWEAPON *w;
    struct D3HMAP *map;
    int z, x;
    D3VECTOR4 v;

    map = d3_DefaultScene->hmap;
    ai_PickedObject = 0;
    d2_PointerLook = 0;
    if (dm_L.x > 0 && dm_L.x < d2_ScreenW && dm_L.y > 0 && dm_L.y < d2_ScreenH &&
        d2_PointerObj == -1) {
        ai_PickedObject = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x40, 1);
        if (ai_PickedObject == 0 && cm_SelectedCount != 0) {
            if ((d3sm_PickK(map, dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z) & 0xff) != -1) {
                d3sm_GetVertex(map, x, z, (D3VECTOR *)&v);
                v.y += 10.0f;
                d2_PointerLook = 0;
            }
        } else if (cm_PickMarker != 0) {
            d3if_Show(cm_PickMarker, 0);
        }
    }
    if (ai_PickedObject == 0) {
        cm_HoverObject = 0;
        ai_PickedChar = 0;
        return;
    }
    for (i = 0; i < 31; i++) {
        c = ai_SceneCharsT[i];
        if (c != 0 && c->selected != 0) {
            if (ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800) {
                if (kb_Down[0x36] == 0) {
                    w = (AIWEAPON *)cm_CharWeapon((char *)c);
                    if (w != 0) {
                        if (w->id == 0x160)
                            cur = 0x18;
                        else if (w->id == 0x174)
                            cur = 0x19;
                        else if (w->id == 0x16e)
                            cur = 0x1e;
                        else if (w->id == 0x154)
                            cur = 0x1a;
                        else if (w->id == 0x158)
                            cur = 0x1f;
                        else if (w->id == 0x15e)
                            cur = 0x21;
                        else if (w->id == 0x156)
                            cur = 0x23;
                        else if (w->id == 0x176)
                            cur = 0x25;
                    }
                    n++;
                }
            }
        }
    }
    if (n > 1)
        cur = 0x23;
    if (ai_PickedObject != 0 && d2_PointerObj == -1) {
        if (strcmp(ai_PickedObject, "sp_ESCAPE") == 0 && n != 0)
            d2_PointerLook = 0x1c;
        else if (strncmp(ai_PickedObject, "sp_CONQUER", 10) == 0 && n != 0)
            d2_PointerLook = 0x1b;
        else if (strncmp(ai_PickedObject, "WARE", 4) == 0 && n != 0)
            d2_PointerLook = 0xe;
        else if (((D3OBJECT *)ai_PickedObject)->kind == 2 && n != 0) {
            if (!(ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)((D3OBJECT *)ai_PickedObject)->owner) & 0x800) ||
                kb_Down[0x36] != 0)
                d2_PointerLook = cur;
        } else
            d2_PointerLook = 0;
        cm_HoverObject = ai_PickedObject;
        return;
    }
    d2_PointerLook = 0;
}

#define G19_E(e, o) (*(int *)((e) + (o)))

/* 0x4cf9c0, refreshes the portrait boxes cm_Portraits: after a click a portrait's checked state becomes its
   character's selection; each box of a character whose body is of type 1 hides its window and frame, the others
   show the selection, the name with the health in percent ("%1N1 %i%"), the picture of the weapon (replaced when the
   weapon changed) and, for a kind 1 / 2 weapon, the count left; cm_SelectedCount counts the selected ones */

void cm_UpdatePortraits()
{
    char buf[0x100];
    int i;
    AICHAR *c;
    AIWEAPON *w;
    AIOBJ *item;

    if (d2_ClickedId != -1) {
        for (i = 0; i < 6; i++) {
            if (cm_Portraits[i].win != -1 && d2_PointerObj == cm_Portraits[i].portrait) {
                if (d2_Objs[cm_Portraits[i].portrait].checked != 0)
                    cm_Portraits[i].c->selected = 1;
                else
                    cm_Portraits[i].c->selected = 0;
            }
        }
    }
    d2_ClearPointerItems();
    cm_SelectedCount = 0;
    for (i = 0; i < 6; i++) {
        if (cm_Portraits[i].win == -1)
            continue;
        c = cm_Portraits[i].c;
        if (c->scene->obj->type == 1) {
            d2_EnableObject(d2_Wins[cm_Portraits[i].win].parent, 0);
            d2_EnableObject(d2_Wins[cm_Portraits[i].frame].parent, 0);
            continue;
        }
        if (c->selected != 0) {
            d2_AddPointerItem(c->picture, 1);
            d2_SetObjValue(cm_Portraits[i].portrait, 1);
            cm_SelectedCount++;
        } else
            d2_SetObjValue(cm_Portraits[i].portrait, 0);
        txt_Format(buf, (int)"%1N1 %i%", cm_Portraits[i].c->id, (int)(gm_HealthRatio(cm_Portraits[i].c) * 100.0));
        d2_SetObjText(cm_Portraits[i].text, buf);
        w = (AIWEAPON *)cm_CharWeaponAny((char *)cm_Portraits[i].c);
        if (w == 0 || w->id == 0)
            continue;
        if (cm_Portraits[i].weaponPic == -1 || d2_Objs[cm_Portraits[i].weaponPic].pic != w->id + 0xdf) {
            if (cm_Portraits[i].weaponPic != -1)
                d2_RemoveObj(cm_Portraits[i].weaponPic);
            d2_LastWindow = cm_Portraits[i].win;
            cm_Portraits[i].weaponPic = d2_WinOpenPic(0x54, 0xf, w->id + 0xdf, d2_LastWindow);
            d2_ScaleObject(cm_Portraits[i].weaponPic, -2);
        }
        if (cm_Portraits[i].countText != -1 && (w->kind == 1 || w->kind == 2)) {
            item = (AIOBJ *)cm_CharWeaponItem((char *)cm_Portraits[i].c);
            if (item != 0) {
                sprintf(buf, "%i", item->usesLeft);
                d2_SetObjText(cm_Portraits[i].countText, buf);
            } else if (cm_Portraits[i].countText != -1)
                d2_SetObjText(cm_Portraits[i].countText, &ch_EmptyName);
        } else if (cm_Portraits[i].countText != -1)
            d2_SetObjText(cm_Portraits[i].countText, &ch_EmptyName);
    }
}
#define G19_R(id, o, t) (*(t *)(d2_ObjsC + (id) * 0x2ec + (o)))

/* 0x4cfc10, builds the portrait boxes cm_Portraits (at most 6, 0x80 pixels apart) for the characters of
   ai_SceneChars that cm_CutChar sees (0x800): a frame picture, a window with the weapon picture and its uses left text
   (when the character has a weapon), the portrait (checkable), the name text and the value bar of skill 3; then
   cm_UpdatePortraits fills them */

void cm_BuildPortraits()
{
    AICHAR *c;
    AIWEAPON *w;
    int i, n, x;
    float t;

    for (i = 0; i < 6; i++) {
        cm_Portraits[i].win = -1;
        cm_Portraits[i].portrait = -1;
        cm_Portraits[i].unused0c = -1;
        cm_Portraits[i].text = -1;
        cm_Portraits[i].countText = -1;
        cm_Portraits[i].weaponPic = -1;
    }
    x = 0;
    for (i = 0, n = 0; i < 31 && n < 6; i++) {
        c = ai_SceneCharsT[i];
        if (c == 0)
            continue;
        if (!(ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800))
            continue;
        cm_Portraits[n].frame = d2_CreatePicObj(x + 0x18, 0xf, 0x5da);
        cm_Portraits[n].win = d2_CreateWindow(x, 0, 0x80, 0x4b, 0x110);
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w != 0 && w->id != 0) {
            cm_Portraits[n].weaponPic = d2_WinOpenPic(0x54, 0xf, w->id + 0xdf, d2_LastWindow);
            d2_ScaleObject(cm_Portraits[n].weaponPic, -2);
            cm_Portraits[n].countText = d2_CreateWinText(0x54, 0x26, &ch_EmptyName, d2_LastWindow);
            d2_Objs[cm_Portraits[n].countText].font = 0x54;
            d2_Objs[cm_Portraits[n].countText].w = 0x18;
            d2_Objs[cm_Portraits[n].countText].centred = 1;
        }
        cm_Portraits[n].c = c;
        d2_Wins[cm_Portraits[n].win].mt = 0;
        d2_Wins[cm_Portraits[n].win].font = 0x54;
        cm_Portraits[n].portrait = d2_WinOpenPic(0x28, 0x11, c->picture, d2_LastWindow);
        d2_Objs[cm_Portraits[n].portrait].checkable = 1;
        cm_Portraits[n].text = d2_CreateWinText(0, 0, &ch_EmptyName, d2_LastWindow);
        d2_Objs[cm_Portraits[n].text].font = 0x54;
        d2_Objs[cm_Portraits[n].text].w = 0x80;
        d2_Objs[cm_Portraits[n].text].centred = 1;
        t = gm_SkillPoints(c, 3);
        sv_ValueBar(0x24, 0x41, (int)t, 0x4df);
        x += 0x80;
        n++;
    }
    cm_UpdatePortraits();
}
/* 0x4cfe70, removes the portrait boxes cm_Portraits[6] again */

void cm_ClosePortraits()
{
    int i;
    int t;

    for (i = 0; i < 6; i++) {
        if (cm_Portraits[i].win != -1) {
            d2_RemoveWindow(cm_Portraits[i].win);
            t = cm_Portraits[i].frame;
            cm_Portraits[i].win = -1;
            d2_RemoveObj(t);
        }
    }
}
/* 0x4cfeb0, the click on an own character object (ai_PickedObject, D3OBJECT.kind 2) while characters are selected: the first
   selected standing scene character is marked (AICHAR.selected) and gets the click comment "_ARBEITER_KOMMENTARE_KLICK" from
   the attacker's or the defender's voice bank (by its building); the picked object becomes ai_ClickedObject. */

void cm_ClickSceneChar()
{
    AICHAR *t = (AICHAR *)cm_CutChar;
    AICHAR *c, *o, **pp;
    D3OBJECT *sel;

    ai_ClickedObject = 0;
    for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
        c = *pp;
        if (c != 0 && c->selected != 0 && (ai_LedSelectBits(t->id, (char *)c) & 0x800) && c->scene != 0 &&
            c->health != 0)
            break;
    }
    if (dm_L.lup != 0 && d2_PointerObj == -1) {
        sel = (D3OBJECT *)ai_PickedObject;
        if (sel != 0 && sel->kind == 2) {
            o = sel->owner;
            if ((ai_LedSelectBits(t->id, (char *)o) & 0x800) && kb_Down[0x36] == 0 &&
                (c == 0 || cm_OtherBuilding((char *)c, (char *)o) == 0)) {
                cm_ClearMarks();
                o->selected = 1;
                if ((int)o->bld == cm_FightAttacker)
                    snd_QueueVoice(o->id, cm_AttackerClickVoices, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                else
                    snd_QueueVoice(o->id, cm_DefenderClickVoices, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                sel = (D3OBJECT *)ai_PickedObject;
            }
        }
        ai_ClickedObject = (int)sel;
    }
}
/* 0x4cffd0, orders the character c of the cutscene record rec (CMCUTINFO) to attack: its entry (cm_CutEntrySlot, b0c and
   held cleared there) is copied with state 2 and tgt's netId, for the weapon kinds 0x15e / 0x160 also the cell (x, z) of
   tgt's body, for 0x174 / 0x176 that cell alone, and sent as message 0x50 (cm_SendCutEntry); held is set for the
   formation types 1 .. 3. A weapon of those kinds with no uses left shows its txt_WeaponEmpty text instead. -1 when c,
   tgt, its body or the entry is missing; else the original returns nothing (eax as the last call left it) */

int cm_OrderAttack(char *rec, char *c, char *tgt, int x, int z)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    AICHAR *ch = (AICHAR *)c;
    AICHAR *t = (AICHAR *)tgt;
    AIWEAPON *w;
    AIOBJ *item;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    float y;

    if (ch == 0)
        return -1;
    w = (AIWEAPON *)cm_CharWeapon(c);
    if (t == 0 || t->scene->obj == 0)
        return -1;
    if (d3_GetHeightAt(hmap, &t->scene->obj->pos, &x, &z, &y) == 0)
        return -1;
    e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    if (e == 0)
        return -1;
    e->held = 0;
    e->b0c = 0;
    memcpy(&order, e, sizeof(order));
    if (w == 0 || w->id == 0x154 || w->id == 0x156 || w->id == 0x158 || w->id == 0x16e || w->id == 0x172 ||
        w->id == 0) {
        order.charId = ch->netId;
        order.stateArg = t->netId;
        order.state = 2;
    } else if (w->id == 0x15e || w->id == 0x160) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty15e);
            return -1;
        }
        order.charId = ch->netId;
        order.stateArg = t->netId;
        order.x = x;
        order.z = z;
        order.state = 2;
    } else if (w->id == 0x174) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty174);
            return -1;
        }
        order.charId = ch->netId;
        order.stateArg = 0;
        order.x = x;
        order.z = z;
        order.state = 2;
    } else if (w->id == 0x176) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty176);
            return -1;
        }
        order.stateArg = 0;
        order.charId = ch->netId;
        order.x = x;
        order.z = z;
        order.state = 2;
    }
    if (ch->formation == 1 || ch->formation == 2 || ch->formation == 3)
        order.held = 1;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
}
/* 0x4d0200, orders the character c of the cutscene record rec (CMCUTINFO) to flee to the escape object tgt
   ("sp_ESCAPE"): its entry (cm_CutEntrySlot, b0c cleared there) is copied with state 3 and the ground cell of tgt's
   world position in stateArg / x, and sent as message 0x50 (cm_SendCutEntry). -1 when tgt, c or rec is missing or the
   position has no cell; else the original returns nothing (VC6 keeps x, z, y in the dead parameters' slots) */

int cm_OrderEscape(char *rec, char *c, char *tgt)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    D3OBJECT *o = (D3OBJECT *)tgt;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    float v[4];                         /* d3_PointToWorld writes the point and the angle after it */
    int x, z;
    float y;

    if (o == 0 || c == 0 || rec == 0)
        return -1;
    d3_PointToWorld(o, &o->pos, (D3VECTOR *)v);
    if (d3_GetHeightAt(hmap, (D3VECTOR *)v, &x, &z, &y) == 0)
        return -1;
    e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    if (e == 0)
        return -1;
    e->b0c = 0;
    memcpy(&order, e, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.stateArg = x;
    order.state = 3;
    order.x = z;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
}
/* 0x4d02d0, an order kind 4 (the walk to a named dummy) for the character c with the name of the object tgt
   (CMCUTENTRY.dummy) and its ground cell (cellX, cellZ); returns 1, 0 when the point is not on the ground. */

int cm_OrderConquer(char *rec, char *c, char *tgt)
{
    char *sc = (char *)d3_DefaultScene->hmap;
    CMCUTENTRY *q;
    float v[4];
    int x, z, h;

    cm_CharWeapon(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0)
        return 0;
    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    strcpy(q->dummy, tgt);
    q->charId = ((AICHAR *)c)->netId;
    q->state = 4;
    q->cellX = x;
    q->cellZ = z;
    return 1;
}
/* 0x4d0380, an order kind 6 (walk to the goods store) for the character c: its entry (cm_CutEntrySlot, b0c cleared
   there) is copied with state 6, the name of the object tgt (dummy), the ground cell of tgt's point (d3_GetHeightAt's x / z)
   as storeX / storeZ, and sent as message 0x50 (cm_SendCutEntry, whose result it returns). -1 when the point is not on
   the ground or the character c is in a job (AICHAR.v1ac). */

int cm_OrderWare(char *rec, char *c, char *tgt)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    D3OBJECT *o = (D3OBJECT *)tgt;
    CMCUTENTRY *q;
    CMCUTENTRY order;
    float v[4];
    int x, z;
    float y;

    cm_CharWeapon(c);
    d3_PointToWorld(o, &o->pos, (D3VECTOR *)v);
    if (d3_GetHeightAt(hmap, (D3VECTOR *)v, &x, &z, &y) == 0 || ((AICHAR *)c)->v1ac != 0)
        return -1;
    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    memcpy(&order, q, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.state = 6;
    strcpy(order.dummy, tgt);
    order.storeX = (unsigned char)x;
    order.storeZ = (unsigned char)z;
    return cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
}
/* 0x4d0460, orders the character c of the cutscene record rec (CMCUTINFO) to the best ground cell within 8 rings of
   its own (cm_BestCellNear, formation type 4): its entry (cm_CutEntrySlot, b0c cleared there) is copied with state 7 and
   that cell in stateArg / x, and sent as message 0x50 (cm_SendCutEntry). -1 when c's body is not on the ground or no
   cell is found; else cm_SendCutEntry's result (VC6 keeps x in c's dead parameter slot) */

int cm_OrderToBestCell(char *rec, char *c)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    AICHAR *ch = (AICHAR *)c;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    int x, z;
    float y;

    if (d3_GetHeightAt(hmap, &ch->scene->obj->pos, &x, &z, &y) != 0 && cm_BestCellNear(c, x, z, 8, &x, &z) != 0) {
        e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
        e->b0c = 0;
        memcpy(&order, e, sizeof(order));
        order.charId = ch->netId;
        order.stateArg = x;
        order.state = 7;
        order.x = z;
        return cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
    }
    return -1;
}
/* 0x4d0520, the formation type 3's order for the character c: its entry (cm_CutEntrySlot, b0c cleared there) is copied
   with state 8 and no target and sent as message 0x50 (cm_SendCutEntry); case 8 of cm_RunCutOrders then sends fight
   command 6 once the previous order slot is done (the character waits). */

void cm_OrderWait(char *rec, char *c)
{
    CMCUTENTRY *q;
    CMCUTENTRY order;

    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    memcpy(&order, q, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.state = 8;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
}

/* 0x4d0570, the order of a mouse click (dm_L.lup) for the character c of the cutscene record rec (CMCUTINFO): on a
   character's body (D3OBJECT.kind 2) an attack (cm_OrderAttack) when it is not in c's building or shift is down; on
   "sp_ESCAPE" cm_OrderEscape, on "sp_CONQUER.." cm_OrderConquer, on "WARE.." cm_OrderWare; with nothing picked the ground
   cell under the pointer (d3sm_Pick) is sent in state 1 (stateArg / x), else txt_ClickNoGround is shown. Returns 1 when
   an order was given */
char cm_OrderClick(char *rec, char *c)
{
    char ret = 0;
    D3OBJECT *sel;
    char *o;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    int x, z;
    D3SM *map;

    if (dm_L.lup != 0) {
        sel = (D3OBJECT *)ai_ClickedObject;
        if (sel != 0) {
            if (sel->kind == 2) {
                o = (char *)sel->owner;
                if (cm_OtherBuilding(c, o) != 0 || kb_Down[DIK_RSHIFT] != 0) {
                    if (cm_OrderAttack(rec, c, o, 0, 0) != -1)
                        ret = 1;
                }
            } else if (strcmp(sel->name, "sp_ESCAPE") == 0) {
                cm_OrderEscape(rec, c, (char *)sel);
                ret = 1;
            } else if (strncmp(sel->name, "sp_CONQUER", 10) == 0) {
                cm_OrderConquer(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            } else if (strncmp((char *)ai_ClickedObject, "WARE", 4) == 0) {
                cm_OrderWare(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            }
        } else {
            map = (D3SM *)d3_DefaultScene->hmap;
            if (d3sm_Pick(map, dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z)) {
                e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
                e->b0c = 0;
                memcpy(&order, e, sizeof(order));
                order.charId = ((AICHAR *)c)->netId;
                order.stateArg = x;
                order.state = 1;
                order.x = z;
                cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, (int)&order);
                ret = 1;
            } else
                sv_SetStatusText((char *)txt_ClickNoGround);
        }
    }
    return ret;
}
/* 0x4d0740, the per-tick mouse handling of the fight for the cutscene record rec: the left button held starts the
   rubber band (sv_StartBand, the marks cleared first), its release ends it; a click gives every marked scene character
   (AICHAR.selected) that cm_CutChar may command (ai_LedSelectBits 0x800) and that is alive its order (cm_OrderClick),
   answered with the voice "_K_C2C_BEFEHL_ERHALTEN" of its side, and drops the mark. Then the marked characters' bodies
   are greyed (d3t_SetGrey, D3OBJECT.stateFlags bit 2) and the unmarked ones restored, unless the body is fading; a
   right click clears every mark (cm_ClearMarks) */

void cm_FightMouse(char *rec)
{
    AICHAR **pp;
    AICHAR *c;

    if (dm_L.lhold == 0 && sv_BandActive != 0)
        sv_EndBand();
    if (dm_L.lhold != 0 && dm_L.rdown == 0) {
        if (sv_BandActive == 0 && di_Absolute != 0 && d2_PointerObj == -1) {
            cm_ClearMarks();
            sv_StartBand();
        }
    } else if (dm_L.lup != 0) {
        for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
            c = *pp;
            if (c != 0 && c->selected != 0 && (ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800) &&
                c->scene != 0 && c->health != 0) {
                if (cm_OrderClick(rec, (char *)c)) {
                    if (cm_CutChar == (char *)cm_FightAttacker)
                        snd_PlayVoice(c->id, cm_AttackerOrderVoices, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    else
                        snd_PlayVoice(c->id, cm_DefenderOrderVoices, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    c->selected = 0;
                }
            }
        }
    }
    for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
        c = *pp;
        if (c != 0 && !(c->scene->flags.bytes.lo & 0x40)) {
            if (c->selected) {
                if (!(c->scene->obj->stateFlags & 2))
                    d3t_SetGrey(c->scene->obj, 1);
            } else if ((c->scene->obj->stateFlags & 2) == 2)
                d3t_SetGrey(c->scene->obj, 0);
        }
    }
    if (dm_L.rup != 0)
        cm_ClearMarks();
}
/* 0x4d08c0, clears the marks (AICHAR.selected) of the 32 cm_Fighters and the grey highlight of their bodies
   (D3OBJECT.stateFlags bit 2: d3t_SetColor black), then the pointer items, the character info windows and the fight
   view */

void cm_ClearMarks()
{
    AICHAR *r;
    D3OBJECT *body;

    for (r = cm_FightersT; (int)&r->scene < (int)&cm_FightersT[32].scene; r++) {
        if (r->selected != 0)
            r->selected = 0;
        if (r->scene != 0) {
            body = r->scene->obj;
            if ((body->stateFlags & 2) == 2)
                d3t_SetColor(body, 0, 0, 0);
        }
    }
    d2_ClearPointerItems();
    cm_CloseCharInfoWins();
    cm_CloseFightView();
}
/* 0x4d0920, the fight copy of the character with the netId id: the first free record of cm_Fighters (id 0xffff; 0 when
   all 32 are taken) gets a copy of the AICHAR, without its object v1ac and scene character, and a new scene character
   (gm_CreateCharModel at the dummy object, path type 4, flag 0x8000) with its hand object, the speed of its weapon
   (AIWEAPON.speed * 2/7) and "cmb_<netId>" as its body's name; the weapon item's uses are reset to 6 (kind 2) or 15 */

char *cm_AddFighter(int id, char *dummy)
{
    AICHAR *c;
    AIWEAPON *w;
    AIOBJ *item;
    int i;

    c = (AICHAR *)gm_FindChar(id);
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id == 0xffff) {
            memcpy(&cm_FightersT[i], c, sizeof(AICHAR));
            cm_FightersT[i].v1ac = 0;
            cm_FightersT[i].scene = 0;
            gm_CreateCharModel(&cm_FightersT[i], d3_CurrentScene, 0, 0, 0, dummy);
            cm_FightersT[i].scene->pathType = 4;
            cm_FightersT[i].scene->flags.bytes.hi |= 0x80;
            cm_UpdateHandObject((char *)&cm_FightersT[i]);
            w = (AIWEAPON *)cm_CharWeapon((char *)c);
            cm_FightersT[i].scene->walkSpeed = w->speed * 0.2857142857142857;
            sprintf(cm_FightersT[i].scene->obj->name, "cmb_%i", cm_FightersT[i].netId);
            item = (AIOBJ *)cm_CharWeaponItem((char *)c);
            if (item != 0) {
                if (w->kind == 2)
                    item->usesLeft = 6;
                else
                    item->usesLeft = 0xf;
            }
            return (char *)&cm_FightersT[i];
        }
    }
    return 0;
}
/* 0x4d0a30, fills ai_SceneChars (32 pointers, cleared first) with the cm_Fighters whose scene character is in the scene
   d3_DefaultScene, in record order; at most 31 are taken */

void cm_CollectSceneChars()
{
    AICHAR **p;
    AICHAR *r;

    memset(ai_SceneCharsT, 0, sizeof(ai_SceneCharsT));
    p = ai_SceneCharsT;
    for (r = cm_FightersT; (int)&r->scene < (int)&cm_FightersT[32].scene; r++) {
        if ((int)p >= (int)&ai_SceneCharsT[31])
            break;
        if (r->scene != 0 && r->scene->scene == d3_DefaultScene)
            *p++ = r;
    }
}
/* 0x4d0a80, the object walk's callback for the flag dummies: "dummy_sp_ESCAPE" gets a flag object "ub_FLUCHTFLAGGE"
   (named "sp_ESCAPE", the animation "anim_FLUCHTFLAGGE.baf"), "dummy_CONQUER" one "ub_CONQUER" (named
   "sp_CONQUER_<n>"), both kind 4 with mask 1 and kept in cm_FlagObjects[cm_FlagObjectCount++]; returns 1. */

char cm_CreateDummyFlag(char *o)
{
    D3OBJECT *obj = (D3OBJECT *)o;
    D3OBJECT *r;
    float pos[4];
    float rot[3];

    if (strstr(o, "dummy_sp_ESCAPE") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)pos);
        r = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ub_FLUCHTFLAGGE");
        cm_FlagObjects[cm_FlagObjectCount] = r;
        strcpy(r->name, "sp_ESCAPE");
        cm_FlagObjects[cm_FlagObjectCount]->kind = 4;
        cm_FlagObjects[cm_FlagObjectCount]->mask = 1;
        cm_FlagObjects[cm_FlagObjectCount]->stateFlags = (cm_FlagObjects[cm_FlagObjectCount]->stateFlags & 0xf7) | 4;
        cm_FlagObjects[cm_FlagObjectCount]->stateFlags |= 0x40;
        d3t_PlayAnim(cm_FlagObjects[cm_FlagObjectCount], "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
        cm_FlagObjectCount++;
        return 1;
    }
    if (strstr(o, "dummy_CONQUER") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)pos);
        r = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ub_CONQUER");
        cm_FlagObjects[cm_FlagObjectCount] = r;
        d3if_SelectTextureSet(r, (D3SURF *)r->info, 1, 1);
        sprintf(cm_FlagObjects[cm_FlagObjectCount]->name, "sp_CONQUER_%i", cm_FlagObjectCount);
        cm_FlagObjects[cm_FlagObjectCount]->owner = 0;
        cm_FlagObjects[cm_FlagObjectCount]->kind = 4;
        cm_FlagObjects[cm_FlagObjectCount]->mask = 1;
        cm_FlagObjects[cm_FlagObjectCount]->stateFlags = (cm_FlagObjects[cm_FlagObjectCount]->stateFlags & 0xf7) | 4;
        cm_FlagObjectCount++;
    }
    return 1;
}
/* 0x4d0ca0, walks the objects of the scene with the callback 0x4d0a80 (the flag dummies) */

void cm_PlaceFlagObjects()
{
    cm_FlagObjectCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x100, cm_CreateDummyFlag, 0);
}
/* 0x4d0cd0, the first free (-1) netId slot of the 16 of a CMGUARDREC, else -1 */

int cm_FreeNetIdSlot(int *a)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (a[i] == -1)
            return i;
    }
    return -1;
}

/* the record ai_ArmGateGuards fills: a local of the town gate event's run handler (he_RunAmbush passes its
   address); only these two fields are read here, the rest of its layout is not known */
typedef struct CMGUARDREC {
    char pad000[0x80];
    int netIds[16];                 /* 0x080, the netIds of the guards already armed, -1 = a free slot (cm_FreeNetIdSlot) */
    char pad0c0[0x50];
    int kind;                       /* 0x110, the kind of arming last asked for (1 .. 3) */
} CMGUARDREC;
#define ai_KindBCharsT (*(AICHAR *(*)[8])&ai_KindBChars)  /* 0x6f1c04, ai_KindBChars (f3init.h: char *[8]) as the AICHAR pointers it holds */

/* arms the next guard: takes the first free slot i of rec, the character ai_KindBChars[i] gives up its weapons and
   gets the item ITEM, loses its v024 health and its skill sk83 is set to SKILL (a field delta of the character).
   The original repeats this code inline (the four copies of 0x4d0cf0 reload ai_KindBChars[i] each time) */
#define CM_ARM_GUARD(ITEM, SKILL)                                                                             i = cm_FreeNetIdSlot(rec->netIds);                                                                           rec->netIds[i] = ai_KindBCharsT[i]->netId;                                                               cm_DiscardWeapons((char *)ai_KindBCharsT[i]);                                                            cm_SendExSellObject(ai_KindBCharsT[i]->netId, -1, ITEM, 1, ai_OwnPlayer, 0);                              gm_AddHealth(ai_KindBCharsT[i]->id, -ai_KindBCharsT[i]->v024);                                            skill = SKILL;                                                                                            cm_BeginFieldBlock((char *)ai_KindBCharsT[i], ai_KindBCharsT[i]->netId);                                  cm_AddFieldDelta(1, 1, (unsigned short)((char *)ai_KindBCharsT[i] - cm_FieldObject + 0x83), &skill);      cm_SendFieldDeltas()

/* 0x4d0cf0, arms the guards of the town gate event: kind 1 two guards with the item 0x156 (sk83 = 0xa8), kinds 2 / 3
   one with 0x158 and one with 0x160 (sk83 = 0xd2); a nonzero kind is kept in rec->kind (reasoned: the items replace
   the weapons cm_DiscardWeapons sends away, the characters are the kind 0xb ones of ai_KindBChars) */
void ai_ArmGateGuards(CMGUARDREC *rec, unsigned char kind)
{
    unsigned char skill;
    int i;

    if (kind != 0)
        rec->kind = kind;
    if (kind == 1) {
        CM_ARM_GUARD(0x156, 0xa8);
        CM_ARM_GUARD(0x156, 0xa8);
    } else if (kind == 2 || kind == 3) {
        CM_ARM_GUARD(0x158, 0xd2);
        CM_ARM_GUARD(0x160, 0xd2);
    }
}
/* 0x4d0f80, the object walk's callback: an object of kind 3 gets its mask (D3OBJECT.mask) cleared; returns 1 */

char cm_ClearKind3MaskCB(char *o)
{
    D3OBJECT *obj = (D3OBJECT *)o;

    if (obj->kind == 3)
        obj->mask = 0;
    return 1;
}

/* the voice banks of a fight side: KIND is the role's suffix (DIEBE thieves, RAEUBER robbers, SOELDNER mercenaries) */
#define CM_LOAD_VOICES(SIDE, KIND)                                                         \
    cm_##SIDE##ClickVoices = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_KLICK_" KIND ".sbf", 0); \
    cm_##SIDE##AttackVoices = snd_LoadVoiceBank("K_C2C_ANGRIFF_" KIND ".sbf", 0);         \
    cm_##SIDE##OrderVoices = snd_LoadVoiceBank("K_C2C_BEFEHL_ERHALTEN_" KIND ".sbf", 0);  \
    cm_##SIDE##TauntVoices = snd_LoadVoiceBank("K_C2C_HOHN_" KIND ".sbf", 0);             \
    cm_##SIDE##PainVoices = snd_LoadVoiceBank("K_C2C_SCHMERZ_" KIND ".sbf", 0)

/* a fight record as cm_SetupFightScene and its caller 0x4d63e0 read it (the rest of its layout is not known here) */
typedef struct CMFIGHTREC {
    char pad000[0xc];
    int owner;                      /* 0x00c, the player whose fight it is (cm_FightOwner) */
    char pad010[4];
    int placeId;                    /* 0x014, the defender's building: its type picks the scenario, its role the defender voices */
    char pad018[0x18];
    unsigned char nChars;           /* 0x030, the count of charIds */
    char pad031[3];
    union {
        struct {
            int attackerId;         /* 0x034, the attacking character (cm_FightAttacker) */
            int defenderId;         /* 0x038, the defending character (cm_FightDefender) */
        };
        int charIds[16];            /* 0x034, the netIds of the fight's leading characters, -1 = none (cm_CutInfos[i]
                                       is the cutscene record of charIds[i]) */
    };
    char pad074[8];
    unsigned char flags;            /* 0x07c, 1 in the open (8: in the town, else the mountain pass), 4 the mountain pass */
    char pad07d[3];
    int defenders[16];              /* 0x080, the netIds of the defending fighters, -1 = none */
    int attackers[16];              /* 0x0c0, the netIds of the attacking fighters, -1 = none */
    int carts[3];                   /* 0x100, the object ids of the carts in the fight, -1 = none */
    int attackerBldId;              /* 0x10c, the attacker's building: its role picks the attacker voices */
    int escort;                     /* 0x110, the carts' escort: the arming kind ai_ArmGateGuards keeps (0 none, 1 .. 3);
                                       the AI side of a won raid takes the loot when random 0 .. 4 is above it */
} CMFIGHTREC;
#define cm_FightRecordT (*(CMFIGHTREC **)&cm_FightRecord)  /* 0x6f0d04, cm_FightRecord (cm.h: char *) as the fight record it points to */
#define cm_FightAttackerC (*(AICHAR **)&cm_FightAttacker)  /* 0x6f0cec, cm_FightAttacker (cm.h: int) as the character it points to */
#define cm_FightDefenderC (*(AICHAR **)&cm_FightDefender)  /* 0x6f0ce8, cm_FightDefender (cm.h: int) as the character it points to */

/* 0x4d0fa0, sets up the fight scene for the fight record rec: the scenario "Kampfszenario_<place>.ed3" by the flags or
   the type of the defender's building (cm_LoadEventScene; a robbers' camp gets a 15 cells wide border), the fight
   tables cleared, the weapon sounds (kampf\*.sbf), the voices of both sides by their roles (cm_FightRole), the boots
   object, the attackers placed at "dummy_OFFENSE_ENTRY" and the defenders (walking 1.2 times as fast) at
   "dummy_DEFENSE_ENTRY", the scene characters collected, the carts "ob_KARREN_<size>_KAMPF.ogr" put at their
   dummies "dummy_KARREN_0<n>" with up to three of their goods shown as "WARE_0<n>" (shuffled), the border cells of the
   height map set to the ground type 0xd and cm_DefenseFactor from the defender's building */
void cm_SetupFightScene(CMFIGHTREC *rec)
{
    char name[0x100];
    D3VECTOR4 pos;
    char order[9];
    AIOBJ *goods[3];
    int k, n, border, i;
    int *cart;
    AIBLD *b;
    AIOBJ *obj, *load, *w;
    D3OBJECT *o;
    AICHAR *r;
    D3SM *sc;
    AIBTYPE *t;
    char *p;

    border = 10;
    if (rec->flags & 1) {
        if (rec->flags & 8)
            cm_LoadEventScene("Kampfszenario_STADT.ed3");
        else
            cm_LoadEventScene("Kampfszenario_BERGPASS.ed3");
    } else if (rec->flags & 4) {
        cm_LoadEventScene("Kampfszenario_BERGPASS.ed3");
    } else {
        b = (AIBLD *)gm_BuildQuery(1, 1, rec->placeId);
        if (gm_IsHomeType((unsigned char *)b) != 0) {
            t = &ai_BTypes[b->type];
            if (t->kind == 0x10) {
                cm_LoadEventScene("Kampfszenario_RAEUBERLAGER.ed3");
                border = 15;
            } else if (t->kind == 0xc)
                cm_LoadEventScene("Kampfszenario_STEINBRUCH.ed3");
            else if (t->kind == 0xb)
                cm_LoadEventScene("Kampfszenario_WALDSTUECK.ed3");
            else if (t->kind == 0xd)
                cm_LoadEventScene("Kampfszenario_MINE.ed3");
        } else if (ai_BTypes[b->type].kind == 4) {
            if (ai_BTypes[b->type].level == 1)
                cm_LoadEventScene("Kampfszenario_SCHMUGGLERLOCH.ed3");
            else if (ai_BTypes[b->type].level == 2)
                cm_LoadEventScene("Kampfszenario_UNTERSCHLUPF.ed3");
            else if (ai_BTypes[b->type].level == 3)
                cm_LoadEventScene("Kampfszenario_DIEBESGILDE.ed3");
        } else if (ai_BTypes[b->type].kind == 0x13) {
            if (ai_BTypes[b->type].level == 1)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_KLEIN.ed3");
            else if (ai_BTypes[b->type].level == 2)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_MITTEL.ed3");
            else if (ai_BTypes[b->type].level == 3)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_GROSS.ed3");
        }
    }
    sc = (D3SM *)d3_DefaultScene->hmap;
    cm_ApplySeason();
    if (d3_Terrain != 0) {
        D3IF_SCENE(d3_CurrentScene).sky = D3IF_SCENE(0).sky;
        d3_Sky = D3IF_SCENE(0).sky;
        sv_UpdateWeather();
        d3sky_Init(d3_Sky, 1);
    }
    memset(cm_CharInfoWins, -1, 32 * sizeof(int));
    for (i = 0; i < 16; i++) {
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
    memset(&cm_FightersT[32], 0, 0x1000);
    memset(cm_Bombs, 0, sizeof(cm_Bombs));
    memset(cm_Grenades, 0, sizeof(cm_Grenades));
    memset(cm_Fighters, 0, 32 * sizeof(AICHAR));
    for (i = 0; i < 32; i++)
        cm_FightersT[i].id = 0xffff;
    for (i = 0; i < 6; i++)
        cm_Portraits[i].win = -1;
    cm_FightSounds = sb_LoadSampleBank("kampf.sbf", 1);
    cm_CrossbowSounds = sb_LoadSampleBank("kampf\\armbrust.sbf", 1);
    cm_BombSounds = sb_LoadSampleBank("kampf\\bombeundtopf.sbf", 1);
    cm_DaggerSounds = sb_LoadSampleBank("kampf\\dolche.sbf", 1);
    cm_PistolSounds = sb_LoadSampleBank("kampf\\pistole.sbf", 1);
    cm_SwordSounds = sb_LoadSampleBank("kampf\\schwerter.sbf", 1);
    b = (AIBLD *)gm_BuildQuery(1, 1, rec->attackerBldId);
    if (b != 0)
        cm_AttackerRole = cm_FightRole((unsigned char *)b);
    if (cm_AttackerRole == 2) {
        CM_LOAD_VOICES(Attacker, "DIEBE");
    } else if (cm_AttackerRole == 1) {
        CM_LOAD_VOICES(Attacker, "RAEUBER");
    } else if (cm_AttackerRole == 0) {
        CM_LOAD_VOICES(Attacker, "SOELDNER");
    }
    b = (AIBLD *)gm_BuildQuery(1, 1, rec->placeId);
    if (b != 0)
        cm_DefenderRole = cm_FightRole((unsigned char *)b);
    if (cm_DefenderRole != cm_AttackerRole) {
        if (cm_DefenderRole == 2) {
            CM_LOAD_VOICES(Defender, "DIEBE");
        } else if (cm_DefenderRole == 1) {
            CM_LOAD_VOICES(Defender, "RAEUBER");
        } else if (cm_DefenderRole == 0) {
            CM_LOAD_VOICES(Defender, "SOELDNER");
        }
    } else {
        cm_DefenderClickVoices = cm_AttackerClickVoices;
        cm_DefenderAttackVoices = cm_AttackerAttackVoices;
        cm_DefenderOrderVoices = cm_AttackerOrderVoices;
        cm_DefenderTauntVoices = cm_AttackerTauntVoices;
        cm_DefenderPainVoices = cm_AttackerPainVoices;
    }
    cm_BootsObject = d3io_LoadObjectRef("ub_STIEFEL", "ub_STIEFEL");
    if (sc == 0) {
        d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create((struct D3TERRAIN *)d3_Terrain,
                                                             ((D3IFSCENE *)d3_DefaultScene)->mapSize, 2);
        sc = (D3SM *)d3_DefaultScene->hmap;
    }
    cm_FightCellScale = sc->cell.x * 1.1;
    sv_ScrollSpeed = 0.15f;
    sv_CameraEaseOn = 0;
    cm_FightRecord = (char *)rec;
    cm_PlaceFlagObjects();
    cm_FightAttacker = (int)gm_FindChar(rec->attackerId);
    cm_FightDefender = (int)gm_FindChar(rec->defenderId);
    if (ai_Chars[ai_PlayerId].netId == cm_FightOwner && (rec->flags & 1)) {
        cm_BeginSendGroup();
        for (i = 0; i < 3; i++) {
            if (rec->carts[i] != -1)
                cm_SendExSellObject(-2, -1, 0x154, 1, ai_OwnPlayer, 0);
        }
        cm_EndSendGroup();
        if (cm_SendGroupState() == 0) {
            do
                sv_PumpNet();
            while (cm_SendGroupState() == 0);
        }
    }
    for (i = 0; i < 16; i++) {
        if (rec->attackers[i] != -1) {
            r = (AICHAR *)cm_AddFighter(rec->attackers[i], "dummy_OFFENSE_ENTRY");
            r->bld = (AIBLD *)cm_FightAttacker;     /* in a fight: the side's leader */
            if (r->scene != 0)
                d3if_SelectTextureSet(r->scene->obj, (struct D3SURF *)r->scene->obj->info, 5, 1);
        }
    }
    for (i = 0; i < 16; i++) {
        if (rec->defenders[i] != -1) {
            r = (AICHAR *)cm_AddFighter(rec->defenders[i], "dummy_DEFENSE_ENTRY");
            r->bld = (AIBLD *)cm_FightDefender;
            r->scene->walkSpeed *= 1.2;
            if (r->scene != 0)
                d3if_SelectTextureSet(r->scene->obj, (struct D3SURF *)r->scene->obj->info, 4, 1);
        }
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, cm_ClearKind3MaskCB);
    memset(ai_SceneChars, 0, sizeof(ai_SceneChars));
    n = 0;
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id != 0xffff && cm_FightersT[i].scene != 0 &&
            cm_FightersT[i].scene->scene == d3_DefaultScene)
            ai_SceneCharsT[n++] = &cm_FightersT[i];
    }
    for (i = 0; i < 9; i++) {
        sprintf(name, "WARE_0%i", i + 1);
        o = d3_FindObject(0, 0x40, 0, name);
        if (o != 0)
            d3if_Show(o, 0);
    }
    for (i = 0, cart = rec->carts; i < 3; i++, cart++) {
        goods[0] = 0;
        goods[1] = 0;
        goods[2] = 0;
        gm_ShuffledBytes(9, (unsigned char *)order);
        sprintf(name, "dummy_KARREN_0%i", i + 1);
        o = d3_FindObject(0, 0x100, 0, name);
        if (o != 0) {
            if (*cart != -1) {
                obj = (AIOBJ *)gm_Query(0, 2, 7, 1, *cart);
                if (obj == 0) {
                    gm_FindObjectInst(*cart);
                    *cart = -1;
                    continue;
                }
                load = (AIOBJ *)gm_Query((void *)obj->list, 1, 0, 0x1dd);
                n = 0;
                for (w = (AIOBJ *)gm_Query((void *)load->list, 1, 5); n < 3 && w != 0; w = (AIOBJ *)gm_QueryNext())
                    goods[n++] = w;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
                if (obj->type == 0x134)
                    sprintf(name, "%s*ob_KARREN_KLEIN_KAMPF.ogr", ai_DataPath);
                else if (obj->type == 0x135)
                    sprintf(name, "%s*ob_KARREN_MITTEL_KAMPF.ogr", ai_DataPath);
                else if (obj->type == 0x136)
                    sprintf(name, "%s*ob_KARREN_GROSS_KAMPF.ogr", ai_DataPath);
                o = d3_LoadGroup(name, 0, 0);
                d3_SetObjectPos(o, (D3VECTOR *)&pos);
                o->owner = obj;
                for (k = 0, p = order; k < 3; k++) {
                    sprintf(name, "WARE_0%i", *p++ + 1);       /* the shuffled goods places */
                    o = d3_FindObject(0, 0xc0, 0, name);
                    if (o != 0) {
                        if (k > n - 1)
                            d3if_Show(o, 0);
                        else {
                            o->owner = goods[k];
                            d3if_Show(o, 1);
                            o->mask = 1;
                        }
                    }
                }
            } else
                d3if_ShowTree(o, 0);
        }
    }
    d3sm_Build((struct D3SM *)d3_DefaultScene->hmap, (struct D3TERRAIN *)d3_Terrain);
    for (i = 0; i < border; i++) {
        for (k = i; k < sc->size; k++) {
            sc->entries[(sc->size - 1) * i + k].type = 0xd;
            sc->entries[(sc->size - i - 1) * sc->size + k].type = 0xd;
            sc->entries[sc->size * k + i].type = 0xd;
            sc->entries[sc->size * (k + 1) - i - 1].type = 0xd;
        }
    }
    if (b != 0)
        cm_DefenseFactor = (float)gm_BuildingSkill((unsigned char *)b, 6, 1) + 1.4f;
    else
        cm_DefenseFactor = 1.0f;
}
/* 0x4d1a10, ends the fight scene of the fight record rec (the counterpart of cm_SetupFightScene): fades to black, unloads
   the sound banks and voices (the defender's only when its role differs), deletes the flag objects cm_FlagObjects and
   the boots object; every fighter with a scene character carries the health lost in the fight back to its character
   (gm_AddHealth, not for the kind 0xb ones; only in the own fight or without a network connection) and loses the scene
   character; then the scroll speed, the sky, the scene and the caption are put back */

void cm_EndFightScene(CMFIGHTREC *rec)
{
    AICHAR *k;
    int i;

    cm_FadeToBlack();
    if (cm_PickMarker != 0) {
        d3if_Delete((struct D3OBJECT *)cm_PickMarker);
        cm_PickMarker = 0;
    }
    sb_UnloadSampleBank(cm_FightSounds);
    sb_UnloadSampleBank(cm_CrossbowSounds);
    sb_UnloadSampleBank(cm_BombSounds);
    sb_UnloadSampleBank(cm_DaggerSounds);
    sb_UnloadSampleBank(cm_PistolSounds);
    sb_UnloadSampleBank(cm_SwordSounds);
    sb_UnloadSampleBank(cm_AttackerClickVoices);
    sb_UnloadSampleBank(cm_AttackerAttackVoices);
    sb_UnloadSampleBank(cm_AttackerOrderVoices);
    sb_UnloadSampleBank(cm_AttackerTauntVoices);
    sb_UnloadSampleBank(cm_AttackerPainVoices);
    if (cm_AttackerRole != cm_DefenderRole) {
        sb_UnloadSampleBank(cm_DefenderClickVoices);
        sb_UnloadSampleBank(cm_DefenderAttackVoices);
        sb_UnloadSampleBank(cm_DefenderOrderVoices);
        sb_UnloadSampleBank(cm_DefenderTauntVoices);
        sb_UnloadSampleBank(cm_DefenderPainVoices);
    }
    for (i = 0; i < cm_FlagObjectCount; i++)
        d3if_Delete(cm_FlagObjects[i]);
    if (cm_BootsObject != 0) {
        d3io_ReleaseObject(cm_BootsObject);
        cm_BootsObject = 0;
    }
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id != 0xffff && cm_FightersT[i].scene != 0) {
            if (rec->owner == ai_Chars[ai_PlayerId].netId || net_Conn.s == INVALID_SOCKET) {
                k = (AICHAR *)gm_FindChar(cm_FightersT[i].netId);
                if (k->kind != 0xb)
                    gm_AddHealth(k->id, cm_FightersT[i].v024 - k->v024);
            }
            ch_Delete(cm_FightersT[i].scene);
            cm_FightersT[i].scene = 0;
        }
    }
    sv_ScrollSpeed = 1.0f;
    if (d3_Terrain != 0) {
        d3_Sky = 0;
        D3IF_SCENE(d3_CurrentScene).sky = 0;
    }
    cm_LeaveEventScene();
    snd_EndSpecialTrack();
    sv_CameraEaseOn = 1;
    d2_CopyCaption((char *)&ch_EmptyName);
}
/* 0x4d1c20, places the fighters ids[16] (-1 = none) of one side of the fight (side "OFF" / "DEF") on the ground of the
   scene: a fighter with a ranged weapon (the goods 0x174 0x176 0x15e 0x160) at the next dummy "dummy_<side>_FERN_%02i",
   the others at "dummy_<side>_NAHK_%02i", each on the free cell nearest the dummy's height map cell (ch_FindFreeCell),
   turned the way the dummy faces; stops when a dummy lies off the height map */
void cm_PlaceFighters(int *ids, char *side)
{
    int melee, ranged, i;
    int z, cz, cx, x;
    float h;
    D3VECTOR4 pos;                  /* d3_PointToWorld writes past the vector */
    D3VECTOR dir;
    char name[0x100];
    D3SM *sc;
    AICHAR *c;
    int *weapon;
    D3OBJECT *o;

    ranged = 1;
    melee = 1;
    sc = (D3SM *)d3_DefaultScene->hmap;
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = (AICHAR *)cm_FindFighter(ids[i]);
        if (c == 0)
            continue;
        weapon = (int *)cm_CharWeapon((char *)c);
        if (weapon != 0 && (*weapon == 0x174 || *weapon == 0x176 || *weapon == 0x15e || *weapon == 0x160))
            sprintf(name, "dummy_%s_FERN_%02i", side, ranged++);
        else
            sprintf(name, "dummy_%s_NAHK_%02i", side, melee++);
        o = d3_FindObject(0, 0x100, 0, name);
        if (o == 0)
            continue;
        d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
        if (d3_GetHeightAt(sc, (D3VECTOR *)&pos, &x, &z, &h) == 0)
            return;
        ch_FindFreeCell(sc, &c->scene->obj->pos, &c->scene->obj->pos, x, z, &cx, &cz, 2, 0);
        d3sm_GetVertex(sc, cx, cz, (D3VECTOR *)&pos);
        ch_SetPos(c->scene, (D3VECTOR *)&pos, 1);
        {
            D3VECTOR rot;

            rot.x = 0;
            rot.y = 0;
            rot.z = 0;
            d3_DirToWorld((struct D3OBJECT *)o, &d3_ZAxis, &dir);
            rot.y = d3_VecAngle((D3VECTOR *)&d3_ZAxis, &dir);
            d3_SetObjectRot(c->scene->obj, &rot);
        }
    }
}
/* 0x4d1e00, the intro of the fight: a random fight track (when the music is on) and the cutscene "Kampf_Intro_Fluege" */

void cm_FightIntro(char *a)
{
    int r;
    EVTSCRIPT *s;

    d2_CutsceneActive = 1;
    if (snd_MusicEnabled != 0) {
        r = util_Random(100);
        if (r < 0x14)
            snd_PlaySpecialTrack("cd2\\Krieg.mp3");
        else if (r < 0x28)
            snd_PlaySpecialTrack("cd1\\RuestetEuch.mp3");
        else if (r < 0x3c)
            snd_PlaySpecialTrack("cd1\\Flucht.mp3");
        else if (r < 0x50)
            snd_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
        else
            snd_PlaySpecialTrack("cd2\\AmKuehlenGrabe.mp3");
    }
    sv_FrameMask = 0x7d6c6;
    sv_UpdateWeather();
    sv_UpdateSkyPhase(&ai_Time);
    s = evt_Load("cutscenes\\kampf\\Kampf_Intro_Fluege.esc");
    if (s != 0) {
        evt_RunScript(s);
        cm_WaitEventStop(s->id);
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1eb0, the outro: stops the stream cm_FightAmbient and runs the cutscene "Kampf_outro_flug" */

void cm_FightOutro(char *a)
{
    EVTSCRIPT *s;

    if (cm_FightAmbient != 0)
        msx_StopAmbient(cm_FightAmbient);
    sv_FrameMask = 0x7d6c6;
    d2_CutsceneActive = 1;
    s = evt_Load("cutscenes\\kampf\\Kampf_outro_flug.esc");
    if (s != 0) {
        evt_RunScript(s);
        cm_WaitEventStop(s->id);
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1f10, the enemy of the fighter c to go for: among the scene characters ai_SceneChars of another side
   (cm_OtherBuilding) still alive, the one whose distance to c weighted by its health ratio (gm_HealthRatio) is less
   than the plain distance of the one found before (from 1e6 on); 0 when none. The loops for a weapon of kind 2 and
   for the others are the same */

AICHAR *cm_NearestEnemy(AICHAR *c)
{
    float best;
    AICHAR *res;
    int i;
    float d;

    best = 1000000.0f;
    res = 0;
    if (((AIWEAPON *)cm_CharWeapon((char *)c))->kind != 2) {
        for (i = 0; i < 32; i++) {
            if (ai_SceneCharsT[i] != 0 && cm_OtherBuilding((char *)c, (char *)ai_SceneCharsT[i]) != 0 &&
                ai_SceneCharsT[i]->health != 0) {
                d = cm_CharObjDistXZ((char *)c, (char *)ai_SceneCharsT[i]->scene->obj);
                if (gm_HealthRatio((short *)ai_SceneCharsT[i]) * d < best) {
                    res = ai_SceneCharsT[i];
                    best = d;
                }
            }
        }
    } else {
        for (i = 0; i < 32; i++) {
            if (ai_SceneCharsT[i] != 0 && cm_OtherBuilding((char *)c, (char *)ai_SceneCharsT[i]) != 0 &&
                ai_SceneCharsT[i]->health != 0) {
                d = cm_CharObjDistXZ((char *)c, (char *)ai_SceneCharsT[i]->scene->obj);
                if (gm_HealthRatio((short *)ai_SceneCharsT[i]) * d < best) {
                    res = ai_SceneCharsT[i];
                    best = d;
                }
            }
        }
    }
    return res;
}
/* 0x4d2010, the threat at the height map vertex (a, b) for the fighter c: every living enemy within limit of it adds
   70 / its distance, every other living fighter of c's own side takes it off (the sides are the attackers / defenders
   of the fight record cm_FightRecord) */

float cm_ThreatAt(AICHAR *c, int a, int b, float limit)
{
    float threat;
    D3VECTOR4 v;
    D3SM *sc;
    int *own, *enemies;
    AICHAR *o;
    float d, dx, dy, dz;
    int i;

    sc = (D3SM *)d3_DefaultScene->hmap;
    threat = 0.0f;
    if ((int)c->bld == cm_FightAttacker) {      /* in a fight: the side's leader */
        own = cm_FightRecordT->attackers;
        enemies = cm_FightRecordT->defenders;
    } else {
        own = cm_FightRecordT->defenders;
        enemies = cm_FightRecordT->attackers;
    }
    d3sm_GetVertex(sc, a, b, (D3VECTOR *)&v);
    for (i = 0; i < 16; i++) {
        if (enemies[i] != -1) {
            o = (AICHAR *)cm_FindFighter(enemies[i]);
            if (o != 0 && o->health > 0) {
                dx = v.x - o->scene->obj->pos.x;
                dy = v.y - o->scene->obj->pos.y;
                dz = v.z - o->scene->obj->pos.z;
                d = sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    threat += 70.0f / d;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (own[i] != -1) {
            o = (AICHAR *)cm_FindFighter(own[i]);
            if (o != 0 && o->health > 0 && o != c) {
                dx = v.x - o->scene->obj->pos.x;
                dy = v.y - o->scene->obj->pos.y;
                dz = v.z - o->scene->obj->pos.z;
                d = sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    threat -= 70.0f / d;
            }
        }
    }
    return threat;
}
/* 0x4d2170, the safest cell around (cx, cy) for the fighter c: walks the diamond rings 0 .. r - 1 around it inside the
   height map, rates every cell whose ground type is not 0 / 0xd (the border) with cm_ThreatAt (limit r cells: r * the
   cell size * 2) and puts the least rated one into (*ox, *oy); returns 0 when no cell was rated
   @deep s40: min-ternary hi / hic (sc->size - 1 < cy + k ? sc->size - 1 : cy + k, as cm_FindNearCell) and
   k = 0 before best (the k / best stores in the original order) took 41 -> 2 ins, size 348 -> 352 right.
   Left: the two reloads of the k-loop latch's reload block (the row loop's exit, its own 2-instruction
   block) in another order: the original loads cy into ebp before the strength-reduced cy - k (edx); ours
   the IV first (split pieces, pass 2 ids k#154#2 before cy#2; the block keeps list order). Same hunk as
   cm_MostThreatenedCellNear; flip.py finds no single change that flips it here. (declaration set: no
   declaration set makes this text byte (every symbol-count shift 0..63 at the unit's start and before the
   function) */

int cm_BestCellNear(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    D3SM *sc;
    float best;
    float f;
    int k, row, col, hi, hic, m, type;

    sc = (D3SM *)d3_DefaultScene->hmap;
    k = 0;
    best = 1000000000.0f;
    for (; k < r; k++) {
        hi = sc->size - 1 < cy + k ? sc->size - 1 : cy + k;
        for (row = cy - k < 0 ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = sc->size - 1 < cx + m ? sc->size - 1 : cx + m;
            for (col = cx - m < 0 ? 0 : cx - m; col <= hic; col++) {
                type = sc->entries[sc->size * row + col].type;
                if (type != 0 && type != 0xd) {
                    f = cm_ThreatAt((AICHAR *)c, col, row, r * sc->cell.x * 2);
                    if (f < best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    return best != 1000000000.0f;
}
/* 0x4d22d0, the most threatened cell around (cx, cy) for the fighter c: the rings of cm_BestCellNear, the cell with the
   greatest cm_ThreatAt rating goes into (*ox, *oy); returns 1 when that rating is above 5 (0 too when no cell rated
   above 0)
   @deep s40: min-ternary hi / hic (sc->size - 1 < cy + k ? sc->size - 1 : cy + k, as cm_FindNearCell) and
   k = 0 before best (best's 0 stored from k's zero register) took 41 -> 2 ins, size right. Left: the two
   reloads of the k-loop latch's reload block (the row loop's exit, its own 2-instruction block) in another
   order: the original loads cy into ebp before the strength-reduced cy - k (edx); ours the IV first. They
   are split pieces (pass 2 ids: k#154#2 id b before cy#2 id 16) and the block keeps list order. Tried
   without result: if-forms of the row / col lower bounds (draft), lo / loc locals (= or far), every
   operand order of the two ternaries, the final test spelled 7 ways (best != 0 / 0.0 / ==, return of the
   &&), best = 0 / 0.0, climb 3 rounds; flip.py names only the float 0.0f constants (best = 1.0f, or the
   final compare's 0.0f: identical instructions with another constant) and the 0 of the row bound, i.e. the
   const-0 web decides the split order. Probes of cy / cy - k references move far more than the order.
   (declaration set: no declaration set makes this text byte (every symbol-count shift 0..63 at the unit's
   start and before the function) */

int cm_MostThreatenedCellNear(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    D3SM *sc;
    float best;
    float f;
    int k, row, col, hi, hic, m, type;

    sc = (D3SM *)d3_DefaultScene->hmap;
    k = 0;
    best = 0.0f;
    for (; k < r; k++) {
        hi = sc->size - 1 < cy + k ? sc->size - 1 : cy + k;
        for (row = cy - k < 0 ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = sc->size - 1 < cx + m ? sc->size - 1 : cx + m;
            for (col = cx - m < 0 ? 0 : cx - m; col <= hic; col++) {
                type = sc->entries[sc->size * row + col].type;
                if (type != 0 && type != 0xd) {
                    f = cm_ThreatAt((AICHAR *)c, col, row, r * sc->cell.x * 2);
                    if (f > best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    if (best != 0.0f && best > 5.0f)
        return 1;
    return 0;
}
/* 0x4d2450, a callback of the object walk: collects the objects named "sp_ESCAPE" into list[cm_PointCount++] */

char cm_CollectEscapeObject(char *o, int *list)
{
    if (stricmp(o, "sp_ESCAPE") == 0) {
        list[cm_PointCount] = (int)o;
        cm_PointCount++;
    }
    return 1;
}
/* 0x4d2490, the closest "sp_ESCAPE" object (0x4d2450) to the character c, or 0 (limit 1e6) */

char *cm_NearestEscapeObject(char *c)
{
    int list[0x20];
    D3VECTOR4 v;
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    D3OBJECT *m, *o;
    int i;

    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0xc0, cm_CollectEscapeObject, list);
    m = ((AICHAR *)c)->scene->obj;
    d3_PointToWorld(m, &m->pos, (D3VECTOR *)&v);
    for (i = 0; i < cm_PointCount; i++) {
        o = (D3OBJECT *)list[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = (char *)o;
        }
    }
    return res;
}
/* 0x4d2550, a callback: collects the objects "WARE_" whose owner (+0x200, a record with the side in +0xa) is not the side
   of the character c into cm_PointList[cm_PointCount++] */

char cm_CollectWareObject(char *o, char *c)
{
    AICHAR *side = (AICHAR *)((AICHAR *)c)->bld;    /* in a fight bld is the side's leader (cm_AddFighter) */

    if (strncmp(o, "WARE_", 5) == 0 && ((AIOBJ *)((D3OBJECT *)o)->owner)->fightSide != side->id)
        cm_PointList[cm_PointCount++] = o;
    return 1;
}
/* 0x4d25a0, the closest object of 0x4d2550 to the character c, or 0 */

char *cm_NearestWareObject(char *c)
{
    D3VECTOR4 v;
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    D3OBJECT *m, *o;
    int i, n;

    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, cm_CollectWareObject, c);
    m = ((AICHAR *)c)->scene->obj;
    d3_PointToWorld(m, &m->pos, (D3VECTOR *)&v);
    n = cm_PointCount;
    for (i = 0; i < n; i++) {
        o = (D3OBJECT *)cm_PointList[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = (char *)o;
        }
    }
    return res;
}
/* 0x4d2650, a callback: collects the objects "sp_CONQUER*" into cm_PointList[cm_PointCount++] */

char cm_CollectConquerPoint(char *o)
{
    if (strncmp(o, "sp_CONQUER", 10) == 0)
        cm_PointList[cm_PointCount++] = o;
    return 1;
}
/* 0x4d2680, the conquer point (an "sp_CONQUER" object, cm_CollectConquerPoint) to go for: the least distance to the
   fighter c's body, weighted by the point's owner (10 for an owner of another side (cm_OtherBuilding), 15 for an
   owner of c's own side other than c, 25 for c itself); 0 when the point chosen is c's own */

D3OBJECT *cm_NearestConquerPoint(AICHAR *c)
{
    D3VECTOR4 v;
    float best;
    D3OBJECT *res;
    D3OBJECT *body, *o;
    void *owner;
    float d, dx, dy, dz;
    int i;

    best = 1000000.0f;
    res = 0;
    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, cm_CollectConquerPoint, c);
    body = c->model->body;
    d3_PointToWorld(body, &body->pos, (D3VECTOR *)&v);
    for (i = 0; i < cm_PointCount; i++) {
        o = (D3OBJECT *)cm_PointList[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (o->owner != 0 && cm_OtherBuilding((char *)c, o->owner) != 0)
            d = (float)(d * 10.0);
        else if (o->owner != 0 && cm_OtherBuilding((char *)c, o->owner) == 0 && o->owner != c)
            d = (float)(d * 15.0);
        else if (o->owner != 0)
            d = (float)(d * 25.0);
        if (d < best) {
            best = d;
            res = o;
        }
    }
    if (res->owner == c)
        res = 0;
    return res;
}
/* 0x4d27a0, builds the fight AI's order chances cm_OrderChances[6 kinds][5 strength levels][6 orders]: the
   percentages stored one by one, then each row made cumulative (v[k] = v[k] * 0.01 + v[k - 1]), and in each row
   a value equal to an earlier one cleared (an order that adds no chance is never drawn) */

void cm_InitOrderChances()
{
    int k, b, i, j;

    memset(cm_OrderChances, 0, 0x2d0);
    cm_OrderChances[0x0] = 10.0f;
    cm_OrderChances[0x1] = 10.0f;
    cm_OrderChances[0x4] = 30.0f;
    cm_OrderChances[0x5] = 50.0f;
    cm_OrderChances[0x6] = 40.0f;
    cm_OrderChances[0x7] = 20.0f;
    cm_OrderChances[0xa] = 20.0f;
    cm_OrderChances[0xb] = 20.0f;
    cm_OrderChances[0xc] = 80.0f;
    cm_OrderChances[0xd] = 20.0f;
    cm_OrderChances[0x10] = 0;
    cm_OrderChances[0x11] = 0;
    cm_OrderChances[0x12] = 70.0f;
    cm_OrderChances[0x13] = 30.0f;
    cm_OrderChances[0x16] = 0;
    cm_OrderChances[0x17] = 0;
    cm_OrderChances[0x18] = 50.0f;
    cm_OrderChances[0x19] = 50.0f;
    cm_OrderChances[0x1c] = 0;
    cm_OrderChances[0x1d] = 0;
    cm_OrderChances[0x1e] = 10.0f;
    cm_OrderChances[0x1f] = 10.0f;
    cm_OrderChances[0x22] = 30.0f;
    cm_OrderChances[0x23] = 50.0f;
    cm_OrderChances[0x24] = 80.0f;
    cm_OrderChances[0x25] = 20.0f;
    cm_OrderChances[0x28] = 0;
    cm_OrderChances[0x29] = 0;
    cm_OrderChances[0x2a] = 80.0f;
    cm_OrderChances[0x2b] = 20.0f;
    cm_OrderChances[0x2e] = 0;
    cm_OrderChances[0x2f] = 0;
    cm_OrderChances[0x30] = 60.0f;
    cm_OrderChances[0x31] = 40.0f;
    cm_OrderChances[0x34] = 0;
    cm_OrderChances[0x35] = 0;
    cm_OrderChances[0x36] = 50.0f;
    cm_OrderChances[0x37] = 50.0f;
    cm_OrderChances[0x3a] = 0;
    cm_OrderChances[0x3b] = 0;
    cm_OrderChances[0x3c] = 5.0f;
    cm_OrderChances[0x3e] = 5.0f;
    cm_OrderChances[0x40] = 20.0f;
    cm_OrderChances[0x41] = 70.0f;
    cm_OrderChances[0x42] = 50.0f;
    cm_OrderChances[0x44] = 10.0f;
    cm_OrderChances[0x46] = 20.0f;
    cm_OrderChances[0x47] = 20.0f;
    cm_OrderChances[0x48] = 80.0f;
    cm_OrderChances[0x4a] = 20.0f;
    cm_OrderChances[0x4c] = 0;
    cm_OrderChances[0x4d] = 0;
    cm_OrderChances[0x4e] = 70.0f;
    cm_OrderChances[0x50] = 30.0f;
    cm_OrderChances[0x52] = 0;
    cm_OrderChances[0x53] = 0;
    cm_OrderChances[0x54] = 50.0f;
    cm_OrderChances[0x56] = 50.0f;
    cm_OrderChances[0x58] = 0;
    cm_OrderChances[0x59] = 0;
    cm_OrderChances[0x5a] = 20.0f;
    cm_OrderChances[0x5c] = 80.0f;
    cm_OrderChances[0x5e] = 0;
    cm_OrderChances[0x5f] = 0;
    cm_OrderChances[0x60] = 30.0f;
    cm_OrderChances[0x62] = 70.0f;
    cm_OrderChances[0x64] = 0;
    cm_OrderChances[0x65] = 0;
    cm_OrderChances[0x66] = 40.0f;
    cm_OrderChances[0x68] = 60.0f;
    cm_OrderChances[0x6a] = 0;
    cm_OrderChances[0x6b] = 0;
    cm_OrderChances[0x6c] = 60.0f;
    cm_OrderChances[0x70] = 0;
    cm_OrderChances[0x71] = 0;
    cm_OrderChances[0x76] = 0;
    cm_OrderChances[0x77] = 0;
    cm_OrderChances[0x7a] = 0;
    cm_OrderChances[0x80] = 0;
    cm_OrderChances[0x86] = 0;
    cm_OrderChances[0x88] = 0;
    cm_OrderChances[0x89] = 0;
    cm_OrderChances[0x8c] = 0;
    cm_OrderChances[0x8e] = 0;
    cm_OrderChances[0x8f] = 0;
    cm_OrderChances[0x92] = 0;
    cm_OrderChances[0x94] = 0;
    cm_OrderChances[0x95] = 0;
    cm_OrderChances[0x98] = 0;
    cm_OrderChances[0x9e] = 0;
    cm_OrderChances[0xa4] = 0;
    cm_OrderChances[0xa6] = 0;
    cm_OrderChances[0xa7] = 0;
    cm_OrderChances[0xaa] = 0;
    cm_OrderChances[0xac] = 0;
    cm_OrderChances[0xad] = 0;
    cm_OrderChances[0xb0] = 0;
    cm_OrderChances[0xb2] = 0;
    cm_OrderChances[0xb3] = 0;
    cm_OrderChances[0x6e] = 40.0f;
    cm_OrderChances[0x72] = 70.0f;
    cm_OrderChances[0x74] = 30.0f;
    cm_OrderChances[0x78] = 20.0f;
    cm_OrderChances[0x7c] = 10.0f;
    cm_OrderChances[0x7d] = 70.0f;
    cm_OrderChances[0x7e] = 30.0f;
    cm_OrderChances[0x82] = 20.0f;
    cm_OrderChances[0x83] = 50.0f;
    cm_OrderChances[0x84] = 100.0f;
    cm_OrderChances[0x8a] = 100.0f;
    cm_OrderChances[0x90] = 100.0f;
    cm_OrderChances[0x96] = 20.0f;
    cm_OrderChances[0x9a] = 10.0f;
    cm_OrderChances[0x9b] = 70.0f;
    cm_OrderChances[0x9c] = 30.0f;
    cm_OrderChances[0xa0] = 20.0f;
    cm_OrderChances[0xa1] = 50.0f;
    cm_OrderChances[0xa2] = 100.0f;
    cm_OrderChances[0xa8] = 100.0f;
    cm_OrderChances[0xae] = 100.0f;
    for (b = 0; b < 6 * 5; b += 5) {
        for (i = 0; i < 5; i++) {
            for (k = 0; k < 6; k++) {
                cm_OrderChances[(b + i) * 6 + k] *= 0.01;
                if (k != 0)
                    cm_OrderChances[(b + i) * 6 + k] += cm_OrderChances[(b + i) * 6 + k - 1];
            }
        }
    }
    for (b = 0; b < 6; b++) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 6; j++) {
                for (k = j + 1; k < 6; k++) {
                    if (cm_OrderChances[(b * 5 + i) * 6 + k] == cm_OrderChances[(b * 5 + i) * 6 + j])
                        cm_OrderChances[(b * 5 + i) * 6 + k] = 0;
                }
            }
        }
    }
}
/* 0x4d2c00, the row of cm_OrderChances the fight AI draws the orders of side from: the kind by the fight flags (bit 0 /
   1 / 2, then side being the attacker cm_FightAttacker or the defender cm_FightDefender; 6 for neither), the level
   0 .. 4 by the ratio of the side's own strength to the enemy's (the sums of cm_FightStrength over the fighters still
   standing, health neither 0 nor 4) against 1.75 / 1.25 / 0.75 / 0.5; the defender holding against the attacker's
   "sp_CONQUER" flags gets level 1 for one taken, 0 for two or three */

float *cm_OrderChancesFor(int side)
{
    int kind;
    float def, att, r;
    D3VECTOR4 v;
    char *o;
    int i, cnt, lvl;

    kind = 6;
    if ((cm_FightRecordT->flags & 1) && cm_FightAttacker == side)
        kind = 0;
    else if ((cm_FightRecordT->flags & 1) && cm_FightDefender == side)
        kind = 1;
    else if ((cm_FightRecordT->flags & 2) && cm_FightAttacker == side)
        kind = 2;
    else if ((cm_FightRecordT->flags & 2) && cm_FightDefender == side)
        kind = 3;
    else if ((cm_FightRecordT->flags & 4) && cm_FightAttacker == side)
        kind = 4;
    else if ((cm_FightRecordT->flags & 4) && cm_FightDefender == side)
        kind = 5;
    def = 0.0f;
    for (i = 0; i < 16; i++) {
        if (cm_FightRecordT->defenders[i] != -1) {
            o = cm_FindFighter(cm_FightRecordT->defenders[i]);
            if (((AICHAR *)o)->health != 0 && ((AICHAR *)o)->health != 4)
                def = cm_FightStrength(o) + def;
        }
    }
    att = 0.0f;
    for (i = 0; i < 16; i++) {
        if (cm_FightRecordT->attackers[i] != -1) {
            o = cm_FindFighter(cm_FightRecordT->attackers[i]);
            if (((AICHAR *)o)->health != 0 && ((AICHAR *)o)->health != 4)
                att = cm_FightStrength(o) + att;
        }
    }
    cnt = 0;
    for (i = 0; i < cm_FlagObjectCount; i++) {
        if (strstr(cm_FlagObjects[i]->name, "sp_CONQUER") != 0) {
            d3_PointToWorld(cm_FlagObjects[i], &cm_FlagObjects[i]->pos, (D3VECTOR *)&v);
            o = cm_FlagObjects[i]->owner;
            if (o != 0 && (int)((AICHAR *)o)->bld == cm_FightAttacker)
                cnt++;
        }
    }
    if (side == cm_FightAttacker)
        r = att / def;
    else
        r = def / att;
    if (r >= 1.75f)
        lvl = 4;
    else if (r >= 1.25f)
        lvl = 3;
    else if (r >= 0.75f)
        lvl = 2;
    else if (r >= 0.5f)
        lvl = 1;
    else
        lvl = 0;
    if (side == cm_FightDefender) {
        if (cnt == 1)
            lvl = cnt;
        else if (cnt == 2 || cnt == 3)
            lvl = 0;
    }
    return &cm_OrderChances[(kind * 5 + lvl) * 6];
}
/* 0x4d2e30, how well the fighter c suits the order 0 .. 5 (the fight AI gives each order drawn to the fighter rated
   highest): 0 (attack) its strength (cm_FightStrength) times 1, 0.5 when it has an order already, 0.2 with a used-up
   weapon; 1 the inverse (1 / strength times 0.4, 0.5 with another order, 1 while on order 1 or with a used-up weapon);
   2 1 on order 2, else 0.5; 3 1; 4 / 5 0.2 on order 2, else 1 / its health ratio (gm_HealthRatio); 0 for others */

float cm_OrderRating(AICHAR *c, int order)
{
    float r;
    float f;
    int kind;
    unsigned char cur;

    r = 0.0f;
    switch (order) {
    case 0:
        if (c->formation == 0)
            f = 1.0f;
        else
            f = 0.5f;
        kind = ((AIWEAPON *)cm_CharWeapon((char *)c))->kind;
        if ((kind == 1 || kind == 2) && ((AIOBJ *)cm_CharWeaponItem((char *)c))->usesLeft <= 0)
            f = 0.2f;
        r = cm_FightStrength((char *)c) * f;
        break;
    case 1:
        cur = c->formation;
        if (cur == 1)
            f = 1.0f;
        else {
            f = 0.4f;
            if (cur != 0)
                f = 0.5f;
        }
        kind = ((AIWEAPON *)cm_CharWeapon((char *)c))->kind;
        if ((kind == 1 || kind == 2) && ((AIOBJ *)cm_CharWeaponItem((char *)c))->usesLeft <= 0)
            f = 1.0f;
        r = f / cm_FightStrength((char *)c);
        break;
    case 2:
        if (c->formation != 2)
            r = 0.5f;
        else
            r = 1.0f;
        break;
    case 4:
    case 5:
        if (c->formation == 2)
            r = 0.2f;
        else
            r = 1.0f / gm_HealthRatio((short *)c);
        break;
    case 3:
        r = 1.0f;
        break;
    }
    return r;
}
/* 0x4d2f80, gives the fighters of the leading character side (cm_FightAttacker: the attackers, else the defenders of
   the fight record) their orders: once per fighter an order 0 .. 5 is drawn (util_RandomF against
   cm_OrderChancesFor) and given (AICHAR.formation) to the fighter still without a new order rated best for it
   (cm_OrderRating), a fighter holding an order already only on a second roll above 0.25; then each fighter carries
   out its order (cm_CarryOutOrder, with the leader's cutscene record cm_CutInfos) */

void cm_GiveFightOrders(int side)
{
    AICHAR *c[16];
    int *ids;
    float *row;
    char *cut;
    AICHAR *sel;
    float r, best, w;
    int i, j, order, n, left, selidx;

    n = 0;
    selidx = 0;
    order = 0;
    cut = 0;
    row = cm_OrderChancesFor(side);
    for (i = 0; i < cm_FightRecordT->nChars; i++) {
        if (cm_FightRecordT->charIds[i] == ((AICHAR *)side)->netId) {
            cut = (char *)&cm_CutInfos[i];
            break;
        }
    }
    if (side == cm_FightAttacker)
        ids = cm_FightRecordT->attackers;
    else
        ids = cm_FightRecordT->defenders;
    for (i = 0; i < 16; i++) {
        if (ids[i] != -1) {
            c[i] = (AICHAR *)cm_FindFighter(ids[i]);
            n++;
        } else
            c[i] = 0;
    }
    for (left = n; left > 0; left--) {
        r = util_RandomF();
        for (j = 0; j < 6; j++) {
            if (r <= row[j]) {
                order = j;
                break;
            }
        }
        best = -1.0f;
        for (i = 0; i < 16; i++) {
            if (c[i] != 0) {
                w = cm_OrderRating(c[i], order);
                if (w > best) {
                    best = w;
                    selidx = i;
                }
            }
        }
        sel = c[selidx];
        if (sel->formation == 0 || cm_RandomFloat() > 0.25) {
            sel->formation = (unsigned char)order;
            c[selidx] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            cm_CarryOutOrder(cut, cm_FindFighter(ids[i]));
    }
}
/* 0x4d3120, the formation order for the character c of the fight record rec by its formation type (AICHAR.formation):
   0 the best free cell near the character for a kind 2 weapon (cm_MostThreatenedCellNear), else the closest selected
   enemy (cm_NearestEnemy), then cm_OrderAttack; 1 the closest "WARE_" object (cm_NearestWareObject) -> cm_OrderWare3;
   2 the closest "sp_CONQUER" point (cm_NearestConquerPoint) -> cm_OrderConquer; 3 cm_OrderWait; 4 cm_OrderToBestCell;
   5 the closest "sp_ESCAPE" object (cm_NearestEscapeObject) -> cm_OrderEscape */

void cm_CarryOutOrder(char *rec, char *c)
{
    D3SM *sc;
    AIWEAPON *k;
    char *o;
    int x, y, h;
    float v[4];

    if (c != 0 && ((AICHAR *)c)->scene != 0) {
        if (((AICHAR *)c)->health == 0)
            return;
        if (((AICHAR *)c)->scene->obj->type == 1)
            return;
    }
    switch (((AICHAR *)c)->formation) {
    case 0:
        k = (AIWEAPON *)cm_CharWeapon(c);
        if ((k->kind == 1 || k->kind == 2) && ((AIOBJ *)cm_CharWeaponItem(c))->usesLeft <= 0)
            return;
        if (k->kind == 2) {
            sc = (D3SM *)d3_DefaultScene->hmap;
            if ((char)d3_GetHeightAt(sc, &((AICHAR *)c)->scene->obj->pos, &x, &y, &h) == 0)
                return;
            if (cm_MostThreatenedCellNear(c, x, y, (int)(500.0f / sc->cell.x), &x, &y) == 0)
                return;
            cm_OrderAttack(rec, c, 0, x, y);
            return;
        }
        o = (char *)cm_NearestEnemy((AICHAR *)c);
        if (o == 0)
            return;
        cm_OrderAttack(rec, c, o, 0, 0);
        return;
    case 5:
        o = cm_NearestEscapeObject(c);
        if (o == 0)
            return;
        cm_OrderEscape(rec, c, o);
        return;
    case 1:
        o = cm_NearestWareObject(c);
        if (o == 0)
            return;
        cm_OrderWare(rec, c, o);
        return;
    case 2:
        o = (char *)cm_NearestConquerPoint((AICHAR *)c);
        if (o == 0)
            return;
        cm_OrderConquer(rec, c, o);
        return;
    case 4:
        cm_OrderToBestCell(rec, c);
        return;
    case 3:
        cm_OrderWait(rec, c);
        return;
    }
}
/* 0x4d3300, a character falls: the sound "Koerperfaellt" and a blood puddle "ub_Blutlache" (random turn) at its place */

void cm_FallDownAction(char *c)
{
    D3VECTOR rot;
    D3OBJECT *b;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    rot.y = (float)(cm_Random(0x168) * 3.14159274f * 0.005555555555555556);
    sb_PlaySample(0, "Koerperfaellt", 1, 0x3f, 0x7f, 0, 0);
    b = d3_CreateObject(0, &((CHACTION *)c)->ch->obj->pos, &rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4d3390, the death of the fighter c once its health ratio (gm_HealthRatio) is below 0.05: blood spray at the body
   (d3t_CreateBlood), its actions stopped and the fall queued (cm_FallDownAction with the sound "kampf/todumfallen"),
   health 0, the body's kind cleared, the cry "Schmerzschrei", v024 set below zero (-life - 20 with a chance of a half,
   text 0x1181, else 10 - life, text 0x1182) and the text shown; a ware it carried (v1ac) is put at the body, shown and
   handed to the defender. Returns 1, 0 while it lives */

int cm_CheckDeath(AICHAR *c)
{
    char buf[0x100];
    CHACTION *a;
    D3OBJECT *ware;

    cm_CharWeapon((char *)c);
    if (gm_HealthRatio((short *)c) < 0.05) {
        d3t_CreateBlood(c->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
        ch_StopActions(c->scene);
        a = ch_AddActionCB(c->scene, (CHACTFUNC)cm_FallDownAction, 0x14, 0x36, 1);
        strcpy(a->u.name, "kampf/todumfallen");
        c->health = 0;
        c->scene->obj->kind = 0;
        sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
        if (cm_Random(100) > 50) {
            c->v024 = (int)(-(int)c->f01c - 20.0);
            txt_Format(buf, 0x1181, c->id);
        } else {
            c->v024 = (int)(10.0 - (int)c->f01c);
            txt_Format(buf, 0x1182, c->id);
        }
        ware = (D3OBJECT *)c->v1ac;
        if (ware != 0) {
            d3_SetWorldPosRot(ware, (float *)&c->scene->obj->pos, &c->scene->obj->matrix);
            d3if_Show(ware, 1);
            c->v1ac = 0;
            ((AIOBJ *)cm_ObjectOwner((char *)ware))->fightSide = ((AICHAR *)cm_FightDefender)->id;
        }
        sv_SetStatusText(buf);
        return 1;
    }
    return 0;
}

#define G19_FIGHTER_SYNC(ch)                                                          \
    r = (AIOBJ *)cm_CharWeaponItem((char *)(ch));                                          \
    if (r != 0) {                                                                     \
        int vv = -1;                                                                  \
        cm_BeginFieldBlock((char *)r, r->objId);                                            \
        cm_AddFieldValues(4, 1, (unsigned short)((char *)&r->usesLeft - cm_FieldObject), &vv); \
        cm_SendFieldDeltas();                                                               \
    }

/* 0x4d3550, the hit action of the fight (queued on the hitter, a's hit record): a hit counts only while the victim's
   body is within the range of the hitter's weapon; a used-up weapon item of the player's fight is synced (its uses
   field -1); a damage shows (cm_ShowDamage) with the pain voice "_K_C2C_SCHMERZ" of the victim's side (3 in 10) or
   the cry "Schmerzschrei" (3 in 10); while both stand, the sound of the hitter's weapon ("Treffer" for a hit, else
   "Daneben_s" of the sword / dagger bank, "schuss_s" for the pistol), then, unless the victim dies (cm_CheckDeath),
   blood and a random flinch "kampf/arm_greifen" / "kampf/bauch_greifen" for a hit (the weapon 0x16e stuns: health 4,
   text 0x1188), "kampf/ausweichen_seite" (pistol, 0x160) or "kampf/stich_abwehr" for a miss, when the victim is idle */

void cm_HitAction(CHACTION *a)
{
    int damage, ok;
    AIWEAPON *hw, *vw;
    char anims[2][0x20] = {"kampf/arm_greifen", "kampf/bauch_greifen"};
    char buf[0x100];
    AICHAR *victim, *hitter;
    AIOBJ *item;
    int rnd;

    hitter = a->hit.hitter;
    ok = 1;
    victim = a->hit.victim;
    damage = a->hit.damage;
    vw = (AIWEAPON *)cm_CharWeapon((char *)victim);
    hw = (AIWEAPON *)cm_CharWeapon((char *)hitter);
    ch_GetHMap(victim->scene);
    if (hw == 0 || cm_CharObjDistXZ((char *)hitter, (char *)victim->scene->obj) > hw->range) {
        damage = 0;
        ok = 0;
    }
    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && (hw->kind == 1 || hw->kind == 2)) {
        item = (AIOBJ *)cm_CharWeaponItem((char *)hitter);
        if (item != 0) {
            int v = -1;
            cm_BeginFieldBlock((char *)item, item->objId);
            cm_AddFieldValues(4, 1, (unsigned short)((char *)item - cm_FieldObject + 0x20), &v);
            cm_SendFieldDeltas();
        }
    }
    if (damage != 0) {
        rnd = cm_Random(10);
        if ((int)victim->bld == cm_FightAttacker)
            cm_ShowDamage((char *)victim, damage, 1);
        else
            cm_ShowDamage((char *)victim, damage, 2);
        if (rnd > 8) {
            if (a->hit.sideId != ((AICHAR *)cm_FightAttacker)->netId)
                snd_QueueVoice(victim->id, cm_AttackerPainVoices, "_K_C2C_SCHMERZ", -1, 500);
            else
                snd_QueueVoice(victim->id, cm_DefenderPainVoices, "_K_C2C_SCHMERZ", -1, 500);
        } else if (rnd > 5)
            sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    }
    if (victim->health != 0 && hitter->health != 0 && ok != 0) {
        if (hw->id == 0x156 || hw->id == 0x158) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            else
                sb_PlaySample(cm_SwordSounds, "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0x154 || hw->id == 0x16e || hw->id == 0x172) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            else
                sb_PlaySample(cm_DaggerSounds, "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0x15e)
            sb_PlaySample(cm_PistolSounds, "schuss_s", 1, 0x3f, 0x7f, 0, 0);
        else if (hw->id == 0x160) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0)
            sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
        if (cm_CheckDeath(victim) == 0) {
            if (damage != 0) {
                d3t_CreateBlood(victim->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
                if (victim->scene->actions == 0 || victim->scene->actions->type == 0x3b) {
                    ch_StopActions(victim->scene);
                    ch_QueueAnimSpeed(victim->scene, anims[cm_Random(2)], 1, 1.4f);
                }
                if (hw->id == 0x16e) {
                    victim->health = 4;
                    txt_Format(buf, 0x1188, vw->text054, victim->id);
                    sv_SetStatusText(buf);
                }
            } else if (victim->scene->actions == 0 || victim->scene->actions->type == 0x3b) {
                ch_StopActions(victim->scene);
                if (hw->id == 0x15e || hw->id == 0x160)
                    ch_QueueAnimSpeed(victim->scene, "kampf/ausweichen_seite", 1, 1.4f);
                else
                    ch_QueueAnimSpeed(victim->scene, "kampf/stich_abwehr", 1, 1.4f);
            }
        }
    }
}
/* 0x4d3980, an explosion event: the fuse sound "ZuendschnurBrennt_l_01", the attacker's worker count synced, 0x4ce290 */

void cm_LayBombAction(char *ev)
{
    AICHAR *att = ((CHACTION *)ev)->pot.thrower;
    AIWEAPON *k;
    AIOBJ *r;

    k = (AIWEAPON *)cm_CharWeapon((char *)att);
    cm_LayBomb(&att->scene->obj->pos);
    sb_PlaySample(cm_BombSounds, "ZuendschnurBrennt_l_01", 3, 0x3f, 0x7f, 0, 0);
    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && (k->kind == 1 || k->kind == 2)) {
        G19_FIGHTER_SYNC(att);
    }
}
/* 0x4d3a30, a bomb event: for the player's side the thrower's used weapon item is synced (its uses field -1),
   then a grenade (cm_ThrowGrenade, 0x4ce5d0) is thrown from the thrower's body to the cell aimed at */

void cm_ThrowBombAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;
    AICHAR *att = b->bomb.thrower;
    AIOBJ *r;

    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner) {
        G19_FIGHTER_SYNC(att);
    }
    cm_ThrowGrenade(&att->scene->obj->pos, b->bomb.x, b->bomb.y);
}
/* 0x4d3ac0, a fighter's cheer (fight command 1, cm_FightSlotCheer with "gestik/jubeln"): the cheering character's
   model is hidden (ch_SetVisible, 0x402480) */

void cm_CheerAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;

    ch_SetVisible(b->cheer.fighter->scene, 0);
}
/* 0x4d3ae0, the end of a pick-up (fight command 4, cm_FightSlotPickUp): the object picked up gets the fight's
   attacker side as its owner (AIOBJ.fightSide), is carried by the fighter (AICHAR.v1ac) and hidden on the ground */

void cm_PickUpAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;
    AICHAR *att = b->pickUp.fighter;
    D3OBJECT *o = b->pickUp.object;
    AIOBJ *p;

    p = (AIOBJ *)cm_ObjectOwner((char *)o);
    if (p != 0) {
        p->fightSide = ((AICHAR *)cm_FightAttacker)->id;
        att->v1ac = (int)o;
    }
    d3if_ShowTree(o, 0);
}
/* 0x4d3b30, decides the fight of rec: the winner known already (cm_FightWinner), else the attacker cm_FightAttacker
   when no attacker of rec stands any more (health neither 0 nor 4, the body not hidden: type 1), the defender
   cm_FightDefender when no defender does, and the defender too when the attacker's side owns every "sp_CONQUER" flag;
   the winner's cutscene entry (cm_CutEntrySlot, the record of the first leader of cm_FightRecord) is sent with state 5
   (cm_SendCutEntry). Returns the winner, 0 while the fight goes on */

AICHAR *cm_CheckFightWinner(CMFIGHTREC *rec)
{
    AICHAR *winner;
    int flags, owned;
    int nDef, nAtt;
    D3VECTOR4 v;
    CMCUTENTRY entry;
    AICHAR *c;
    CMCUTENTRY *slot;
    int i, n;

    winner = 0;
    owned = 0;
    flags = 0;
    if (cm_FightWinner != 0)
        return (AICHAR *)cm_FightWinner;
    nDef = 0;
    for (i = 0; i < 16; i++) {
        if (rec->defenders[i] != -1) {
            c = (AICHAR *)cm_FindFighter(rec->defenders[i]);
            if (c->health != 0 && c->health != 4 && c->scene->obj->type != 1)
                nDef++;
        }
    }
    nAtt = 0;
    for (i = 0; i < 16; i++) {
        if (rec->attackers[i] != -1) {
            c = (AICHAR *)cm_FindFighter(rec->attackers[i]);
            if (c->health != 0 && c->health != 4 && c->scene->obj->type != 1)
                nAtt++;
        }
    }
    if (nAtt <= 0)
        winner = (AICHAR *)cm_FightAttacker;
    else if (nDef <= 0)
        winner = (AICHAR *)cm_FightDefender;
    for (i = 0; i < cm_FlagObjectCount; i++) {
        if (strstr(cm_FlagObjects[i]->name, "sp_CONQUER") != 0) {
            d3_PointToWorld(cm_FlagObjects[i], &cm_FlagObjects[i]->pos, (D3VECTOR *)&v);
            flags++;
            c = cm_FlagObjects[i]->owner;
            if (c != 0 && (int)c->bld == cm_FightAttacker)
                owned++;
        }
    }
    if (flags != 0 && owned == flags)
        winner = (AICHAR *)cm_FightDefender;
    if (winner != 0) {
        n = cm_FightRecordT->nChars;
        for (i = 0; i < n && cm_FightRecordT->charIds[i] == -1; i++)
            ;
        slot = (CMCUTENTRY *)cm_CutEntrySlot((char *)&cm_CutInfos[i], (char *)winner);
        entry = *slot;
        entry.charId = winner->netId;
        entry.state = 5;
        cm_SendCutEntry(cm_CutInfos[i].playerId, (int)&entry);
    }
    return winner;
}


/* 0x4d3d10, loot of a won fight: a random share (0.07 + 0.001 * 0 .. 49 of the loser's goods value 0x509e60, at
   most 0x9c400) is moved from the loser's record to the winner's (money message 0x4c47e0), then each object of the
   loser's stock (0x4ffe90 list 1) is moved to the winner (0x4c4880, 80 .. 99 % of its count) and its price added; the
   value goes to the winner's record (0x506430, AIFIN.lootTakings) */
int cm_LootStock(AIBLD *a, AIBLD *b, AICHAR *d, AICHAR *t)
{
    AIOBJ *oa, *ob, *q;
    int cnt, v;

    oa = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x116);
    ob = (AIOBJ *)gm_Query(a->objList, 2, 6, 0, 0x2a);
    v = gm_GetGoodsValue((char *)t);
    v = v < 0 ? 0 : v;
    v = v < 0x9c400 ? v : 0x9c400;
    v = (int)(((double)(unsigned int)cm_Random(0x32) * 0.001 + 0.07) * v);
    cm_SendMoveGoods(d->netId, t->netId, ai_OwnPlayer, v);
    if (ob != 0 && oa != 0) {
        for (q = (AIOBJ *)gm_Query(ob->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
            cnt = (int)(((double)(unsigned int)(cm_Random(0x14) + 0x50) * 0.01) * (double)q->count);
            if (cnt < 1)
                cnt = 1;
            cm_SendExSellObject(oa->objId, ob->objId, q->id, cnt, ai_OwnPlayer, 0);
            v = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * cnt + v);
        }
    }
    q = (AIOBJ *)gm_PlayerRecord((char *)d);
    if (q != 0)
        ((AIFIN *)q)->lootTakings += v;
    return v;
}
/* 0x4d3ea0, the loot of the carts of a won fight: for each of the 3 cart ids (CMFIGHTREC.carts), the goods of the
   cart (0x4ffe90 chains) go to the winner (0x4c4880, at least 1, f of the count; a free pick-up with b == 0) and their
   price is added; with no winner record half of the value goes to the money message 0x4c47e0; the value goes to the
   record of the attacker side (0x506430, AIFIN.lootTakings) */
int cm_LootCarts(CMFIGHTREC *a, AIBLD *b, AICHAR *d, int unused, float f)
{
    AIOBJ *ob, *q, *p;
    int cnt, i;
    int dmg = 0;
    float x;

    if (cm_FightAttacker == 0 || cm_FightDefender == 0)
        return 0;
    {
        ob = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x116);
        for (i = 0; i < 3; i++) {
            if (a->carts[i] != -1) {
                p = (AIOBJ *)gm_Query(0, 2, 7, 1, a->carts[i]);
                p = (AIOBJ *)gm_Query(p->list, 1, 0, 0x1dd);
                for (q = (AIOBJ *)gm_Query(p->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
                    gm_ObjectBuildingOwner((unsigned short *)q);
                    x = (float)q->count * f;
                    if (1.0f > x)
                        x = 1.0f;
                    cnt = (int)x;
                    if (ob != 0)
                        cm_SendExSellObject(ob->objId, q->ownerId, q->id, cnt, ai_OwnPlayer, 0);
                    else
                        cm_SendExSellObject(-1, q->ownerId, q->id, cnt, ai_OwnPlayer, 0);
                    dmg = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * cnt + dmg);
                }
                if (ob == 0) {
                    dmg = (int)(dmg * 0.5);
                    cm_SendMoveGoods(d->netId, -1, ai_OwnPlayer, dmg);
                }
                q = (AIOBJ *)gm_PlayerRecord((char *)cm_FightAttacker);
                if (q != 0)
                    ((AIFIN *)q)->lootTakings += dmg;
            }
        }
    }
    return dmg;
}
/* 0x4d4050, the fight role of a building kind (first byte of its 0x24d-byte record): 0x13 -> 0, 0x10 -> 1, 4 -> 2, else 0 */

int cm_FightRole(unsigned char *b)
{
    AIBTYPE *t = &ai_BTypes[*b];

    if (t->kind == 0x13)
        return 0;
    if (t->kind == 0x10)
        return 1;
    return t->kind == 4 ? 2 : 0;
}
/* 0x4d4090, the loot list of a won fight: for each of the 3 cart ids at +0x100 of cm_FightRecord the goods (mode 0 all,
   1 only those of the player, 2 only the others) get a cell of 5 per row (120 x 96 pixels from x, y): their picture
   (0x525 frame, 0xdf + id), the count "%i " plus a suffix, the value "%T"; returns the sum of the values */

int cm_LootList(int x, int y, int mode)
{
    int i;
    int j = 0;
    int total = 0;
    int px, py;
    int t, t2;
    AIOBJ *q;
    char buf[0x100];

    for (i = 0; i < 3; i++) {
        if (cm_FightRecordT->carts[i] == -1)
            continue;
        q = (AIOBJ *)gm_Query(0, 2, 7, 1, cm_FightRecordT->carts[i]);
        q = (AIOBJ *)gm_Query(q->list, 1, 0, 0x1dd);
        for (q = (AIOBJ *)gm_Query(q->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
            if (mode != 0 && !(mode == 1 && q->fightSide == ai_PlayerId) &&
                !(mode == 2 && q->fightSide != ai_PlayerId))
                continue;
            px = x + (j % 5) * 120;
            py = y + (j / 5) * 96;
            d2_WinOpenPic(px, py, 0x525, d2_LastWindow);
            d2_WinOpenPic(px, py, q->id + 0xdf, d2_LastWindow);
            t = d2_CreateWinText(px - 0x10, py - 0xf, &ch_EmptyName, d2_LastWindow);
            d2_Objs[t].font = 0x56;
            d2_Objs[t].w = 0x50;
            d2_Objs[t].centred = 1;
            sprintf(buf, "%i ", q->count);
            strcat(buf, txt_AmountUnit);
            d2_SetObjText(t, buf);
            t2 = d2_CreateWinText(px - 0x10, py + 0x34, &ch_EmptyName, d2_LastWindow);
            d2_Objs[t2].font = 0x56;
            d2_Objs[t2].w = 0x50;
            d2_Objs[t2].centred = 1;
            txt_Format(buf, (int)"%T", (int)gm_GoodsPrice(q->id, ai_OwnPlayer) * q->count);
            d2_SetObjText(t2, buf);
            total = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * q->count + total);
            j++;
        }
    }
    return total;
}
/* 0x4d4340, the portrait row of the fight: for each of the 16 ids (-1 = none) a column (width = (0x6ed980 + 8 - 0x30) /
   (count + 1), 6 per row, rows of 100 pixels from x, y) with the frame 0x5da, the portrait, the health bar (0x504e00),
   the name and health text ("%1N1 %i%"), the building icon and the state icon (0x3b / 0x37) of the character */

void cm_FightPortraits(int *ids, int x, int y)
{
    int i, n = 0;
    int w, px, py, v, t, f, ic;
    AICHAR *c;
    AIWEAPON *o;
    char buf[0x100];

    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            n++;
    }
    w = (d2_LastWindowPtr->w - 0x30) / (n + 1);
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = (AICHAR *)cm_FindFighter(ids[i]);
        px = (i % 6 + 1) * w + x;
        py = y + i / 6 * 100;
        d2_WinOpenPic(px - 0x10, py, 0x5da, d2_LastWindow);
        d2_WinOpenPic(px - 4, py, c->picture, d2_LastWindow);
        v = gm_SkillPoints(c, 3);
        sv_ValueBar(px, py + 0x3c, (int)(double)(float)v, 0x4eb);
        t = d2_CreateWinText(px - 0x24, py - 0x11, &ch_EmptyName, d2_LastWindow);
        d2_Objs[t].w = 0x78;
        d2_Objs[t].centred = 1;
        d2_Objs[t].font = 0x56;
        txt_Format(buf, (int)"%1N1 %i%", c->id, (int)(gm_HealthRatio(c) * 100.0));
        d2_SetObjText(t, buf);
        o = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (o != 0 && o->id != 0) {
            ic = d2_WinOpenPic(px + 0x28, py + 0xf, o->id + 0xdf, d2_LastWindow);
            d2_ScaleObject(ic, -2);
        }
        if (c->health == 0) {
            f = (int)-c->f01c;
            if ((double)c->v024 == (double)f - 20.0)
                d2_WinOpenPic(px, py + 6, 0x3b, d2_LastWindow);
            else
                d2_WinOpenPic(px, py + 6, 0x37, d2_LastWindow);
        }
    }
}
/* 0x4d4540, the dialog "cutscenes\combat_start" that ends a fight (a: the fight record; its flags bits 2 / 1 / 4 pick the
   text): when the attacker is a player's character, the line of the winner (texts 0x1138 .. 0x113c, 0x1125 + role),
   the portrait row (cm_FightPortraits) and two buttons (1: settle); then, for a raid in the open (flags bit 0) not
   settled yet: a player defender sees the carts' loot (cm_LootList, their value at 10 / 50 / 80 per cent) and picks the
   share 10 / 50 / 90 (taken when random 10 .. 99 is at most it, else refused), or the AI side picks one at random when
   it fights no defenders or its random 0 .. 4 is above the escort. The loot goes through cm_LootCarts into
   cm_FightLootValue (-1: refused), cm_FightSettled = 1 when it was taken. Returns 1 when the fight goes on. */

int cm_FightStartDialog(CMFIGHTREC *a)
{
    int res = 1;
    int st = 0;
    int line = 0;
    int win, role, i, n, r, v, sel, k, total;
    int b1, b2, b3, b4;
    unsigned char *place, *home;
    float fv;
    AIJOB *job;
    char *bld;
    char tb[0x400];
    char tc[0x400];

    win = d2_OpenForm(0, 0, "cutscenes\\combat_start");
    d2_PlaceMainWindow(win, 3);
    cm_FightSettled = 0;
    place = gm_BuildQuery(1, 1, a->placeId);
    home = gm_BuildQuery(1, 1, a->attackerBldId);
    role = cm_FightRole(home);
    d2_SetForm(win, 1);
    if (cm_FightAttackerC->kind == 6) {
        if (a->flags & 2) {
            io_PrintText(0x1138, role + 0x1125, home, cm_FightDefenderC->id, place);
            d2_SetForm(win, 2);
            line = io_PrintText(0x113a, cm_FightDefenderC->id, place);
        } else if (a->flags & 1) {
            for (i = 0; i < 3; i++) {
                if (a->carts[i] != -1)
                    st++;
            }
            fv = gm_GetSkill((char *)cm_FightAttacker, 3);
            io_PrintText(0x112e, home, cm_FightDefenderC->id);
            d2_SetForm(win, 2);
            io_PrintText(0x112f, st, 0xc10);
            if (fv >= 0.5) {
                if (a->escort != 0)
                    io_PrintText(0x1134);
                else
                    io_PrintText(0x1135);
            }
            if (fv >= 0.75) {
                v = gm_SkillPoints(cm_FightDefender, 3);
                r = (int)(v * 0.023809524f);
                if (r > 4)
                    r = 4;
                r = r < 0 ? 0 : r;
                io_PrintText(0x1131, r + 0x1128);
            }
            if (cm_FightDefenderC->sex == 0)
                line = io_PrintText(0x1133, 0x1136);
            else
                line = io_PrintText(0x1133, 0x1137);
        } else if (a->flags & 4) {
            io_PrintText(0x113b, home, cm_FightDefenderC->id);
            d2_SetForm(win, 2);
            line = io_PrintText(0x113c, cm_FightDefenderC->id, place);
        }
        d2_SetForm(win, 1);
        io_PrintText(0x1132);
        cm_FightPortraits(a->attackers, 0, 0xb4);
        d2_GetWinObj(win, 2, line);
        line++;
        d2_GetWinObj(win, 2, line);
        while (sv_RunFrame((int)cm_FightStartDialog, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                cm_FightSettled = 1;
                res = 0;
            }
            if (d2_ClickedId == 0) {
                res = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                cm_FightSettled = 1;
                res = 0;
                sv_PopRequest = 1;
            }
        }
    }
    if ((a->flags & 1) && cm_FightDefenderC->kind == 6 && cm_FightSettled == 0) {
        AIRECIPE rc = {0};

        total = 0;
        for (i = 0; i < 3; i++) {
            if (a->carts[i] != -1)
                st++;
        }
        job = ((AIOBJ *)gm_Query(0, 2, 7, 1, a->carts[0]))->job24;     /* the job the first cart runs */
        d2_SetForm(win, 1);
        io_PrintText("$C");
        if (job != 0) {
            bld = gm_FindBuildingById(job->cartBld);
            k = 0;
            for (i = 0; i < 16; i++) {
                if (a->attackers[i] != -1)
                    k++;
            }
            if (bld != 0)
                io_PrintText(0x113d, st, 0xc10, bld, cm_FightAttackerC->id, k);
        }
        v = gm_SkillPoints(cm_FightDefender, 3);
        r = (int)(v * 0.023809524f);
        if (r > 4)
            r = 4;
        if (a->escort != 0)
            io_PrintText(0x113e, (r < 0 ? 0 : r) + 0x1128);
        else
            io_PrintText(0x113f);
        cm_LootList(0x10, 0x96, 0);
        for (i = 0; i < 3; i++) {
            n = a->carts[i];
            if (n != -1) {
                bld = (char *)gm_FindObjectInst(n);
                if (bld != 0) {
                    gm_StallGoodsList(bld, (unsigned *)&rc);
                    for (k = 0; k < rc.count; k++)
                        total = (int)(gm_GoodsPrice(rc.item[k], ai_OwnPlayer) * rc.amount[k] + total);
                }
            }
        }
        d2_SetForm(win, 2);
        io_PrintText("$C");
        k = io_PrintText(0x1141, (int)(total * 0.1), (int)(total * 0.5), (int)(total * 0.8));
        b1 = d2_GetWinObj(win, 2, k);
        b2 = d2_GetWinObj(win, 2, k + 1);
        b3 = d2_GetWinObj(win, 2, k + 2);
        line = 0;
        for (i = 0; i < 16; i++) {
            if (a->defenders[i] != -1)
                line++;
        }
        b4 = d2_GetWinObj(win, 2, k + 3);
        if (line == 0)
            d2_EnableObject(b4, 0);
        while (sv_RunFrame((int)cm_FightStartDialog, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                res = 0;
            }
            if (d2_ClickedId == -1)
                continue;
            sel = d2_PointerObj;
            if (sel == b4) {
                sv_PopRequest = 1;
                res = 1;
            } else if (sel == b1 || sel == b2 || sel == b3) {
                st = 0;
                if (sel == b1)
                    st = 10;
                else if (sel == b2)
                    st = 50;
                else if (sel == b3)
                    st = 90;
                if (util_Random(0x5a) + 10 <= st) {
                    home = gm_BuildQuery(1, 1, a->attackerBldId);
                    txt_Format(tb, 0x1142);
                    cm_MessageBox(tb, 0);
                    fv = (float)(st * 0.01);
                    sv_PopRequest = 1;
                    res = 0;
                    cm_FightSettled = 1;
                    cm_FightLootValue = cm_LootCarts(cm_FightRecordT, (AIBLD *)home, cm_FightAttackerC, cm_FightDefender, fv);
                } else {
                    txt_Format(tc, 0x1143);
                    cm_MessageBox(tc, 0);
                    sv_PopRequest = 1;
                    res = 1;
                }
            }
        }
    } else if ((a->flags & 1) && cm_FightDefenderC->kind != 6 && cm_FightSettled == 0) {
        n = 0;
        for (i = 0; i < 16; i++) {
            if (a->defenders[i] != -1)
                n++;
        }
        if (n == 0 || util_Random(5) > a->escort) {
            st = 0;
            r = util_Random(4);
            if (r == 0)
                st = 10;
            else if (r == 1)
                st = 90;
            else if (r >= 2)
                st = 50;
            if (util_Random(0x5a) + 10 <= st) {
                home = gm_BuildQuery(1, 1, a->attackerBldId);
                fv = (float)(st * 0.01);
                res = 0;
                cm_FightSettled = 1;
                cm_FightLootValue = cm_LootCarts(cm_FightRecordT, (AIBLD *)home, cm_FightAttackerC, cm_FightDefender, fv);
            } else {
                cm_FightLootValue = -1;
                res = 1;
            }
        }
    }
    if (win != -1)
        d2_CloseView(win);
    return res;
}

#define G20_US(p) (*(unsigned short *)(p))
#define G20_ME (ai_CharInts[ai_PlayerId][0])
#define G20_ATT ((char *)cm_FightDefender)
#define G20_DEF ((char *)cm_FightAttacker)
/* 0x4d4d70, the end of a fight (a: the fight record, its flags say how it was fought): the weapons the fighters lose
   (cm_FightWeaponLoss, by the fight's owner), the side picked (side: by cm_FightWinner, else the side with more
   fighters standing), the dialog "cutscenes\combat_end" with that side's text (0x1189 .. 0x118f), the portrait rows of
   both sides with their roles (0x1191, 0x1193, cm_FightPortraits), then by the kind of the fight: the loot of the
   carts (flags 1: cm_LootList, the goods moved to the attacker's building), the capture of the target building
   (flags 2, the player attacking: texts 0x119f .. 0x11a3 offer four outcomes: 1 the building is taken, 2 a share of
   money and goods is taken, 3 the attacker gains face, 0 the town guards are called, 0x11a4 .. 0x11a7), the bars of
   the defenders of a building of kind 0x13 (flags 4: their health, the money for the wounded) or the AI's capture
   (flags 2, the AI attacking: the same outcomes by random) */
void cm_FightEndDialog(CMFIGHTREC *a)
{
    char ba[0x800];
    char bb[0x400];
    char tc[0x400];
    char tmp[0x100];
    AIOBJ *obj, *ob, *q, *p;
    AICHAR *side, *c, *ch;
    AIBLD *b, *ca;
    AIBTYPE *bt;
    AIFIN *rec;
    int win, i, n, role, v, cnt, total, na, nb, id, k;
    int h, row, col, px, py;
    int wait = 0;

    obj = 0;
    role = 0;
    d2_CopyCaption(&ch_EmptyName);
    sv_SetStatusText(&ch_EmptyName);
    if (a->attackerBldId != -1) {
        b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
        obj = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x116);
        if (obj == 0)
            obj = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x2a);
    }
    if (cm_FightOwner == ai_Chars[ai_PlayerId].netId) {
        for (i = 0; i < 16; i++) {
            id = a->defenders[i];
            if (id != -1 && (c = (AICHAR *)gm_FindChar(id)) != 0)
                cm_FightWeaponLoss((char *)c);
        }
        for (i = 0; i < 16; i++) {
            id = a->attackers[i];
            if (id != -1 && (c = (AICHAR *)gm_FindChar(id)) != 0)
                cm_FightWeaponLoss((char *)c);
        }
    }
    if (cm_FightWinner == 0) {
        na = 0;
        for (i = 0; i < 16; i++) {
            id = a->defenders[i];
            if (id != -1) {
                c = (AICHAR *)cm_FindFighter(id);
                if (c->health != 0 && c->health != 4 && c->model->body->type != 1)
                    na++;
            }
        }
        nb = 0;
        for (i = 0; i < 16; i++) {
            id = a->attackers[i];
            if (id != -1) {
                c = (AICHAR *)cm_FindFighter(id);
                if (c->health != 0 && c->health != 4 && c->model->body->type != 1)
                    nb++;
            }
        }
    }
    side = cm_FightWinner == 0 ? (nb > na ? cm_FightAttackerC : cm_FightDefenderC)
                               : (cm_FightWinner == cm_FightAttacker ? cm_FightDefenderC : cm_FightAttackerC);
    win = d2_OpenForm(0, 0, "cutscenes\\combat_end");
    d2_PlaceMainWindow(win, 3);
    d2_SetForm(win, 2);
    io_PrintText(0x7d);
    d2_SetForm(win, 1);
    if (ai_Chars[ai_PlayerId].netId == side->netId) {
        if (side == cm_FightDefenderC) {
            if (a->flags & 1) {
                io_PrintText(0x1189, cm_FightDefenderC->id, cm_FightAttackerC->id);
                io_PrintText(0x118a);
            } else
                io_PrintText(0x1189, cm_FightDefenderC->id, cm_FightAttackerC->id);
        } else if (a->flags & 1)
            /* @original the same text both ways: VC6 merges the arms, the flags load stays ahead of the defender
               test */
            io_PrintText(0x118e, cm_FightAttackerC->id, cm_FightDefenderC->id);
        else
            io_PrintText(0x118e, cm_FightAttackerC->id, cm_FightDefenderC->id);
    } else if (side == cm_FightAttackerC) {
        io_PrintText(0x118c, cm_FightDefenderC->id, cm_FightAttackerC->id);
        if (a->flags & 1) {
            p = (AIOBJ *)gm_Query(0, 2, 7, 1, a->carts[0]);
            if (p->job24 != 0 && gm_FindBuildingById(p->job24->cartBld) != 0)
                io_PrintText(0x118d);
        }
    } else
        io_PrintText(0x118f, cm_FightAttackerC->id, cm_FightDefenderC->id);
    while (sv_RunFrame((int)cm_FightEndDialog, 0x7d6c6)) {
        if (dm_L.rup != 0 || d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1191);
    b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
    bt = &ai_BTypes[b->type];
    if (bt->kind == 0x13)
        role = 0;
    else if (bt->kind == 0x10)
        role = 1;
    else if (bt->kind == 4)
        role = 2;
    io_PrintText(0x1193, cm_FightAttackerC->id, role + 0x1125);
    cm_FightPortraits(a->attackers, 0, 100);
    ca = (AIBLD *)gm_BuildQuery(1, 1, a->placeId);
    if (ca != 0) {
        bt = &ai_BTypes[ca->type];
        if (bt->kind == 0x13)
            role = 0;
        else if (bt->kind == 0x10)
            role = 1;
        else if (bt->kind == 4)
            role = 2;
    } else
        role = 0;
    io_PrintText("$7A");
    io_PrintText(0x1193, cm_FightDefenderC->id, role + 0x1125);
    cm_FightPortraits(a->defenders, 0, 0x12c);
    while (sv_RunFrame((int)cm_FightEndDialog, 0x7d6c6)) {
        if (dm_L.rup != 0 || d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1197);
    if (a->flags & 1) {
        /* the carts: the loot list, the goods moved to the attacker's building */
        if (ai_PlayerId == cm_FightDefenderC->id)
            cm_LootList(0x24, 0x32, 2);
        else if (ai_PlayerId == cm_FightAttackerC->id) {
            if (side == cm_FightAttackerC)
                v = cm_LootList(0x24, 0x32, 0);
            else
                v = cm_LootList(0x24, 0x32, 1);
            rec = (AIFIN *)gm_PlayerRecord((char *)cm_FightAttacker);
            if (rec != 0)
                rec->lootTakings += v;
            if (a->attackerBldId != -1) {
                b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
                b->roundSums[0] += v;
            }
        }
        for (i = 0; i < 3; i++) {
            id = a->carts[i];
            if (id == -1)
                continue;
            p = (AIOBJ *)gm_Query(0, 2, 7, 1, id);
            p = (AIOBJ *)gm_Query(p->list, 1, 0, 0x1dd);
            for (q = (AIOBJ *)gm_Query(p->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
                if (side == cm_FightAttackerC)
                    q->fightSide = cm_FightAttackerC->id;
                if (q->fightSide == cm_FightAttackerC->id) {
                    if (obj != 0 && cm_FightOwner == ai_Chars[ai_PlayerId].netId) {
                        gm_ObjectBuildingOwner((unsigned short *)q);
                        cm_SendExSellObject(obj->objId, q->ownerId, q->id, q->count, ai_OwnPlayer, 0);
                    }
                    gm_GoodsPrice(q->id, ai_OwnPlayer);
                }
            }
        }
        wait = 1;
    } else if ((a->flags & 2) && side == cm_FightAttackerC && side->id == ai_PlayerId) {
        /* the player took the building: four outcomes to choose */
        ca = (AIBLD *)gm_BuildQuery(1, 1, a->placeId);
        txt_Format(ba, 0x119f, ca, cm_FightAttackerC->id, ca);
        if (ca->type != 0x1e && ca->type != 0x20 && ca->type != 0x1f) {
            txt_Format(bb, 0x11a0, ca->type * 14 + 0x48e);
            strcat(ba, bb);
        }
        txt_Format(bb, 0x11a1);
        strcat(ba, bb);
        txt_Format(bb, 0x11a2);
        strcat(ba, bb);
        txt_Format(bb, 0x11a3);
        strcat(ba, bb);
        n = cm_MessageBox(ba, 0x11);
        if (ca->type == 0x1e || ca->type == 0x20 || ca->type == 0x1f)
            n++;
        if (n == 1) {
            /* the building is taken */
            bt = &ai_BTypes[ca->type];
            if (bt->kind == 4 || bt->kind == 0x10)
                ai_CountMissionEvent(cm_FightAttacker, 0x1c);
            cm_SendOwnership(ca->id, cm_FightAttackerC->netId, cm_FightAttackerC->netId);
            txt_Format(ba, 0x11a4, cm_FightAttackerC->id, ca);
            cm_MessageBox(ba, 0);
        } else if (n == 2) {
            /* a share of the defender's money (5 .. 7 %, at most 0x9c400) and of the goods of the building's store
               (80 .. 99 %) goes to the attacker */
            tc[0] = ch_EmptyName[0];
            memset(tc + 1, 0, 0x3ff);
            txt_Format(tc, 0x11a5, cm_FightAttackerC->id, ca);
            cm_MessageBox(tc, 0);
            ob = (AIOBJ *)gm_Query(ca->objList, 2, 6, 0, 0x2a);
            if (ob == 0)
                ob = (AIOBJ *)gm_Query(ca->objList, 2, 6, 0, 0x116);
            v = gm_GetGoodsValue((char *)cm_FightDefender);
            v = v < 0x9c400 ? v : 0x9c400;
            v = (int)((float)((double)cm_Random(0x32) * 0.001 + 0.07) * (v < 0 ? 0 : v));
            txt_Format(tc, "$Z%T$A", v);
            cm_SendMoveGoods(cm_FightAttackerC->netId, cm_FightDefenderC->netId, ai_OwnPlayer, v);
            if (ob != 0 && obj != 0) {
                for (q = (AIOBJ *)gm_Query(ob->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
                    cnt = (int)(((double)(cm_Random(0x14) + 0x50) * 0.01) * q->count);
                    if (cnt < 1)
                        cnt = 1;
                    sprintf(tmp, "%i %s$A", cnt, (char *)txt_ObjectNames[q->id][0]);
                    strcat(tc, tmp);
                    if (ai_Chars[ai_PlayerId].netId == cm_FightOwner)
                        cm_SendExSellObject(obj->objId, ob->objId, q->id, cnt, ai_OwnPlayer, 0);
                    v = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * cnt + v);
                }
            }
            d2_SetForm(win, 1);
            io_PrintText(tc);
            rec = (AIFIN *)gm_PlayerRecord((char *)cm_FightAttacker);
            if (rec != 0)
                rec->lootTakings += v;
            if (a->attackerBldId != -1) {
                b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
                b->roundSums[0] += v;
            }
            wait = 1;
        } else if (n == 3) {
            tc[0] = ch_EmptyName[0];
            memset(tc + 1, 0, 0x3ff);
            txt_Format(tc, 0x11a6, cm_FightAttackerC->id, ca);
            cm_MessageBox(tc, 0);
            if (ai_Chars[ai_PlayerId].netId == cm_FightOwner)
                gm_ChangeBuildingState((unsigned char *)ca, 0x14 - cm_Random(0x14));
        } else if (n == 0) {
            /* the town guards are called: every guard (title groups 10 .. 12) turns against the attacker */
            txt_Format(ba, 0x11a7, cm_FightAttackerC->id, cm_FightAttackerC->id);
            cm_MessageBox(ba, 0);
            for (i = 0; i < 0x300; i++) {
                ch = &ai_Chars[i];
                if (ch->id != 0xffff && ch->health != 0 &&
                    ((k = gm_TitleGroup(ch->jobId)) == 10 || k == 11 || k == 12))
                    cm_SendRelations(cm_FightAttackerC->netId, ch->netId, 6, 1);
            }
        }
    } else if (a->flags & 4) {
        /* the defenders of a building of kind 0x13: their bars, the wounded paid for */
        b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
        if (ai_BTypes[b->type].kind == 0x13) {
            total = 0;
            for (v = 0; v < 16; v++) {
                id = a->defenders[v];
                if (id != -1) {
                    c = (AICHAR *)cm_FindFighter(id);
                    col = v % 4;
                    row = v / 4 + 1;
                    px = col * 80 + 0x30;
                    py = row * 100;
                    d2_WinOpenPic(px, py, 0x5db, d2_LastWindow);
                    d2_WinOpenPic(px + 2, py, c->picture, d2_LastWindow);
                    sv_ValueBar(px, py + 0x3c, c->sk83, 0x4eb);
                    h = 0;
                    if (c->health == 0) {
                        h = (int)(c->sk83 * 0.023809524f + 1.0f);
                        d2_WinOpenPic(px + 6, py + 6, 0x3b, d2_LastWindow);
                        h = gm_PlayerFactor(h * 40, ai_OwnPlayer);
                    }
                    if (side == cm_FightAttackerC) {
                        if (c->health > 0) {
                            int z = 0;

                            cm_BeginFieldBlock((char *)c, c->netId);
                            cm_AddFieldValue(4, 1, (char *)&c->bld - cm_FieldObject, &z);
                            cm_SendFieldDeltas();
                        }
                        txt_Format(tmp, "%T", h);
                        if (h != 0)
                            d2_CreateWinText(px, py + 0x50, tmp, d2_LastWindow);
                        total += h;
                    }
                }
            }
            if (total != 0)
                cm_SendMoveGoods(cm_FightAttackerC->netId, -1, ai_OwnPlayer, total);
            else if (ai_PlayerId == cm_FightAttackerC->id)
                io_PrintText(0x11b6);
            if (a->attackerBldId != -1) {
                b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
                b->roundSums[0] += total;
            }
            wait = 1;
        }
    } else if ((a->flags & 2) && side == cm_FightAttackerC && cm_FightAttackerC->kind != 6 && cm_FightAttackerC->kind != 7) {
        /* the AI took the building: the outcome by random (a guard's or a job's building of its own always) */
        k = 0;
        if (!gm_TitleJob(cm_FightAttackerC->id, gm_BuildingJob(bt->kind)) || bt->kind == 0xd || bt->kind == 0xc ||
            bt->kind == 0xb)
            k = util_Random(3) + 1;
        if (k == 0) {
            txt_Format(bb, 0x11a4, cm_FightAttackerC->id, ca);
            cm_MessageBox(bb, 0);
        } else if (k == 2) {
            tc[0] = ch_EmptyName[0];
            memset(tc + 1, 0, 0x3ff);
            txt_Format(tc, 0x11a5, cm_FightAttackerC->id, ca);
            cm_MessageBox(tc, 0);
            p = (AIOBJ *)gm_Query(ca->objList, 2, 6, 0, 0x2a);
            if (p == 0)
                p = (AIOBJ *)gm_Query(ca->objList, 2, 6, 0, 0x116);
            v = gm_GetGoodsValue((char *)cm_FightDefender);
            v = v < 0x9c400 ? v : 0x9c400;
            v = (int)((float)((double)cm_Random(0x32) * 0.001 + 0.07) * (v < 0 ? 0 : v));
            txt_Format(tc, "$Z%T$A", v);
            cm_SendMoveGoods(cm_FightAttackerC->netId, cm_FightDefenderC->netId, ai_OwnPlayer, v);
            if (p != 0 && obj != 0) {
                for (q = (AIOBJ *)gm_Query(p->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
                    n = (int)(((double)(cm_Random(0x14) + 0x50) * 0.01) * q->count);
                    if (n < 1)
                        n = 1;
                    sprintf(tmp, "%i %s$A", n, (char *)txt_ObjectNames[q->id][0]);
                    strcat(tc, tmp);
                    if (ai_Chars[ai_PlayerId].netId == cm_FightOwner)
                        cm_SendExSellObject(obj->objId, p->objId, q->id, n, ai_OwnPlayer, 0);
                    v = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * n + v);
                }
            }
            d2_SetForm(win, 1);
            io_PrintText(tc);
            rec = (AIFIN *)gm_PlayerRecord((char *)cm_FightAttacker);
            if (rec != 0)
                rec->lootTakings += v;
            if (a->attackerBldId != -1) {
                b = (AIBLD *)gm_BuildQuery(1, 1, a->attackerBldId);
                b->roundSums[0] += v;
            }
            wait = 1;
        } else if (k == 3) {
            tc[0] = ch_EmptyName[0];
            memset(tc + 1, 0, 0x3ff);
            txt_Format(tc, 0x11a6, cm_FightAttackerC->id, ca);
            cm_MessageBox(tc, 0);
            if (ai_Chars[ai_PlayerId].netId == cm_FightOwner)
                gm_ChangeBuildingState((unsigned char *)ca, 0x14 - cm_Random(0x14));
        } else if (k == 1) {
            txt_Format(bb, 0x11a7, cm_FightAttackerC->id, cm_FightAttackerC->id);
            cm_MessageBox(bb, 0);
            for (i = 0; i < 0x300; i++) {
                ch = &ai_Chars[i];
                if (ch->id != 0xffff && ch->health != 0 &&
                    ((k = gm_TitleGroup(ch->jobId)) == 10 || k == 11 || k == 12))
                    cm_SendRelations(cm_FightAttackerC->netId, ch->netId, 6, 1);
            }
        }
    }
    while (wait && sv_RunFrame((int)cm_FightEndDialog, 0x7d6c6)) {
        if (dm_L.rup != 0 || d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    di_ClearEvents(0x10);
    d2_CloseView(win);
}
/* 0x4d60b0, the check of the fight scene (cm_InitScenes registers it for kind 3) and its opening lines (a: the fight
   record): 0 when a leader is missing or both are the same, a cart is gone or its goods are worth nothing
   (gm_StallGoodsList's value) or the attacker's building is gone; else for both leaders one party note each by the
   kind of the fight (flags 1: texts 0x11e0 / 0x11e1, 2: 0x11e3 / 0x11e4 with the role of the attacker's building and
   the target building, 4: 0x11e6 / 0x11e7), the kind's closing text (0x11e2 / 0x11e5 / 0x11e8) appended; 1 */
int cm_CheckFight(CMFIGHTREC *a)
{
    char buf[0x400];
    AIRECIPE rc;
    int i, cart, role;
    AICHAR *ca, *cb;
    char *place;

    ca = (AICHAR *)gm_FindChar(a->attackerId);
    cb = (AICHAR *)gm_FindChar(a->defenderId);
    if (ca == 0 || cb == 0 || ca == cb) {
        /* @original the failing guard tests ca == cb once more and returns 0 either way (no code left of it):
           only with that second test, one more reference of ca and cb, VC6 serves cb before the cart loop's
           pointer (cb esi, the pointer and place edi); the plain guard gives the two swapped */
        if (ca == cb)
            return 0;
        return 0;
    }
    if (a->flags & 1) {
        for (i = 0; i < 3; i++) {
            if (a->carts[i] != -1) {
                cart = gm_FindObjectInst(a->carts[i]);
                if (cart == 0)
                    return 0;
                gm_StallGoodsList((char *)cart, (unsigned *)&rc);
                if (rc.pada8[2] <= 0)       /* the goods' value */
                    return 0;
            }
        }
    }
    place = gm_FindBuildingById(a->placeId);
    if (gm_BuildQuery(1, 1, a->attackerBldId) == 0)
        return 0;
    if (a->flags & 1) {
        txt_Format(buf, 0x11e0, cb->id);
        strcat(buf, txt_Ptrs[0x11e2]);
        cm_SendPartyNote(ca, (G28REC *)a, buf, ch_EmptyName);
        txt_Format(buf, 0x11e1, ca->id);
        strcat(buf, txt_Ptrs[0x11e2]);
        cm_SendPartyNote(cb, (G28REC *)a, buf, ch_EmptyName);
    } else if (a->flags & 2) {
        role = cm_FightRole((unsigned char *)gm_FindBuildingById(a->attackerBldId));
        txt_Format(buf, 0x11e3, role + 0x1125, cb->id, place);
        strcat(buf, txt_Ptrs[0x11e5]);
        cm_SendPartyNote(ca, (G28REC *)a, buf, ch_EmptyName);
        txt_Format(buf, 0x11e4, ca->id, role + 0x1125, place);
        strcat(buf, txt_Ptrs[0x11e5]);
        cm_SendPartyNote(cb, (G28REC *)a, buf, ch_EmptyName);
    } else if (a->flags & 4) {
        txt_Format(buf, 0x11e6, cb->id);
        strcat(buf, txt_Ptrs[0x11e8]);
        cm_SendPartyNote(ca, (G28REC *)a, buf, ch_EmptyName);
        txt_Format(buf, 0x11e7, ca->id);
        strcat(buf, txt_Ptrs[0x11e8]);
        cm_SendPartyNote(cb, (G28REC *)a, buf, ch_EmptyName);
    }
    return 1;
}
/* 0x4d63e0, a fight starts (a: the fight record): characters that are gone are dropped from both id lists, a cart
   without goods or no defender ends it (bad = 2); with the player present (0x6f0d8c == 0) the fight scene is built and
   fought (cm_SetupFightScene .. cm_FightIntro, the party run by cm_RunSceneParty), cm_FightStartDialog settles the
   loot and cm_FightEndDialog shows the end, else (remote) it is decided at once by strength (sum of cm_FightStrength +
   random 30 per side, every fighter loses up to 20 * random 10 health) and the loot goes through cm_LootStock /
   cm_LootCarts. */

void cm_RunFight(CMFIGHTREC *a)
{
    char buf[0x800];
    int rc[0x2e];
    int bad = 0, nb, seed, i, id, sa, sb, w, n, r;
    char *c, *p, *win, *e1, *e2;
    AICHAR *ch;
    AIBLD *bld;
    int *e;

    seed = cut_GetRandSeed();
    nb = 0;
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && gm_FindChar(id) == 0) {
            bad = 1;
            a->defenders[i] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        id = a->attackers[i];
        if (id != -1) {
            if (gm_FindChar(id) == 0) {
                bad = 1;
                a->attackers[i] = -1;
            } else
                nb++;
        }
    }
    if (a->flags & 1) {
        for (i = 0; i < 3; i++) {
            id = a->carts[i];
            if (id != -1) {
                p = (char *)gm_FindObjectInst(id);
                if (p != 0) {
                    gm_StallGoodsList(p, rc);
                    if (rc[0x2c] > 0)
                        continue;
                }
                bad = 2;
            }
        }
    }
    if (gm_BuildQuery(1, 1, a->attackerBldId) == 0 || bad == 2 || nb == 0)
        return;
    if (cm_SkipEvents == 0) {
        cm_SceneDuration = 200;
        cm_FightOwner = a->owner;
        cm_SetupFightScene(a);
        cm_CollectSceneChars();
        cm_PlaceFighters(a->attackers, "OFF");
        cm_PlaceFighters(a->defenders, "DEF");
        cm_FightIntro((char *)a);
        if (cm_FightFixedLight != 0)
            d3_SetLightSet(2, 0, 1.0f, 1);
        else
            sv_UpdateSkyPhase(&ai_Time);
        cm_FightWinner = 0;
        cm_FightSettled = 0;
        cm_FightLootValue = 0;
        cm_FightStartDialog(a);
        cut_RandSeed(seed);
        cm_CollectSceneChars();
        cm_CollectSceneChars();
        cm_RunSceneParty((char *)a, 0);
        if (cm_FightWinner == 0 && cm_FightSettled == 0)
            cm_RunSceneParty((char *)a, 0);
        if (cm_FightSettled == 0) {
            for (i = 0; i < 16; i++) {
                if (cm_DamagePops[i].h != -1)
                    d2_RemoveObj(cm_DamagePops[i].h);
                cm_DamagePops[i].h = -1;
                cm_DamagePops[i].v = 0;
                cm_DamagePops[i].c = 0;
            }
            cm_FightEndDialog(a);
            cm_FightOutro((char *)a);
        } else if (cm_FightLootValue != 0 && cm_FightAttackerC->kind == 6) {
            e1 = 0;
            bld = (AIBLD *)gm_BuildQuery(1, 1, cm_FightRecordT->attackerBldId);
            if (bld != 0)
                e1 = gm_Query(bld->objList, 2, 6, 0, 0x116);
            if (cm_FightLootValue > 0) {
                if (e1 == 0)
                    txt_Format(buf, 0x1144, cm_FightLootValue);
                else
                    txt_Format(buf, 0x1145, cm_FightLootValue);
            } else
                txt_Format(buf, 0x1146);
            cm_MessageBox(buf, 0x10);
        }
        cm_EndFightScene(a);
        return;
    }
    sa = 0;
    sb = 0;
    cm_FightAttacker = gm_FindChar(a->attackerId);
    cm_FightDefender = gm_FindChar(a->defenderId);
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0 && ch->health != 0 && ch->health != 4) {
            sa = (int)(cm_FightStrength((char *)ch) + sa);
            r = util_Random(10);
            gm_AddHealth(ch->id, -r * 20);
        }
    }
    for (i = 0; i < 16; i++) {
        id = a->attackers[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0 && ch->health != 0 && ch->health != 4) {
            sb = (int)(cm_FightStrength((char *)ch) + sb);
            r = util_Random(10);
            gm_AddHealth(ch->id, -r * 20);
        }
    }
    if (util_Random(30) + sb > util_Random(30) + sa)
        win = G20_DEF;
    else
        win = G20_ATT;
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0)
            cm_FightWeaponLoss((char *)ch);
    }
    for (i = 0; i < 16; i++) {
        if (a->attackers[i] != -1 && (ch = (AICHAR *)gm_FindChar(a->defenders[i])) != 0)
            cm_FightWeaponLoss((char *)ch);
    }
    if (a->flags & 2) {
        if (win == G20_DEF) {
            e1 = gm_BuildQuery(1, 1, a->attackerBldId);
            e2 = gm_BuildQuery(1, 1, a->placeId);
            cm_LootStock(e2, e1, G20_DEF, G20_ATT);
        }
    } else if (!(a->flags & 4) && (a->flags & 1) && win == G20_DEF) {
        c = gm_BuildQuery(1, 1, a->attackerBldId);
        cm_LootCarts((char *)a, c, G20_DEF, cm_FightDefender, 100.0f);
    }
}
/* 0x4d6930, the result of the fight in the record r (+0x354 / +0x358): 0x6f0cf8 set -> 0 and the ransom 0x6f0cfc, else 1 */

void cm_FightResult(char *r)
{
    if (cm_FightSettled != 0) {
        ((CMCUTINFO *)r)->result = 0;
        ((CMCUTINFO *)r)->lootValue = cm_FightLootValue;
    } else
        ((CMCUTINFO *)r)->result = 1;
}
/* 0x4d6970, one frame of the fight UI (r: the fight record): the countdown bar cm_FightTimeBar sized to the fight area,
   the redraws 0x525ff0 .. 0x4ce830, and the click on the button cm_FightButton (while 0x6f0d88 / 0x6f0cf4 are clear) sends
   message 0x50 with the cut entry of the player's character (0x4cdf00) in state 5, else the dialog is closed (0x6f22a0) */

void cm_FightFrame(char *r)
{
    CMCUTENTRY sl;
    CMCUTENTRY *q;

    d2_SetObjValue(cm_FightTimeBar, 0, cm_SceneDuration, cm_SceneDeadline - cm_SceneTicks + 1);
    sv_BorderCameraView();
    cm_UpdatePortraits();
    cm_UpdatePointer();
    cm_ClickSceneChar();
    cm_FightMouse(r);
    cm_UpdateSceneChars();
    cm_UpdateCharInfoWins(r);
    cm_UpdateBombs();
    cm_UpdateGrenades();
    if (cm_SceneTimedOut == 0 && cm_FightWinner == 0) {
        if (d2_ClickedId != -1 && d2_PointerObj == cm_FightButton) {
            q = (CMCUTENTRY *)cm_CutEntrySlot(r, cm_CutChar);
            memcpy(&sl, q, sizeof(CMCUTENTRY));
            sl.charId = ((AICHAR *)cm_CutChar)->netId;
            sl.state = 5;
            cm_SendCutEntry(((CMCUTINFO *)r)->playerId, (int)&sl);
        }
    } else
        sv_PopRequest = 1;
}
/* 0x4d6a50, builds the fight UI: the form "cutscenes\combat" (cm_FightForm), the button cm_FightButton (text
   cm_FightButtonText) and the bar cm_FightTimeBar at the bottom, then the portrait boxes (0x4cfe70 / 0x4cfc10) */

void cm_BuildFightUI(char *a)
{
    int v, t;

    v = d2_OpenForm(0, 0, "cutscenes\\combat");
    cm_FightForm = v;
    d2_PlaceMainWindow(v, 3);
    d2_SetForm(v, 0);
    t = d2_WinCreateSlider(0x10, 0xc, 0xbf, cm_FightButtonText, d2_LastWindow);
    cm_FightButton = t;
    d2_Objs[t].centred = 1;
    d2_CenterObj(cm_FightButton);
    t = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x190) / 2, 0x37, 0x190, 0, cm_SceneDuration, 0x83, 0x42, d2_LastWindow);
    cm_FightTimeBar = t;
    d2_Objs[t].font = 0x54;
    d2_Objs[d2_Wins[d2_LastWindow].parent].interactive = 1;
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    cm_ClosePortraits();
    cm_BuildPortraits();
}
/* 0x4d6b70, closes the fight UI: the form cm_FightForm, the marks (0x4d08c0), the portrait boxes */

void cm_CloseFightUI()
{
    d2_CloseView(cm_FightForm);
    cm_FightForm = 0;
    cm_ClearMarks();
    cm_ClosePortraits();
}
/* 0x4d6ba0, the action of one fighter (f): the target's health is set; a blow (not with the pot 0x174 or the bomb
   0x176) by a fighter that is idle or walking / standing (action 0x2d / 0x3a) shouts the attack (1 in 10), takes the
   damage off the target when it lands and queues the strike: "kampf/schlagen1" without a weapon, else cm_HitAction
   with the weapon's animation (speed 1.4; a landing blow of the weapons 0x16e / 0x172 ends the action), then a pause.
   The pot queues cm_LayBombAction (the thrower's own fight places it at the free cell near him, cm_SendObjCell), the
   bomb cm_ThrowBombAction with the throw sound; both end the action. */

void cm_FighterAct(CMFIGHTSLOT *f)
{
    struct D3HMAP *h = d3_DefaultScene->hmap;
    AICHAR *c, *tgt, *side;
    AIWEAPON *k;
    CHACTION *x, *ev;
    char *s, *d;
    int dmg, lands, ox, oy;
    float py;

    if (f->active == 0 || f->id == -1)
        return;
    c = (AICHAR *)cm_FindFighter(f->id);
    k = (AIWEAPON *)cm_CharWeapon((char *)c);
    side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
    tgt = (AICHAR *)cm_FindFighter(f->target);
    if (tgt != 0)
        tgt->v024 = f->health;
    if (!(k != 0 && (k->id == 0x174 || k->id == 0x176))) {
        if (f->hit != 0 && ((x = c->scene->actions) == 0 || x->type == 0x2d || x->type == 0x3a)) {
            if (util_Random(10) > 8) {
                if (side->netId == cm_FightAttackerC->netId)
                    snd_QueueVoice(c->id, cm_AttackerAttackVoices, "_K_C2C_ANGRIFF", -1, 500);
                else
                    snd_QueueVoice(c->id, cm_DefenderAttackVoices, "_K_C2C_ANGRIFF", -1, 500);
            }
            lands = f->kill;
            dmg = f->damage;
            if (lands != 0)
                tgt->v024 -= dmg;
            if (k == 0)
                ch_QueueAnim(c->scene, "kampf/schlagen1", 1);
            else {
                if (k->id == 0) {
                    if (lands != 0)
                        ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_HitAction, 8, 0x2e, 1);
                    else
                        ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_HitAction, 2, 0x2e, 1);
                } else if (lands != 0)
                    ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_HitAction, 6, 0x2e, 1);
                else
                    ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_HitAction, 2, 0x2e, 1);
                ev->hit.speed = 1.4f;
                ev->hit.victim = tgt;
                ev->hit.hitter = c;
                ev->hit.damage = dmg;
                ev->hit.sideId = side->netId;
                for (s = k->anim, d = ev->u.name; (*d++ = *s++) != 0;)
                    ;
                if ((k->id == 0x16e || k->id == 0x172) && lands != 0) {
                    f->id = -1;
                    f->active = 0;
                }
            }
            ch_AddAction(c->scene, 0x3b, cm_Random(0x1e) + 0x8c);
        }
    }
    if (k->id == 0x174 && f->kill != 0) {
        ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_LayBombAction, 6, 0x2e, 1);
        ev->pot.victim = tgt;
        ev->pot.thrower = c;
        ev->pot.x = f->x;
        ev->pot.y = f->y;
        strcpy(ev->u.name, "spezial/einhaendig_vom_boden_nehmen");
        if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner) {
            if (!d3_GetHeightAt((struct D3SM *)h, &c->scene->obj->pos, &cm_PickCellX, &cm_PickCellZ, &py))
                return;
            cm_BestCellNear((char *)c, cm_PickCellX, cm_PickCellZ, 8, &ox, &oy);
            cm_SendObjCell(c->scene->obj->name, ox, oy, 0);
            f->id = -1;
            f->active = 0;
        }
    }
    if (k->id == 0x176 && f->hit != 0) {
        ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_ThrowBombAction, 5, 0x2e, 1);
        ev->bomb.thrower = c;
        ev->bomb.x = f->x;
        ev->bomb.y = f->y;
        ev->bomb.sideId = side->netId;
        for (s = k->anim, d = ev->u.name; (*d++ = *s++) != 0;)
            ;
        ch_AddAction(c->scene, 0x3b, cm_Random(0x1e) + 0x8c);
        sb_PlaySample(cm_BombSounds, "topfwerfen_s", 1, 0x3f, 0x7f, 0, 0);
        f->id = -1;
        f->active = 0;
    }
}
/* 0x4d6f60, stops the actions of the fighter in the fight slot f (id, active): 0x405610 on its scene character */

void cm_FightSlotStop(CMFIGHTSLOT *f)
{
    AICHAR *c;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        if (c != 0)
            ch_StopActions(c->scene);
    }
}
/* 0x4d6f90, the cheer of the winner in the fight slot f: event 0x4d3ac0 with the animation "gestik/jubeln" */

void cm_FightSlotCheer(CMFIGHTSLOT *f)
{
    AICHAR *c;
    CHACTION *ev;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_CheerAction, 5, 0x2e, 1);
        ev->cheer.fighter = c;
        strcpy(ev->u.name, "gestik/jubeln");
    }
}
/* 0x4d7000, the fighter slot f picks up the object lying at its place (dummy): the object (0x449690) is marked as
   owned by the character's side (AIOBJ.fightSide), event 0x4d3ae0 */

void cm_FightSlotPickUp(CMFIGHTSLOT *f)
{
    AICHAR *c, *side;
    D3OBJECT *o;
    CHACTION *ev;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        side = (AICHAR *)c->bld;
        ch_StopActions(c->scene);
        o = d3_FindObject(0, 0x40, 0, f->dummy);
        if (o != 0) {
            ((AIOBJ *)o->owner)->fightSide = side->id;
            ev = ch_AddActionCB(c->scene, (CHACTFUNC)cm_PickUpAction, 5, 0x2e, 1);
            ev->pickUp.fighter = c;
            ev->pickUp.object = o;
            strcpy(ev->u.name, "spezial/einhaendig_vom_boden_nehmen");
        }
    }
}
/* 0x4d70a0, the fighter slot f puts down the object it carries (AICHAR.v1ac) at its place */

void cm_FightSlotPutDown(CMFIGHTSLOT *f)
{
    AICHAR *c;
    D3OBJECT *o, *body;
    CHACTION *a;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        ch_StopActions(c->scene);
        d3_FindObject(0, 0x40, 0, f->dummy);
        o = (D3OBJECT *)c->v1ac;
        if (o != 0) {
            body = c->scene->obj;
            d3_SetWorldPosRot(o, (float *)&body->pos, &body->matrix);
            d3if_Show(o, 1);
            c->v1ac = 0;
        }
        a = ch_AddAction(c->scene, 0x2e, 1);
        strcpy(a->u.name, "spezial/einhaendig_vom_boden_nehmen");
    }
}
/* 0x4d7150, the character in the fight slot f is the winner so far (cm_FightWinner): text 0x11b7 */

void cm_FightSlotWinner(CMFIGHTSLOT *f)
{
    char buf[0x100];
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(f->id);
    cm_FightWinner = (int)c;
    txt_Format(buf, 0x11b7, c->id);
    sv_SetStatusText(buf);
}
/* 0x4d7190, the escape flag of a fighter action f: the escape dummy named by f gets the fighter as its owner (the
   fighter marked fled, text 0x11b8); when the owner changes the old flag is deleted and a new "ub_FLUCHTFLAGGE" object
   (animated, the texture set 'g' + ref54 of the side leader of the kinds 5 .. 7, named "CONQUER_FLAG") is put at the
   dummy's place (the name of the fled bit is a reasoned guess from the flag). */

void cm_PlantEscapeFlag(CMFIGHTSLOT *f)
{
    AICHAR *c = 0;
    D3OBJECT *o, *r;
    AICHAR *side;
    char buf[0x100];
    D3VECTOR4 pos;
    D3VECTOR rot;
    char k;

    if (f->active != 0) {
        o = d3_FindObject(0, 0x40, 0, f->dummy);
        if (f->id != -1) {
            c = (AICHAR *)cm_FindFighter(f->id);
            c->scene->flags.fled = 1;
            txt_Format(buf, 0x11b8, c->id);
            sv_SetStatusText(buf);
        }
        if ((AICHAR *)o->owner != c) {
            if (o->flagObj != 0) {
                d3if_Delete(o->flagObj);
                o->flagObj = 0;
            }
            if (c != 0) {
                rot.x = 0;
                rot.y = 0;
                rot.z = 0;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
                r = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, "ub_FLUCHTFLAGGE");
                cm_FlagObjects[cm_FlagObjectCount] = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, &ch_EmptyName);
                d3t_PlayAnim(r, "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
                side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
                if (side != 0 && side->ref54 != 0 && (side->kind == 5 || side->kind == 6 || side->kind == 7))
                    k = side->ref54 + 'g';
                else
                    k = 0;
                if (k != 0)
                    d3if_SelectTextureSet(r, (struct D3SURF *)r->info, k, 1);
                sprintf(r->name, "CONQUER_FLAG");
                r->owner = 0;
                r->kind = 4;
                r->mask = 1;
                r->stateFlags = (r->stateFlags & 0xf7) | 0x44;
                o->flagObj = r;
            } else
                o->flagObj = 0;
        }
        o->owner = c;
    }
}
/* 0x4d7360, the attack of the fighter c on the fighter g (rec: c's cutscene record): -1 when c cannot attack (a used-up
   weapon of the kinds 1 / 2, c's actions busy). In range (or with a weapon that is not one of the melee kinds) the hit
   is rolled (random 255 against the weapon's accuracy percent of c's skill sk83) with the damage (dmgMin .. dmgMax
   percent of g's life value, * 1.25 when c's side leader has the item 0x2b9); in the player's own fight a melee attack
   also orders the other side's AI to strike back (cm_OrderAttack, unless an order is pending); the fight command 2 goes
   out (cm_SendFightCommand). Out of range c walks to the free cell near g (cm_SendObjCell), unless c holds its place.
   Returns the send slot, -1 when nothing was sent. */

int cm_Attack(CMCUTINFO *rec, AICHAR *c, AICHAR *g)
{
    struct D3HMAP *h = d3_DefaultScene->hmap;
    CMFIGHTSLOT s;
    AIWEAPON *k;
    AIOBJ *item;
    AICHAR *side;
    CMCUTINFO *other;
    CMCUTENTRY *e;
    int i, n, hit, state;
    int r;

    memset(&s, 0, sizeof(s));
    s.id = c->netId;
    s.active = 2;
    s.target = g->netId;
    if (c->formation == 1 || c->formation == 2 || c->formation == 3)
        s.held = 1;
    k = (AIWEAPON *)cm_CharWeapon((char *)c);
    if (k->kind == 1 || k->kind == 2) {
        item = (AIOBJ *)cm_CharWeaponItem((char *)c);
        if (item != 0 && item->usesLeft <= 0)
            return -1;
    }
    if (!(k->id == 0x154 || k->id == 0x156 || k->id == 0x158 || k->id == 0x16e || k->id == 0x172 || k->id == 0 ||
          k->id == 0x176) ||
        !(cm_CharObjDistXZ((char *)c, (char *)g->scene->obj) > k->range)) {
        if (c->scene->actions != 0)
            return -1;
        r = util_Random(0xff);
        side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
        if (r > 0xff - (int)((k->accuracy * 0.01) * c->sk83))
            hit = 1;
        else
            hit = 0;
        if (hit != 0)
            s.damage = (int)((double)(unsigned int)(cm_Random(k->dmgMax - k->dmgMin) + k->dmgMin) * g->f01c * 0.01);
        else
            s.damage = 0;
        if (side != 0 && ai_InventoryHas(side, 0x2b9) != 0)
            s.damage = (int)(s.damage * 1.25f);
        s.hit = 1;
        s.kill = hit;
        if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && k->kind == 0) {
            other = 0;
            n = cm_FightRecordT->nChars;
            for (i = 0; i < n; i++) {
                if (cm_FightRecordT->charIds[i] != -1 && rec != &cm_CutInfos[i]) {
                    other = &cm_CutInfos[i];
                    break;
                }
            }
            e = (CMCUTENTRY *)cm_CutEntrySlot((char *)other, (char *)g);
            if (e->orderSlot == -1 || cm_GetSlotState(e->orderSlot) != 0) {
                state = e->state;
                e->orderSlot = -1;
                if (state == 0) {
                    item = (AIOBJ *)cm_CharWeaponItem((char *)g);
                    if (item != 0 && item->usesLeft > 0)
                        e->orderSlot = cm_OrderAttack((char *)other, (char *)g, (char *)c, 0, 0);
                }
            }
        }
        cm_SendSetField(c->netId, 0x1c8, 4, 0x20000000, 0);
        return cm_SendFightCommand(c, 2, &s);
    }
    {
        int ix, iz;
        float py;

        if (c->scene->actions != 0 || s.held != 0)
            return -1;
        if (!d3_GetHeightAt((struct D3SM *)h, &g->scene->obj->pos, &ix, &iz, &py))
            return -1;
        if (cm_FindNearCell(ix, iz, &ix, &iz) == 0)
            return -1;
        return cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
    }
}
