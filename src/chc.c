/* chc.c -- original file name unknown: character script commands ("CreateCharacter(): Could not load
   character"). Extent in GildeGold.exe (inferred): starts at 0x40ba10 right after the script builtins
   (evtc.c) and ends with the command registration at 0x40d270; the object commands
   follow at 0x40d680. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/chc.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these two with other types (void / int flag, D3ANIMFLAGS); ch.h has ours */
#define d3_Unk44e4a0 d3_Unk44e4a0_void
#include "d3.h"
#include "d3_anim.h"
#undef d3_Unk44e4a0
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#include "chc.h"
#include "d3_engine.h"
#include "d3_interface.h"
#include "d3_snd.h"
#include "d3_vec.h"
#include "err.h"
#include "f3init.h"
#include "mem.h"
#include "sblib.h"
#include "sv.h"
#include "unk_458810.h"

/* 0x40ba10, CreateCharacter(name, x, y, z): creates the character at that position and gives it the
   group of the running script (it is shown only while that group is the current room); returns it, or 0
   after an error when the character could not be loaded. The script arguments arrive as pointers. */
int chc_CreateCharacter(char **name, float *x, float *y, float *z)
{
    D3VECTOR4 pos;
    CHARACTER *ch;

    pos.x = *x;
    pos.y = *y;
    pos.z = *z;
    ch = ch_Create(*name, (D3VECTOR *)&pos);
    if (!ch)
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacter(): Could not load character");
    else
        ch->scriptGroup = evt_Cur->group;
    return (int)ch;
}
/* 0x40ba90, CreateCharacterAtDummy(mesh, dummy, name): creates the character at the dummy's world
   position, turns it into the dummy's direction, gives it `name` (body and object) and the group of the
   running script; returns 0 after an error when the dummy is missing or the mesh cannot be loaded. */

int chc_CreateCharacterAtDummy(char **mesh, D3OBJECT **dummy, char **name)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;
    CHARACTER *ch;

    if (!*dummy) {
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacterAtDummy(): invalid dummy");
        return 0;
    }
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    ch = ch_Create(*mesh, (D3VECTOR *)&pos);
    if (ch) {
        strcpy(ch->name, *name);
        strcpy(ch->obj->name, *name);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(*dummy, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot(ch->obj, &rot);
        d3_SetObjectPos(ch->obj, (D3VECTOR *)&pos);
        ch->scriptGroup = evt_Cur->group;
    } else
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacterAtDummy(): Could not load character");
    return (int)ch;
}
/* 0x40bbb0, KillCharacter(ch): deletes the character and returns ch_Delete's result. */

int chc_KillCharacter(CHARACTER **ch)
{
    return ch_Delete(*ch);
}
/* 0x40bbc0, waits until the character has walked there */

int chc_WalkToDummy(CHARACTER **ch, D3OBJECT **dummy)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummy) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummy;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (!mem_IsValid(*dummy) || !mem_IsValid(*ch))
            return 0;
        ch_WalkToObject(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummy;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummy(): invalid character or dummy");
    return 0;
}
/* 0x40bc80, WalkToDummyRotate(ch, dummy): walks the character to the dummy and turns it into the
   dummy's direction; 1 when the walk was queued, 0 while the command runs or the arguments are invalid */

int chc_WalkToDummyRotate(CHARACTER **ch, D3OBJECT **dummy)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyRotate) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyRotate;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (!mem_IsValid(*dummy) || !mem_IsValid(*ch))
            return 0;
        ch_WalkToObjectTurn(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyRotate;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotate(): invalid character or dummy");
    return 0;
}
/* 0x40bd40, WalkToDummyRotateVerified(ch, dummy, dist): WalkToDummyRotate, but first fails with an
   error when the character chc_DummyBlocked finds within dist of the dummy is in the way */

int chc_WalkToDummyRotateVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyRotateVerified) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyRotateVerified;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (chc_DummyBlocked(dummy, dist)) {
            evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotateVerified(): dummy blocked");
            return 0;
        }
        ch_WalkToObjectTurn(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyRotateVerified;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotateVerified(): invalid character or dummy");
    return 0;
}
/* 0x40be00, WalkToDummyVerified(ch, dummy, dist): walks the character to the dummy, but first fails
   with an error when another character blocks the dummy within dist */

int chc_WalkToDummyVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyVerified) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyVerified;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (chc_DummyBlocked(dummy, dist)) {
            evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyVerified(): dummy blocked");
            return 0;
        }
        ch_WalkToObject(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyVerified;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyVerified(): invalid character or dummy");
    return 0;
}
/* 0x40bec0, PlayAnimation(ch, name, n): queues the animation name n times (ch_QueueAnim); 1 after an
   error when the character is invalid, 0 otherwise (the script waits while its actions run) */

int chc_PlayAnimation(CHARACTER **ch, char **name, int *n)
{
    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimation) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimation;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAni(): invalid character or dummy");
        return 1;
    }
    ch_QueueAnim(*ch, *name, *n);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimation;
    return 0;
}
/* 0x40bf60, action callback: plays the sound named in the action */

void chc_AniSoundCB(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    void *snd = sb_GetSample(0, CH_ACTOBJ(a)->name2);

    if (snd) {
        if (d3_Terrain)
            d3snd_PlayAt(snd, ch->obj, 0x7f, 3000.0f);
        else
            d3snd_PlayAt(snd, ch->obj, 0x7f, 1000.0f);
    }
}
/* 0x40bfc0, PlayAnimationSound(ch, name, n, frame, sound): queues the animation name n times and plays
   sound at that frame through the chc_AniSoundCB callback; 1 after an error when the character is
   invalid, 0 otherwise */

int chc_PlayAnimationSound(CHARACTER **ch, char **name, int *n, int *frame, char **sound)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationSound) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationSound;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniSound(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniSoundCB, *frame, 0x2e, *n);
    if (a) {
        strcpy(a->u.name, *name);
        strcpy(CH_ACTOBJ(a)->name2, *sound);
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationSound;
    return 0;
}
/* 0x40c090, action callback: starts the script named in the action */

void chc_AniScriptCB(CHACTION *a)
{
    EVTSCRIPT *sc = evt_Load(a->scriptName);

    if (sc) {
        evt_RunScript(sc);
        sc->group = a->scriptAction.group;
    }
}
/* 0x40c0d0, PlayAnimationScript(ch, name, n, frame, script): queues the animation name n times and lets
   the action run script at that frame (chc_AniScriptCB, group -1 = none); 1 after an error, else 0 */

int chc_PlayAnimationScript(CHARACTER **ch, char **name, int *n, int *frame, char **script)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationScript) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationScript;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScript(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptCB, *frame, 0x2e, *n);
    if (a) {
        if (strlen(*name) >= 0x5f) {
            evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScript(): script name too long");
            return 1;
        }
        strncpy(a->u.name, *name, 0x3f);
        strncpy(a->scriptName, *script, 0x5f);
        a->scriptAction.group = evt_NoGroup;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationScript;
    return 0;
}
/* 0x40c1d0, the same with an int argument for the script */

void chc_AniScriptIntCB(CHACTION *a)
{
    EVTSCRIPT *sc = evt_Load(a->scriptName);

    if (sc) {
        evt_RunScriptArgs(sc, 1, a->scriptAction.arg);
        sc->group = a->scriptAction.group;
    }
}
/* 0x40c210, PlayAnimationScriptInt(ch, name, n, frame, script, arg): PlayAnimationScript with one int
   argument passed to the script (evt_RunScriptArgs) */

int chc_PlayAnimationScriptInt(CHARACTER **ch, char **name, int *n, int *frame, char **script, int *arg)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationScriptInt) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationScriptInt;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScriptInt(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptIntCB, *frame, 0x2e, *n);
    if (a) {
        if (strlen(*name) >= 0x5f) {
            err_Message("PlayCharacterAniScript(): script name too long");
            return 1;
        }
        strncpy(a->u.name, *name, 0x3f);
        strncpy(a->scriptName, *script, 0x5f);
        a->scriptAction.group = evt_NoGroup;
        a->scriptAction.arg = *arg;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationScriptInt;
    return 0;
}
/* 0x40c330, GetCharacterHandle(name): returns the character with that name, or 0 after an error (the
   script then works with an empty handle) */

int chc_GetCharacterHandle(char **name)
{
    char buf[0x80];
    CHARACTER *ch = ch_FindByName(*name);

    if (!ch) {
        sprintf(buf, "GetCharacterHandle(): character '%s' not found", *name);
        evt_Error(evt_Cur, evt_Cur->pc, buf);
    }
    return (int)ch;
}
/* 0x40c390, TakeObject(ch, name, frame, obj): makes the character take the object with the named
   animation at frame (ch_TakeObject); 1 after an error, else 0 */

int chc_TakeObject(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj)
{
    if (evt_CurCall && evt_CurCall->fn == chc_TakeObject) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObject;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObject(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObject(): Invalid dummy");
        return 1;
    }
    ch_TakeObject(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObject;
    return 0;
}
/* 0x40c440, TakeObjectLeft(ch, name, frame, obj): the same with the left hand (ch_TakeObject1) */

int chc_TakeObjectLeft(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj)
{
    if (evt_CurCall && evt_CurCall->fn == chc_TakeObjectLeft) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObjectLeft;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectLeft(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectLeft(): Invalid dummy");
        return 1;
    }
    ch_TakeObject1(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObjectLeft;
    return 0;
}
/* 0x40c4f0, TakeObjectScript(ch, name, n, obj, frame, script): takes the object with the named
   animation at frame, stores the object's model name in the action (name2) and runs script through
   chc_AniScriptCB; 1 after an error, else 0
   @deep Q4 D3MATERIAL_NAME104 (0x104 of D3MATERIAL read as the model-name char*): the typed union name/ops in d3.h flips d3_MirDrawObject by 8 bytes at +0x1be (build REGRESSED, the known type debt of notes/vc6-matching.md); no other typed access possible without that header change */

int chc_TakeObjectScript(CHARACTER **ch, char **name, int *n, D3OBJECT **obj, int *frame, char **script)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_TakeObjectScript) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObjectScript;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectScript(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectScript(): Invalid dummy");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptCB, *frame, 0x31, *n);
    if (a) {
        strcpy(a->u.name, *name);
        if (*obj && (*obj)->material && D3MATERIAL_NAME104((*obj)->material))
            strcpy(CH_ACTOBJ(a)->name2, D3MATERIAL_NAME104((*obj)->material));
        strcpy(a->scriptName, *script);
        a->scriptAction.group = evt_NoGroup;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObjectScript;
    return 0;
}
/* 0x40c630, GiveObject(ch, name): puts the named object into the character's right hand (ch_SetHandObject); 0 when the character is invalid, else 1 */

int chc_GiveObject(CHARACTER **ch, char **name)
{
    if (!*ch)
        return 0;
    ch_SetHandObject(*ch, 2, *name);
    return 1;
}
/* 0x40c660, DropObject(ch, name, frame, obj): drops the object with the named animation at frame (ch_DropObject); 1 after an error (also when the object is not in the scene), else 0 */

int chc_DropObject(CHARACTER **ch, char **name, int *frame, char **obj)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DropObject(): Invalid character");
        return 1;
    }
    if (*obj && !d3_FindObject(0, 0x40, (int)*obj, 0))
        return 1;
    if (evt_CurCall && evt_CurCall->fn == chc_DropObject) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_DropObject;
            return 0;
        }
        return 0;
    }
    ch_DropObject(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_DropObject;
    return 0;
}
/* 0x40c720, DropObjectLeft(ch, name, frame, obj): the same with the left hand (ch_DropObject1) */

int chc_DropObjectLeft(CHARACTER **ch, char **name, int *frame, char **obj)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DropObjectLeft(): Invalid character");
        return 1;
    }
    if (*obj && !d3_FindObject(0, 0x40, (int)*obj, 0))
        return 1;
    if (evt_CurCall && evt_CurCall->fn == chc_DropObjectLeft) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_DropObjectLeft;
            return 0;
        }
        return 0;
    }
    ch_DropObject1(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_DropObjectLeft;
    return 0;
}
/* 0x40c7e0, SitDown(ch, name): sits the character down with the named animation (ch_SitDown); 1 after an error, else 0 */

int chc_SitDown(CHARACTER **ch, char **name)
{
    if (evt_CurCall && evt_CurCall->fn == chc_SitDown) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_SitDown;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SitDown(): Invalid character");
        return 1;
    }
    ch_SitDown(*ch, *name);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_SitDown;
    return 0;
}
/* 0x40c870, SitDownAtOnce(ch, name): SitDown, but marks the action (flags & 1) so the animation starts at once */

int chc_SitDownAtOnce(CHARACTER **ch, char **name)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_SitDownAtOnce) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_SitDownAtOnce;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SitDown(): Invalid character");
        return 1;
    }
    a = ch_SitDown(*ch, *name);
    if (a)
        a->scriptAction.flags |= 1;
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_SitDownAtOnce;
    return 0;
}
/* 0x40c910, GetUp(ch, name): stands the character up with the named animation (ch_StandUp); 1 after an error, else 0 */

int chc_GetUp(CHARACTER **ch, char **name)
{
    if (evt_CurCall && evt_CurCall->fn == chc_GetUp) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_GetUp;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "GetUp(): Invalid character");
        return 1;
    }
    ch_StandUp(*ch, *name);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_GetUp;
    return 0;
}
/* 0x40c9a0, SetCharacterCamera(ch, view): cuts to the camera preset named CLOSEUP, LEFT_SHOULDER, RIGHT_SHOULDER or EGO (0 .. 3); 1 after an error, else 0 */

int chc_SetCharacterCamera(CHARACTER **ch, char **view)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetCharacterCamera(): Invalid character");
        return 1;
    }
    if (!strcmp("CLOSEUP", *view))
        ch_SetCamera(*ch, 0);
    else if (!strcmp("LEFT_SHOULDER", *view))
        ch_SetCamera(*ch, 1);
    else if (!strcmp("RIGHT_SHOULDER", *view))
        ch_SetCamera(*ch, 2);
    else if (!strcmp("EGO", *view))
        ch_SetCamera(*ch, 3);
    return 0;
}
/* 0x40ca60, MoveCharacterCamera(ch, view, time): moves over time to the camera preset named CLOSEUP, LEFT_SHOULDER, RIGHT_SHOULDER or EGO (0 .. 3); 1 after an error, else 0 */

int chc_MoveCharacterCamera(CHARACTER **ch, char **view, int *time)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "MoveCharacterCamera(): Invalid character");
        return 1;
    }
    if (!strcmp("CLOSEUP", *view))
        ch_MoveCamera(*ch, 0, *time);
    else if (!strcmp("LEFT_SHOULDER", *view))
        ch_MoveCamera(*ch, 1, *time);
    else if (!strcmp("RIGHT_SHOULDER", *view))
        ch_MoveCamera(*ch, 2, *time);
    else if (!strcmp("EGO", *view))
        ch_MoveCamera(*ch, 3, *time);
    return 0;
}
/* 0x40cb40, PreloadAnimation(ch, a, b, c, d): preloads four animations for the character (ch_PreloadAnims); 1 after an error, else 0 */

int chc_PreloadAnimation(CHARACTER **ch, char **a, char **b, char **c, char **d)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PreloadAnimation(): Invalid character");
        return 1;
    }
    ch_PreloadAnims(*ch, 4, *a, *b, *c, *d);
    return 0;
}
/* 0x40cba0, CharacterIdle(ch): 1 when the character has no action left, else 0 (also 0 after an error) */

int chc_CharacterIdle(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterIdle(): Invalid character");
        return 0;
    }
    return (*ch)->actions == 0;
}
/* 0x40cbe0, StopCharacter(ch): removes all actions of the character (ch_StopActions); 0, also after an error */

int chc_StopCharacter(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "StopCharacter(): Invalid character");
        return 0;
    }
    ch_StopActions(*ch);
    return 0;
}
/* 0x40cc20, KillCharacterAnimations(ch): stops the current action and removes all others (ch_RemoveAction); 0, also after an error */

int chc_KillCharacterAnimations(CHARACTER **ch)
{
    CHACTION *a;

    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DeleteCharacterAnimations(): Invalid character");
        return 0;
    }
    if (a = (*ch)->actions) {
        a->stop = 1;
        while (a->next)
            ch_RemoveAction(a->next);
        return 0;
    }
    return 0;
}
/* 0x40cc80, returns the character standing on (or walking to) the dummy */

int chc_DummyBlocked(D3OBJECT **dummy, int *dist)
{
    D3VECTOR4 pos;
    CHARACTER *found = 0;
    CHACTION *a;
    int i;

    if (!*dummy) {
        evt_Error(evt_Cur, evt_Cur->pc, "DummyBlocked(): invalid dummy");
        return 0;
    }
    i = 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    for (; i < CH_MAX; i++) {
        if (ch_List[i] && ch_List[i]->scene == d3_DefaultScene && ch_List[i]->group == evt_NoGroup) {
            if (d3_VecEqual(&ch_List[i]->obj->pos, (D3VECTOR *)&pos, (float)*dist)) {
                found = ch_List[i];
                break;
            }
            a = ch_List[i]->actions;
            if (a && a->type == '-' && d3_VecEqual(&a->u.walk.tpos, (D3VECTOR *)&pos, (float)*dist)) {
                found = ch_List[i];
                break;
            }
        }
    }
    return (int)found;
}
/* 0x40cd70, CharacterCount(): the number of characters in the default scene */

int chc_CharacterCount(void)
{
    int n = 0;
    int i;

    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i] && ch_List[i]->scene == d3_DefaultScene)
            n++;
    }
    return n;
}
/* 0x40cda0, LookAtCharacter(ch, target): turns the character towards the target character (yaw from d3_YawTo, unwrapped against its current rotation) through action 7; 1 after an error, else 0 */

int chc_LookAtCharacter(CHARACTER **ch, CHARACTER **target)
{
    D3VECTOR rot;
    D3VECTOR b;
    D3VECTOR a;
    float angle;

    if (*ch && *target) {
        angle = d3_YawTo((*ch)->obj, &(*target)->obj->pos);
        rot.x = 0.0f;
        rot.y = angle;
        rot.z = 0.0f;
        d3_UnwrapAngles(&rot, &(*ch)->obj->rotation, &a, &b);
        rot.x += (*ch)->obj->rotation.x;
        rot.y += (*ch)->obj->rotation.y;
        rot.z += (*ch)->obj->rotation.z;
        ch_AddAction(*ch, 7, (int)(angle * 57.295776f), 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "LookAtCharacter(): invalid dest- or targetcharacter");
    return 1;
}
/* 0x40ce90, LookAtObject(ch, obj): turns the character towards the object's world position through action 7; 1 after an error, else 0 */

int chc_LookAtObject(CHARACTER **ch, D3OBJECT **obj)
{
    D3VECTOR4 pos;
    float angle;

    if (*ch && *obj) {
        d3_PointToWorld(*obj, &(*obj)->pos, (D3VECTOR *)&pos);
        angle = d3_YawTo((*ch)->obj, (D3VECTOR *)&pos);
        ch_AddAction(*ch, 7, (int)(angle * 57.295776f), 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "LookAtObject(): invalid dest- or targetobject");
    return 1;
}
/* 0x40cf10, SetCharacterToDummy(ch, dummy): puts the character at the dummy's world position, facing the dummy's direction; 1 after an error, else 0 */

int chc_SetCharacterToDummy(CHARACTER **ch, D3OBJECT **dummy)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;

    if (*ch && *dummy) {
        d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
        ch_SetPos(*ch, (D3VECTOR *)&pos, 1);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(*dummy, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot((*ch)->obj, &rot);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "SetCharacterToDummy(): invalid character or dummy");
    return 1;
}
/* 0x40cfd0, CharacterSitting(ch): 1 when the character's sit flag (flags byte 0x10) is set, else 0 (also 0 after an error) */

int chc_CharacterSitting(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterSitting(): invalid character");
        return 0;
    }
    if ((*ch)->flags.bytes.lo & 0x10)
        return 1;
    return 0;
}
/* 0x40d010, GetCharacterSubObjectHandle(ch, name): the character's sub-object (0x140: hands, head) with that name, or 0 */

int chc_GetCharacterSubObjectHandle(CHARACTER **ch, char **name)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "GetCharacterSubObjectHandle(): invalid character");
        return 0;
    }
    return (int)d3_FindObject((int)(*ch)->obj, 0x140, 0, *name);
}
/* 0x40d060, SetCharacterStepSample(name): sets the step sound sample name (ch_StepSample); always 1 */

int chc_SetCharacterStepSample(char **name)
{
    strcpy(ch_StepSample, *name);
    return 1;
}
/* 0x40d080, SetLowPoly(ch, on): shows the low-poly body of the character instead of the normal one; 1 (also after an error) */

int chc_SetLowPoly(CHARACTER **ch, int *on)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetLowPoly(): invalid character or dummy");
        return 1;
    }
    if (*on)
        (*ch)->flags.lowPoly = 1;
    else
        (*ch)->flags.lowPoly = 0;
    return 1;
}
/* 0x40d0d0, where is passed as the string itself (not a pointer to it) */

int chc_AttachObject(CHARACTER **ch, char **where, char **obj)
{
    int hand;

    if (!*ch)
        return 0;
    if (!strcmp((char *)where, "d3_LeftHand"))
        hand = 1;
    else if (!strcmp((char *)where, "d3_RightHand"))
        hand = 2;
    else if (!strcmp((char *)where, "d3_Head"))
        hand = 3;
    ch_SetHandObject(*ch, hand, *obj);
    return 1;
}
/* 0x40d150, SetCharacterTransperancy(ch, alpha): sets the alpha of the body and of its transport; 1 after an error, else 0 */

int chc_SetCharacterTransperancy(CHARACTER **ch, int *alpha)
{
    CHCALPHA f;

    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAni(): invalid character or dummy");
        return 1;
    }
    memset(&f, 0, sizeof(f));
    f.f0 = 0;
    f.alpha = *alpha;
    d3_SetObjectAlphaC((*ch)->obj, (*ch)->obj->info, f);
    if ((*ch)->transport)
        d3_SetObjectAlphaC(*(*ch)->transport, (*(*ch)->transport)->info, f);
    return 0;
}
/* 0x40d1e0, CharacterSitsDown(ch): 1 when the character's sit flag (flags byte 0x10) is set, else 0 (also 0 after an error) */

int chc_CharacterSitsDown(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterSitsDown(): invalid character");
        return 0;
    }
    if ((*ch)->flags.bytes.lo & 0x10)
        return 1;
    return 0;
}
/* 0x40d220, FindInCharacterName(ch, s): 1 when s occurs in the character's name, else 0 (also 0 after an error) */

int chc_FindInCharacterName(CHARACTER **ch, char **s)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "FindInCharacterName(): invalid character");
        return 0;
    }
    return strstr((*ch)->name, *s) != 0;
}
/* 0x40d270, registers the commands above (the last function of this file) */

void chc_Register(void)
{
    evt_ImportCommand("CreateCharacter", chc_CreateCharacter, 1, 4, 6, 7, 7, 7);
    evt_ImportCommand("CreateCharacterAtDummy", chc_CreateCharacterAtDummy, 1, 3, 6, 1, 6);
    evt_ImportCommand("KillCharacter", chc_KillCharacter, 5, 1, 1);
    evt_ImportCommand("WalkToDummy", chc_WalkToDummy, 5, 2, 1, 1);
    evt_ImportCommand("WalkToDummyRotate", chc_WalkToDummyRotate, 5, 2, 1, 1);
    evt_ImportCommand("WalkToDummyVerified", chc_WalkToDummyVerified, 5, 3, 1, 1, 1);
    evt_ImportCommand("WalkToDummyRotateVerified", chc_WalkToDummyRotateVerified, 5, 3, 1, 1, 1);
    evt_ImportCommand("PlayAnimation", chc_PlayAnimation, 5, 3, 1, 6, 1);
    evt_ImportCommand("PlayAnimationSound", chc_PlayAnimationSound, 5, 5, 1, 6, 1, 1, 6);
    evt_ImportCommand("PlayAnimationScript", chc_PlayAnimationScript, 5, 5, 1, 6, 1, 1, 6);
    evt_ImportCommand("PlayAnimationScriptInt", chc_PlayAnimationScriptInt, 5, 6, 1, 6, 1, 1, 6, 1);
    evt_ImportCommand("GetCharacterHandle", chc_GetCharacterHandle, 1, 1, 6);
    evt_ImportCommand("TakeObject", chc_TakeObject, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("TakeObjectLeft", chc_TakeObjectLeft, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("TakeObjectScript", chc_TakeObjectScript, 1, 6, 1, 6, 1, 1, 1, 6);
    evt_ImportCommand("GiveObject", chc_GiveObject, 1, 2, 1, 6);
    evt_ImportCommand("DropObject", chc_DropObject, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("DropObjectLeft", chc_DropObjectLeft, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("SitDown", chc_SitDown, 1, 2, 1, 6);
    evt_ImportCommand("SitDownAtOnce", chc_SitDownAtOnce, 1, 2, 1, 6);
    evt_ImportCommand("GetUp", chc_GetUp, 1, 2, 1, 6);
    evt_ImportCommand("SetCharacterCamera", chc_SetCharacterCamera, 5, 2, 1, 6);
    evt_ImportCommand("MoveCharacterCamera", chc_MoveCharacterCamera, 5, 3, 1, 6, 1);
    evt_ImportCommand("PreloadAnimation", chc_PreloadAnimation, 5, 5, 1, 6, 6, 6, 6);
    evt_ImportCommand("PreloadSitMesh", (int (*)())sv_ReturnZero, 5, 1, 1);
    evt_ImportCommand("CharacterIdle", chc_CharacterIdle, 1, 1, 1);
    evt_ImportCommand("StopCharacter", chc_StopCharacter, 5, 1, 1);
    evt_ImportCommand("KillCharacterAnimations", chc_KillCharacterAnimations, 5, 1, 1);
    evt_ImportCommand("DummyBlocked", chc_DummyBlocked, 1, 2, 1, 1);
    evt_ImportCommand("CharacterCount", chc_CharacterCount, 1, 0);
    evt_ImportCommand("LookAtCharacter", chc_LookAtCharacter, 5, 2, 1, 1);
    evt_ImportCommand("LookAtObject", chc_LookAtObject, 5, 2, 1, 1);
    evt_ImportCommand("SetCharacterToDummy", chc_SetCharacterToDummy, 5, 2, 1, 1);
    evt_ImportCommand("CharacterSitting", chc_CharacterSitting, 5, 1, 1);
    evt_ImportCommand("GetCharacterSubObjectHandle", chc_GetCharacterSubObjectHandle, 5, 2, 1, 6);
    evt_ImportCommand("SetCharacterStepSample", chc_SetCharacterStepSample, 5, 1, 6);
    evt_ImportCommand("SetLowPoly", chc_SetLowPoly, 5, 2, 1, 1);
    evt_ImportCommand("AttachObject", chc_AttachObject, 5, 3, 1, 6, 6);
    evt_ImportCommand("SetCharacterTransperancy", chc_SetCharacterTransperancy, 5, 2, 1, 1);
    evt_ImportCommand("CharacterSitsDown", chc_CharacterSitsDown, 5, 1, 1);
    evt_ImportCommand("FindInCharacterName", chc_FindInCharacterName, 1, 2, 1, 6);
}

char ch_StepSample[] = "normal_s";  /* 0x5f1230, "normal_s" (ch.c data) */
