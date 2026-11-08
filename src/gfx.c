/* gfx.c -- 0x476b90..0x479d00 of the "d2" 2D layer, probably D:\develop\Units\d2.v1\GFX.C (its own TU,
   not d2.c's): the item list shown next to the pointer (id / amount pairs, drawn with the pointer picture), pointer
   picture, hover and scroll wheel handling of windows, the surfaces of the item list, editing of input fields
   (keys, mouse drag, slider objects), input focus, mouse handling and hit rectangles, 24 bit bmp loading, and the
   GFX.C surfaces (create, free, lock, unlock, blits, pixels, clip). One TU: d2_FocusInput matches only with
   d2_CountPointerItems defined above it; the GFX.C __LINE__ 0x43f inside d2_MakeSurface fits a file that starts
   about at 0x476c00. Ends where shp.c's .data starts (0x5fed74). Names are ours.
   Open: the start (the d2.c seam near 0x476b90) is unproven; .bss is COMMON, no lead.
   Open: gfx.h is not canonical - most gfx.c functions are declared in d2.h, with copies in cm.h / rt.h.
   unit of rea 8cef7d5b (src/game/gfx.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3.h"
#include "di_input.h"
#include "tb.h"
#include "txt.h"
#include "dd_vesa.h"
#include "gfx.h"
#include "shp_bank.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */
#endif
#include "data_views.h"
#include "cm.h"
#include "d3_io_a.h"
#include "f3init.h"
#include "mem.h"
#include "sblib.h"
#include "sv.h"
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define d2_MoveObjI ((void (*)(int, int, int))d2_MoveObj)  /* 0x46cda0 (d2.c: short x, short y): @original this unit passes the coordinates as ints (d2_CreatePicObj) */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original the radio loop pushes 2 arguments (0x47868e / 0x478698: push 0 / push 1, push eax) */

/* 0x476b90, adds amount to the item list drawn next to the pointer: the entry of picture id (first match, else
   the first free one); no effect when amount is 0 or the list is full (6 entries) */
void d2_AddPointerItem(int id, int amount)
{
    int i;

    if (amount != 0) {
        for (i = 0; i < 6; i++)
            if (d2_PointerItems[i].id == id)
                break;
        if (i == 6) {
            for (i = 0; i < 6; i++)
                if (d2_PointerItems[i].id == -1)
                    break;
            if (i == 6)
                return;
        }
        d2_PointerItems[i].id = id;
        d2_PointerItems[i].amount += amount;
    }
}
/* 0x476c00, sets the entry of picture id in the item list drawn next to the pointer (first match, else the
   first free one); amount 0 frees the entry, a full list is no effect */

void d2_SetPointerItem(int id, int amount)
{
    int i;

    for (i = 0; i < 6; i++)
        if (d2_PointerItems[i].id == id)
            break;
    if (i == 6) {
        for (i = 0; i < 6; i++)
            if (d2_PointerItems[i].id == -1)
                break;
        if (i == 6)
            return;
    }
    d2_PointerItems[i].id = id;
    d2_PointerItems[i].amount = amount;
    if (amount == 0) {
        d2_PointerItems[i].amount = 0;
        d2_PointerItems[i].id = -1;
    }
}
/* 0x476c70, empties the item list drawn next to the pointer (all 6 entries free) */

void d2_ClearPointerItems(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        d2_PointerItems[i].amount = 0;
        d2_PointerItems[i].id = -1;
    }
}
/* 0x476c90, number of entries in use (id != -1) of the item list drawn next to the pointer */

int d2_CountPointerItems(void)
{
    int i, n = 0;

    for (i = 0; i < 6; i++)
        if (d2_PointerItems[i].id != -1)
            n++;
    return n;
}
/* 0x476cb0, draws the pointer at x, y on surf: the pointer picture (or the pointer sprite d2_PointerSprite when
   one is set, moved to x, y) and the item list drawn next to it, amounts under the pictures; locks the surface
   while it draws and unlocks it again unless the caller held it locked; no effect during a cutscene */

void d2_DrawPointer(void *surf, int x, int y, int a, int b)
{
    int n, px, py, saved, hold, i;
    int c0, c1, c2, c3;
    void *pic;
    int amt, id;
    D2FRCT *f;
    char buf[0x40];
    char *s, *d;

    if (d2_CutsceneActive == 0 && ((hold = ((D2SURF *)surf)->locked) != 0 || d2_LockSurface(surf) != 0)) {
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        d2_SetClipRect(0, 0, ((D2SURF *)surf)->w, ((D2SURF *)surf)->h);
        if (d2_PointerSprite == -1) {
            d2_DrawShape(x, y, surf, d2_PointerPic, (unsigned char)d2_PointerLook);
        } else {
            d2_AnimView[d2_PointerSprite].x = (short)x;
            d2_AnimView[d2_PointerSprite].y = (short)y;
            d2_DrawAnim(d2_PointerSprite, surf);
        }
        saved = d2_CurrentFont;
        d2_SetFont(d2_DefaultFont + 1);
        n = 0;
        for (i = 0; i < 6; i++) {
            f = &d2_PointerItems[i];
            if (f->id != -1) {
                px = x + d2_Pics[d2_PointerPicIndex].w + (n % 3) * 0x1a;
                py = (d2_FontHeight + 0x1a) * (n / 3) + y;
                pic = d2_GetPic(f->id);
                if (d2_DrawShapeScaled(px, py, surf, pic, d2_PicShift, -2, 0)) {
                    amt = f->amount;
                    id = f->id;
                    if (id >= 0xdf && id < 0x3bb) {
                        if (amt != -1) {
                            sprintf(buf, "%i", amt);
                        } else {
                            s = txt_Ptrs[0xd7];
                            d = buf;
                            while ((*d++ = *s++) != 0)
                                ;
                        }
                    } else {
                        sprintf(buf, "%i", amt);
                    }
                    d2_DrawTextBox(px - 8, py + 0x18, 0x28, d2_FontHeight, surf, buf, 8);
                }
                n++;
            }
        }
        d2_AddDirty(x, y, 0x80, 0x80);
        d2_SetFont(saved);
        d2_SetClipRect(c0, c1, c2, c3);
        if (hold == 0)
            d2_UnlockSurface(surf);
    }
}
/* 0x476f00, draws the pointer at the mouse position plus x, y */

int d2_DrawMousePointer(int x, int y)
{
    int a = 6, b = 6;
    int px = d2_MouseX + x;
    int py = d2_MouseY + y;
    if (d2_PointerPic == 0)
        d2_PointerPic = d2_GetPic(d2_FindPic("_MOUSE_CURSOR"));
    if (dm_L.ldown != 0) {
        px--;
        py++;
        a = 7;
        b = 5;
    }
    d2_DrawPointer(d2_ScreenSurf, px, py, a, b);
    return 1;
}
/* 0x476f80, sets (pic != 0) or removes the sprite of the pointer */

void d2_SetPointerSprite(int a, int pic)
{
    if (pic != 0) {
        if (d2_PointerSprite != -1) {
            d2_FreeAnim(d2_PointerSprite);
            d2_PointerSprite = -1;
        }
        d2_PointerSprite = d2_StartAnim(d2_MouseX, d2_MouseY, d2_GetPic(pic), 0);
        return;
    }
    if (d2_PointerSprite != -1) {
        d2_FreeAnim(d2_PointerSprite);
        d2_PointerSprite = -1;
    }
    d2_PointerLook = 0;
}
/* 0x477000, hover time and the scroll wheel / scroll buttons of windows @deep B3: 151 code hunks; read the whole body against 0x477000..0x47731c: flow, stores, conditions and calls are the same (reset block 5 stores, f8aac8/6ed9e4 if-else, held block with di_ClearEvents 0x10, f8a650/f8a652/f8ab64 stores, btndown/btnup order, both scroll-button branches, wup/wdown, saved font); the code hunks are instruction shape only: the original caches the mouse shorts in di/bx at entry and re-reads them after di_ClearEvents (0x4115b0) (mov di,WORD; movsx ecx,di), ours loads/re-uses them as 32-bit (mov eax,dword / mov di late); previous note already tried short locals (71 asm lines, no reload). No meaning error found */

void d2_Unk477000(void)
{
    int saved = -1;
    D2WIN *w, *tw;
    int i, step, id, bd, bu;

    if (d2_Unkf8aacc != d2_Unkf8ab64 || d2_MouseX < d2_Unk5febdc - 5 || d2_MouseY < d2_Unk5febe0 - 5 ||
        d2_MouseX > d2_Unk5febdc + 5 || d2_MouseY > d2_Unk5febe0 + 5 || dm_L.ldown != 0) {
        d2_RightHeldObj = -1;
        d2_Unkf8a654 = tb_AnimTick;
        d2_Unk6ed9e4 = 0;
        d2_Unk5febdc = d2_MouseX;
        d2_Unk5febe0 = d2_MouseY;
    }
    if (d2_RightHeldObj == -1) {
        if (d2_Unkf8aacc != -1 && dm_L.rhold != 0) {
            d2_RightHeldObj = d2_Unkf8aacc;
            d2_Unk6ed9e4 = 1;
            d2_Unk5febdc = d2_MouseX;
            d2_Unk5febe0 = d2_MouseY;
            goto held;
        }
    } else if (d2_Unk6ed9e4 != 0) {
held:
        if (dm_L.rhold == 0) {
            d2_RightHeldObj = -1;
            d2_Unkf8a654 = tb_AnimTick;
            d2_Unk6ed9e4 = 0;
            d2_Unk5febdc = d2_MouseX;
            d2_Unk5febe0 = d2_MouseY;
            di_ClearEvents(0x10);
        }
    }
    if (d2_CurrentFont != -1)
        saved = d2_CurrentFont;
    d2_Unkf8a650 = d2_MouseX;
    d2_Unkf8a652 = d2_MouseY;
    d2_Unkf8ab64 = d2_Unkf8aacc;
    for (i = 0; i < 0x60; i++) {
        w = &d2_Wins[i];
        if ((w->flags & 0x200) && w->active == 1 && w->font != 0)
            d2_SetFont(w->font);
    }
    if (d2_HitWindow != -1 && (w = &d2_Wins[d2_HitWindow]) != 0 && dm_L.ldown != 0 && (bd = w->btndown) != -1 &&
        (bu = w->btnup) != -1 && d2_FocusedInput == 0) {
        if (d2_PointerObj == bd) {
            tw = &d2_Wins[d2_Objs[bd].winIndex];
            if (tw->scrollRemY == tw->scrollStep || tw->scrollRemY == 0)
                d2_ScrollWindowBy(0, tw->visible, tw->id);
        } else if (d2_PointerObj == bu) {
            tw = &d2_Wins[d2_Objs[bu].winIndex];
            if (tw->scrollRemY == tw->scrollStep || tw->scrollRemY == 0)
                d2_ScrollWindowBy(0, -tw->visible, tw->id);
        }
    }
    if (d2_PointerWinVisible != -1) {
        w = &d2_Wins[d2_PointerWinVisible];
        if (w != 0) {
            if (dm_L.wup != dm_L.wdown && (step = w->visible) != 0 && d2_FocusedInput == 0) {
                if (dm_L.wup != 0) {
                    id = w->id;
                    step = -step;
                } else {
                    if (dm_L.wdown == 0)
                        goto done;
                    id = w->id;
                }
                d2_ScrollWindowBy(0, step, id);
            }
        }
    }
done:
    if (saved != -1)
        d2_SetFont(saved);
}
/* 0x477320, creates the surfaces of the item list @deep Q2 only left: D2SLOT.unk00/unk04 (the definition writes both -1 when creating / freeing each slot), and no reader of either field exists in any drafted function (grep D2SLOT/f8aa90/f8aaa0/f8aadc: only this unit; the surfaces are created and freed but not drawn yet), so no evidence for their role; the function, d2_MakeSurface, d2_ExtraSymbolsPic / d2_TextCursorPic / d2_TextCursorOffset and d2_ItemListSurf64/128/256 are renamed */

void d2_CreateItemListSurfaces(void)
{
    int desc[16];
    int i;

    d2_PointerPic = 0;
    d2_PointerLook = 0;
    d2_TextCursorOffset = (d2_TextCursorPic != d2_ExtraSymbolsPic) ? 0 : -7;
    memset(desc, 0, sizeof desc);
    desc[1] = 0x40;
    desc[2] = 0x40;
    d2_ItemListSurf64 = d2_MakeSurface(desc, 1);
    for (i = 0; i < 3; i++) {
        memset(desc, 0, sizeof desc);
        d2_ItemListSurf128[i].unk00 = -1;
        d2_ItemListSurf128[i].unk04 = -1;
        desc[1] = 0x80;
        desc[2] = 0x80;
        d2_ItemListSurf128[i].surf = d2_MakeSurface(desc, 3);
    }
    memset(desc, 0, sizeof desc);
    d2_ItemListSurf256.unk00 = -1;
    d2_ItemListSurf256.unk04 = -1;
    desc[1] = 0x100;
    desc[2] = 0x100;
    d2_ItemListSurf256.surf = d2_MakeSurface(desc, 3);
    for (i = 0; i < 6; i++) {
        d2_PointerItems[i].id = -1;
        d2_PointerItems[i].amount = 0;
    }
}
/* 0x477420, frees the surfaces of the item list @deep Q2 only left: D2SLOT.unk00/unk04 (both set -1 after freeing the slot's surface); no reader of either field in any drafted function, no evidence for their role; the function and d2_FreeSurface are renamed */

void d2_FreeItemListSurfaces(void)
{
    int i;

    if (d2_ItemListSurf64 != 0) {
        d2_FreeSurface(d2_ItemListSurf64);
        d2_ItemListSurf64 = 0;
    }
    for (i = 0; i < 3; i++) {
        if (d2_ItemListSurf128[i].surf != 0) {
            d2_FreeSurface(d2_ItemListSurf128[i].surf);
            d2_ItemListSurf128[i].surf = 0;
            d2_ItemListSurf128[i].unk00 = -1;
            d2_ItemListSurf128[i].unk04 = -1;
        }
    }
    d2_FreeSurface(d2_ItemListSurf256.surf);
    d2_ItemListSurf256.unk00 = -1;
    d2_ItemListSurf256.unk04 = -1;
    d2_ItemListSurf256.surf = 0;
}
/* 0x4774a0, sets the value of a number input from the mouse drag (val at y0, now at y1) */

void d2_DragInputValue(D2INPUT *in, int val, int y0, int y1)
{
    int step, max, min;

    step = d2_InputStep(in);
    in->value = val - step * ((y1 - y0) / 4);
    val = in->value;
    max = in->maxval;
    if (val > max) {
        in->value = max;
    } else {
        min = in->minval;
        if (val < min) {
            if (((char)in->flags & 0x40) == 0 || val != -1 || min != 0 || max != 0)
                in->value = min;
            else
                in->value = -1;
        }
    }
    _itoa(in->value, in->text, 10);
}
/* 0x477530, ends the editing of the input field */

void d2_EndInputEdit(void)
{
    if (d2_FocusedInput != 0) {
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        if (d2_LimitsChanged != -1) {
            d2_LimitsChanged = -1;
            di_MaxY = d2_SavedMaxY;
            di_MinY = d2_SavedMinY;
        }
    }
}

#define FOCUS_SET     d2_Objs[D2CUR->obj].highlighted = 1;     d2_InputDragOffsetY = di_MaxY - y;     d2_InputDragValue = D2CUR->value;     d2_InputDragReleased = 0;

/* 0x477590, keyboard input of the input field being edited */
void d2_InputKey(void)
{
    int y = dm_L.y;
    int len, max, i;
    D2WIN *w;

    if (kb_Last == 0)
        return;
    switch (kb_Last) {
    case 0xe:
        len = strlen(D2CUR->text);
        if (len > 0)
            D2CUR->text[len - 1] = 0;
        kb_Last = 0;
        break;
    case 0x1c:
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        d2_ClickUsed = 0;
        if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
        break;
    case 1:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x57:
    case 0x58:
        break;
    case 0xf:
        w = d2_Objs[D2CUR->obj].win;
        if (kb_Down[0x36] == 0 && kb_Down[0x2a] == 0) {
            if (d2_FocusObjIndex >= w->nobjs - 1)
                d2_FocusObjIndex = -1;
            for (i = d2_FocusObjIndex + 1; i < w->nobjs; i++) {
                if (d2_Objs[w->objs[i]].type == 0x41 && d2_Objs[w->objs[i]].disabled == 0) {
                    d2_FocusObjIndex = i;
                    d2_Objs[D2CUR->obj].highlighted = 0;
                    d2_FocusedInput = (D2INPUT *)d2_Objs[w->objs[i]].data;
                    FOCUS_SET
                    break;
                }
            }
        } else {
            if (d2_FocusObjIndex == 0)
                d2_FocusObjIndex = w->nobjs;
            for (i = d2_FocusObjIndex - 1; i >= 0; i--) {
                if (d2_Objs[w->objs[i]].type == 0x41 && d2_Objs[w->objs[i]].disabled == 0) {
                    d2_FocusObjIndex = i;
                    d2_Objs[D2CUR->obj].highlighted = 0;
                    d2_FocusedInput = (D2INPUT *)d2_Objs[w->objs[i]].data;
                    FOCUS_SET
                    break;
                }
            }
        }
        if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
        if ((D2CUR->flags & 1) != 0) {
            d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
            d2_TextCursorY = D2CUR->y;
            if (d2_TextCursorSprite == -1)
                d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - 7, d2_ExtraSymbolsPic + 8);
            else
                d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - 7, d2_TextCursorSprite);
        }
        kb_Last = 0;
        break;
    default:
        if (D2CUR->flags & 2) {
            if (kb_Last < 2 || kb_Last > 0xb || kb_Down[0x36] != 0 || kb_Down[0x2a] != 0)
                break;
            _itoa(D2CUR->value, D2CUR->text, 10);
        }
        len = strlen(D2CUR->text);
        if (D2CUR->flags & 1)
            max = D2CUR->maxval;
        else
            max = D2CUR->ndigits;
        if (len < max && (D2CUR->maxw == 0 || d2_TextCursorX <= D2CUR->x + D2CUR->maxw)) {
            if (kb_Down[0x36] == 0 && kb_Down[0x2a] == 0)
                D2CUR->text[len] = kb_FindKey(0, 0, 0, kb_Last);
            else
                D2CUR->text[len] = kb_FindKey(1, 0, 0, kb_Last);
            D2CUR->text[len + 1] = 0;
        }
        kb_Last = 0;
        break;
    }
    if (d2_FocusedInput != 0) {
        if (D2CUR->flags & 2)
            D2CUR->value = atoi(D2CUR->text);
        d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
        if (d2_TextCursorSprite != -1)
            d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - 7, d2_TextCursorSprite);
    }
}
/* 0x477a70, mouse drag on a number input */

void d2_DragInput(void)
{
    int step;
    char fl;

    if (d2_FocusedInput != 0 && (D2CUR->flags & 2) != 0) {
        if (dm_L.lextra != 0 && D2CUR->value < D2CUR->maxval)
            D2CUR->value = D2CUR->maxval;
        if (dm_L.ldown != 0 && D2CUR->value != -1) {
            if (d2_DragActive == 0) {
                step = d2_InputStep(D2CUR);
                d2_DragActive = 1;
                d2_SavedMaxY = di_MaxY;
                d2_SavedMinY = di_MinY;
                d2_DragStartValue = D2CUR->value;
                d2_DragStartX = dm_L.x;
                d2_DragStartY = dm_L.y;
                fl = (char)D2CUR->flags;
                if (fl & 0x80) {
                    di_MaxY = ((d2_DragStartValue - D2CUR->minval) * 4) / step + d2_DragStartY;
                    di_MinY = ((d2_DragStartValue - D2CUR->maxval) * 4) / step + d2_DragStartY;
                }
                D2CUR->frameY = ((d2_DragStartValue - D2CUR->minval) * 4) / step + d2_DragStartY;
            }
            d2_DragInputValue(D2CUR, d2_DragStartValue, d2_DragStartY, dm_L.y);
        }
    }
}
/* 0x477ba0, mouse and keys on the focused slider object @deep B3: 134 code + 4 slots hunks (20 fpu / 5 fill explained); read the body against 0x477ba0..0x477ec0: same flow (focus d2_Unk5febec, type 0x45, flags & 0x40, buf->unk198 picks d2_PointerX/0c, the ldown block loads f0/f6 and the six fields min/max/flags/len/value2, the fl & 0x48 / 2 / 1 branches with mn/v2/mx clamps, wheel: lclick/lhold, max < 100, hilo/hihi inc/dec, same clamps; the original spills flags/max/min/len/value2/range to six stack slots ([esp+0x10] keeps fl as a byte, read at the wheel) and takes edi from the entry, ours keeps max in ebx/ebp; the unit note records that all 120 orders of the five field loads were tried with no change. No meaning error found */

void d2_SliderInput(void)
{
    D2OBJ *o;
    D2FRAME *f0, *f6;
    void *h;
    int px, py, v, mn, mx, len, v2, d, a;
    unsigned int fl;

    if (dm_L.ldown == 0 || d2_Unk5febec == -1)
        d2_Unk5febec = -1;
    else
        d2_PointerObj = d2_Unk5febec;
    if (d2_PointerObj == -1)
        return;
    o = &d2_Objs[d2_PointerObj];
    if (o->type != 0x45)
        return;
    if (o->u.s.flags & 0x40)
        return;
    if (o->buf == 0 || o->buf->loaded == 0) {
        px = dm_L.x;
        py = dm_L.y;
    } else {
        px = d2_PointerX;
        py = d2_PointerY;
    }
    if (dm_L.ldown != 0) {
        h = o->data;
        f0 = d2_GetFrame(h, 0);
        d2_GetFrame(h, 1);
        d2_GetFrame(h, 3);
        f6 = d2_GetFrame(h, 6);
        o = &d2_Objs[d2_PointerObj];
        fl = o->u.s.flags;
        mx = o->u.s.max;
        mn = o->u.s.min;
        len = o->u.s.len;
        v2 = o->u.s.value2;
        d = mx - mn;
        d2_Unk5febec = d2_PointerObj;
        if ((fl & 0x48) == 0) {
            if ((fl & 2) && px > f6->w + o->x && px < o->w - f6->w + o->x) {
                a = px - f0->w - o->x;
                v = (int)(a * ((double)d / len) + mn + 0.5);
            } else if ((fl & 1) && py > f6->h + o->y && py < o->y + o->h - f6->h) {
                a = o->y + o->h - f0->h - py;
                v = (int)(a * ((double)d / len) + mn + 0.5);
            } else {
                goto wheel;
            }
            if (v < mn)
                v = mn;
            else if ((fl & 0x10) && v > v2)
                v = v2;
            else if (v > mx)
                v = mx;
            o->dirty = 1;
            d2_Objs[d2_PointerObj].u.s.value = v;
        }
    }
wheel:
    if (dm_L.lclick == 0) {
        if (dm_L.lhold == 0)
            return;
        if (d2_Objs[d2_PointerObj].u.s.max < 100)
            return;
    }
    mx = d2_Objs[d2_PointerObj].u.s.max;
    mn = d2_Objs[d2_PointerObj].u.s.min;
    o = &d2_Objs[d2_PointerObj];
    v = o->u.s.value;
    if ((fl & 2) == 0) {
        if (o->hilo != 0)
            v++;
        else if (o->hihi != 0)
            v--;
    } else {
        if (o->hilo != 0)
            v--;
        else if (o->hihi != 0)
            v++;
    }
    if (v < mn)
        v = mn;
    else if ((fl & 0x10) && v > v2)
        v = v2;
    else if (v > mx)
        v = mx;
    o->dirty = 1;
    d2_Objs[d2_PointerObj].u.s.value = v;
}
/* 0x477ed0, per frame handling of the input fields and sliders */

int d2_UpdateInput(void)
{
    D2OBJ *o;
    D2INPUT *p;
    D2WIN *w;
    int y, i;

    if (d2_DragActive != 0 && dm_L.lextra == 0 && (dm_L.ldown == 0 || d2_FocusedInput == 0 || (D2CUR->flags & 2) == 0)) {
        d2_DragActive = 0;
        di_MaxY = d2_SavedMaxY;
        di_MinY = d2_SavedMinY;
        d2_EndInputEdit();
    }
    y = dm_L.y;
    if (dm_L.ldown == 0)
        d2_ClickUsed = 0;
    if (d2_FocusedInput == 0 && d2_TextCursorSprite != -1) {
        d2_RemoveObj(d2_TextCursorSprite);
        d2_TextCursorSprite = -1;
    }
    if (d2_PointerObj == -1 || (o = &d2_Objs[d2_PointerObj])->type != 0x41 ||
        d2_PointerObj != (p = (D2INPUT *)o->data)->obj) {
focus:
        if (dm_L.ldown == 0)
            goto click;
        if (d2_FocusedInput != 0) {
            d2_ClickUsed = 1;
            goto click;
        }
    } else {
        if (dm_L.ldown != 0) {
            if (D2CUR != p && d2_ClickUsed == 0) {
                if (D2CUR != 0)
                    d2_Objs[D2CUR->obj].highlighted = 0;
                d2_FocusedInput = p;
                d2_Objs[p->obj].highlighted = 1;
                d2_InputDragOffsetY = di_MaxY - y;
                d2_InputDragValue = p->value;
                d2_InputDragReleased = 0;
                w = d2_Objs[D2CUR->obj].win;
                i = 0;
                if (w != 0) {
                    while (1) {
                        if (i >= w->nobjs)
                            break;
                        if (w->objs[i] == p->obj)
                            d2_FocusObjIndex = i;
                        i++;
                    }
                }
                if ((D2CUR->flags & 0x40) != 0 && D2CUR->maxval == 0 && D2CUR->minval == 0) {
                    if (d2_CountPointerItems() == 0 && D2CUR->value == 0) {
                        D2CUR->value = -1;
                    }
                }
                if (D2CUR->flags & 1) {
                    d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
                    d2_TextCursorY = D2CUR->y;
                    if (d2_TextCursorSprite == -1) {
                        d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorPic);
                    } else {
                        d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorSprite);
                    }
                } else {
                    if (d2_TextCursorSprite != -1) {
                        d2_RemoveObj(d2_TextCursorSprite);
                        d2_TextCursorSprite = -1;
                    }
                }
            }
            goto focus;
        }
click:
        if (d2_FocusedInput != 0 && d2_PointerObj == D2CUR->obj && (D2CUR->flags & 2) != 0 && dm_L.lclick != 0) {
            if (D2CUR->maxval > D2CUR->value && D2CUR->value != -1 && D2CUR->icon != -1) {
                D2CUR->value += d2_InputStep(D2CUR);
            }
            if (D2CUR->maxval == 0 && D2CUR->minval == 0) {
                if (d2_CountPointerItems() == 0 && D2CUR->value == 0 && (D2CUR->flags & 0x40) != 0) {
                    D2CUR->value = -1;
                }
            }
        }
    }
    if (dm_L.rup != 0) {
        if (d2_FocusedInput == 0)
            goto last;
        if ((D2CUR->flags & 1) != 0) {
            di_ClearEvents(0x10);
        }
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        d2_ClickUsed = 0;
        if (d2_TextCursorSprite == -1)
            goto last;
        d2_RemoveObj(d2_TextCursorSprite);
        d2_TextCursorSprite = -1;
    }
    if (d2_FocusedInput != 0) {
        if (dm_L.ldown == 0) {
            d2_InputDragReleased = 1;
            dm_L.lclick = 0;
        }
        d2_InputKey();
        d2_DragInput();
        if (d2_FocusedInput != 0 && (D2CUR->flags & 0x10) != 0 && d2_PointerObj == D2CUR->obj && dm_L.lclick != 0) {
            D2CUR->currentTextId = D2CUR->currentTextId + 1;
            if (D2CUR->currentTextId > D2CUR->textCount - 1 + D2CUR->firstTextId)
                D2CUR->currentTextId = D2CUR->firstTextId;
            d2_InputSetString(D2CUR->id, D2CUR->currentTextId);
        }
    }
last:
    d2_SliderInput();
    if (d2_PointerObj != -1 && (d2_Objs[d2_PointerObj].style & 0x40) != 0)
        d2_CopyCaption(d2_Objs[d2_PointerObj].caption);
    return 0;
}
/* 0x478320, gives the input object obj the focus */

void d2_FocusInput(int obj)
{
    D2INPUT *in;
    D2WIN *w;
    int i;

    in = (D2INPUT *)d2_Objs[obj].data;
    if (d2_FocusedInput != in) {
        if (d2_FocusedInput != 0)
            d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = in;
        d2_Objs[in->obj].highlighted = 1;
        w = d2_Objs[D2CUR->obj].win;
        i = 0;
        if (w != 0) {
            while (1) {
                if (i >= w->nobjs)
                    break;
                if (w->objs[i] == in->obj)
                    d2_FocusObjIndex = i;
                i++;
            }
        }
        if ((D2CUR->flags & 0x40) != 0 && D2CUR->maxval == 0 && D2CUR->minval == 0) {
            if (d2_CountPointerItems() == 0 && D2CUR->value == 0)
                D2CUR->value = -1;
        }
        if ((D2CUR->flags & 2) != 0 && D2CUR->maxval != 0)
            D2CUR->value = 1;
        if ((D2CUR->flags & 1) != 0) {
            d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
            d2_TextCursorY = D2CUR->y;
            if (d2_TextCursorSprite == -1)
                d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorPic);
            else
                d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorSprite);
        } else if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
    }
}

/* 0x4784c0, mouse handling: object and window under the pointer, clicks (sound, radio groups, toggles), the hit
   rectangles, dragging the scroll bar of a window */
#define RAD(i) ((D2RADIO *)((char *)d2_Radios + (i)))
/* 0x4784c0 @deep B3, 257 code hunks / 602 of 896 bytes, register allocation: the original hoists no zero register (test / mov esi,[mem]; test esi,esi where ours keeps 0 in edi and uses cmp [mem],edi) and the mouse / hit loads repeat differently; the flow, branches and stores read the same (the old note's fit: swapped branches, comma-expression call argument, do-while radio loop); readability is also open here: Q2 (27 mouse / hit table identifiers), Q4 (2 casts) and Q7 (d2_SetObjValue, d3io_NoopCallback declared K&R) */
void d2_MouseUpdate(void)
{
    D2OBJ *o;
    D2CTRLX *c;
    D2HIT *h;
    D2WIN *w;
    int *p, *q;
    int i, j, k, m, n, found, win, v, line;
    short mx, my;

    d2_Unkf8aacc = d2_HitTest(d2_MouseX, d2_MouseY);
    d2_UpdateInput();
    if (d2_PointerWin != -1) {
        d2_HitWindow = d2_PointerWin;
        d2_Unkf8aa80 = -1;
        for (i = 0; i < d2_Wins[d2_PointerWin].nobjs; i++) {
            if (d2_Wins[d2_PointerWin].objs[i] == d2_PointerObj) {
                d2_Unkf8aa80 = i;
                break;
            }
        }
    } else {
        d2_HitWindow = -1;
    }
    d2_Unk477000();
    if (dm_L.lclick != 0) {
        k = d2_PointerObj;
        if (d2_Unkf8aacc != -1 && ((o = &d2_Objs[d2_PointerObj])->checkable != 0 || o->interactive != 0) && o->type != 0x45) {
            d2_ClickedId = d2_Unkf8aacc;
            di_GuiClick = 1;
            c = (D2CTRLX *)o->buf;
            if (c == 0) {
                d2_ClickedForm = -1;
            } else if (c->shown != 0 && c->loaded == 0) {
                d2_ClickedForm = c->id;
            }
            sb_PlaySampleI(d2_Unk6ed990,
                          o->checkable != 0 ? (o->checked ^= 1, d2_Objs[d2_PointerObj].highlighted = d2_Objs[d2_PointerObj].checked,
                                           "MausclickLinks_s_01")
                                        : d2_Unkf8aacc == 1 ? "MausclickRechts_s_01" : "MausclickLinks_s_01",
                          1, 0x3f, 0x7f, 0, 0);
            found = 0;
            k = d2_PointerObj;
            i = 0;
            do {
                if (found)
                    break;
                j = 0;
                if (RAD(i)->n > 0) {
                    p = RAD(i)->objs;
                    do {
                        if (*p == k) {
                            m = 0;
                            if (RAD(i)->n > 0) {
                                q = RAD(i)->objs;
                                do {
                                    n = *q;
                                    if (n == k) {
                                        d2_SetObjValue2(n, 1);
                                        RAD(i)->sel = m;
                                    } else {
                                        d2_SetObjValue2(n, 0);
                                    }
                                    m++;
                                    q++;
                                    k = d2_PointerObj;
                                } while (m < RAD(i)->n);
                            }
                            found = 1;
                        }
                        j++;
                        p++;
                    } while (j < RAD(i)->n);
                }
                i += 0x8c;
            } while (i < 0x460);
            if (d2_ClickedId != d2_Unkf8aa9c)
                d2_Unkf8aa9c = d2_ClickedId;
            d2_Unkf8a65c = d2_Unkf8aa80;
        }
        if (k != -1)
            d2_ClickedObj = &d2_Objs[k];
    } else {
        d2_ClickedId = -1;
        d2_ClickedObj = 0;
        d2_Unkf8a65c = -1;
    }
    my = d2_MouseY;
    mx = d2_MouseX;
    d2_HitClicked = -1;
    d2_HitUnderPointer = -1;
    for (i = 0; i < 0x20; i++) {
        h = &d2_HitRects[i];
        if (!(mx < h->x || h->x + h->w < mx || my < h->y || h->y + h->h < my || h->used == 0)) {
            d2_HitUnderPointer = i;
            if (dm_L.lclick != 0)
                d2_HitClicked = i;
            break;
        }
    }
    win = d2_HitWindow;
    if (win != -1) {
        w = &d2_Wins[win];
        if (dm_L.ldown != 0 && (w->flags & 0x20) != 0 && w->w - 0xc + w->x < mx) {
            v = w->y;
            if (v + 10 < my && my < w->h - 10 + v) {
                n = ((my - v - 0xc) * w->contentHeight) / (w->h - 0x18) + 4;
                w->scrollY = n;
                line = n / d2_FontHeight;
                w->firstline = line;
                *(int *)((char *)w + 0x34) = *(int *)((char *)w + 0x38 + line * 4);
                if (n != w->appliedScrollY)
                    d3io_NoopCallback(win);
            }
        }
    }
}
/* 0x478870, copies a string to d2_CaptionCopy */

void d2_CopyCaption(char *s)
{
    char c;
    int off = (int)d2_CaptionCopy - (int)s;

    do {
        c = *s;
        s[off] = c;
        s++;
    } while (c != 0);
}
/* 0x478890, adds a hit rectangle, returns its number or 0 if the table is full */

int d2_AddHitRect(short x, short y, short w, short h, int user)
{
    D2HIT *p;
    int i = d2_HitCount + 1;

    d2_HitCount = i;
    d2_HitUnderPointer = i;
    if (i > 0x40)
        return 0;
    p = &d2_HitRects[i];
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->user = user;
    p->used = 1;
    return i;
}
/* 0x4788f0, removes hit rectangle i */

void d2_RemoveHitRect(int i)
{
    if (d2_HitCount >= 1 && i >= 0) {
        memset(&d2_HitRects[i], 0, sizeof(D2HIT));
        d2_HitCount--;
    }
}
/* 0x478930, 0 if the two colours are equal, else 1 */

int d2_CompareColour(char *a, char *b)
{
    if (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
        return 0;
    return 1;
}
/* 0x478960, sets a colour */

void d2_SetColour(D2RGB *c, unsigned char r, unsigned char g, unsigned char b)
{
    c->r = r;
    c->g = g;
    c->b = b;
}
/* 0x478980, width | height << 16 of a 24 bit bmp, -1 if it can not be opened, -2 if it is not 24 bit */

unsigned int d2_GetBmpSize(char *name)
{
    FILE *f;
    BITMAPINFOHEADER bi;

    f = fopen(name, "rb");
    if (f != 0) {
        fseek(f, 0xe, 0);
        fread(&bi, 1, 0x28, f);
        fclose(f);
        if (bi.biPlanes == 1 && bi.biBitCount == 0x18 && bi.biCompression == 0)
            return bi.biHeight << 16 | bi.biWidth;
        return 0xfffffffe;
    }
    return 0xffffffff;
}
/* 0x478a00, reads the pixels of a 24 bit bmp (bottom-up files flipped, bgr turned to rgb) to dst @deep B3 127 code hunks (93 of 132 instructions differ): register allocation only. Original: dst=ebx (loaded at 0x478a7e), i=edi, pad=ebp, ebx saved after the fopen guard at 0x478a20, w*3 re-materialised per iteration (lea edx,[ecx+ecx*2] at 0x478ab2); ours: the w3 local owns ebx, i=ebp, pad=edi, ebx pushed in the prologue at 0x478a07, dst reloaded from the stack in the fread address. Listing read end to end (fopen/fseek/fread/fclose, abs height, flip kept in the dead name slot, n = w*h, bgr->rgb swap): no meaning error left. spell.py entries tried, one edit/build each, all reverted as WORSE: (1) w3 inlined into the fread args -> 255/336; (2) early-return fopen guard -> 251/320 (size 320 vs 336); (3) flip as ternary y -> 191/336 (gets the register test but loses the push order). dup-tail: both fclose arms already duplicated in the source; push-pop: the callee-saved save position has no source lever left. Readability done: Q1 fixed, named from the body and caller (reads a 24 bit bmp's pixels into a surface buffer, flips bottom-up rows, bgr->rgb). Remaining: register assignment / re-materialisation, deep's toolbox. */

int d2_ReadBmpPixels(char *name, unsigned char *dst)
{
    FILE *f;
    BITMAPINFOHEADER bi;
    int i, y, flip, n;
    unsigned int pad;
    int w3;
    unsigned char *p, t;

    f = fopen(name, "rb");
    if (f != 0) {
        fseek(f, 0xe, 0);
        fread(&bi, 1, 0x28, f);
        fseek(f, bi.biClrUsed * 4 + 0x36, 0);
        if (bi.biCompression == 0 && bi.biPlanes == 1 && bi.biBitCount == 0x18) {
            flip = bi.biHeight >= 0;
            bi.biHeight = abs(bi.biHeight);
            i = 0;
            w3 = bi.biWidth * 3;
            if (bi.biHeight > 0) {
                pad = w3 & 3;
                do {
                    y = i;
                    if (flip)
                        y = bi.biHeight - i - 1;
                    fread(dst + y * bi.biWidth * 3, 1, w3, f);
                    if (pad != 0)
                        fseek(f, 4 - pad, 1);
                    i++;
                } while (i < bi.biHeight);
            }
            fclose(f);
            n = bi.biWidth * bi.biHeight;
            if (n > 0) {
                p = dst + 2;
                do {
                    t = p[-2];
                    p[-2] = *p;
                    *p = t;
                    p += 3;
                    n--;
                } while (n != 0);
                return 1;
            }
        } else {
            fclose(f);
        }
        return 1;
    }
    return 0;
}
/* 0x478b50, makes a 24 bit surface of a bmp, 0 on failure */

D2SURF *d2_LoadBmpSurface(char *name)
{
    D2SURF d;
    D2SURF *s;
    unsigned int v;
    int w, h;

    memset(&d, 0, sizeof(d));
    v = d2_GetBmpSize(name);
    if ((int)v < 0)
        return 0;
    w = v & 0xffff;
    h = (int)v >> 16;
    if (h < 0)
        h = abs(h);
    d.h = h;
    d.w = w;
    d.bpp = 0x18;
    s = d2_MakeSurface(&d, 0);
    if (s == 0)
        return 0;
    d2_ReadBmpPixels(name, (unsigned char *)s->bits);
    return s;
}
/* 0x478bd0, rgb bytes (w x h) to the 16 bit pixels of surface s @deep B3 8 code hunks / 9 differing instructions, all one fold: the original keeps the row pointer at src+1 (lea esi,[edx+0x1] at 0x478bed, store [esp+0x28]) and reads the first pixel as p[-1],p[0],p[1] (0x478bff mov al,[esi+1]; 0x478c02 mov cl,[esi]; 0x478c04 mov dl,[esi-1]); ours rebases p to src (mov esi,edx; loads [esi+2],[esi+1],[esi]) - same addresses, same dd_RGB call, 35 equal instructions. Meaning read against 0x478bd0..0x478c4f: stride=w*3, rp=src+1, p=rp, y< h / x<w unsigned loops, store s->bits[s->pitch*y+x], rp += stride - all correspond. diffclass verdict: 3 hunks, code 9, 'compare the meaning with the original disassembly first' - done, no meaning error. spell.py: no hunk matched the menu. Tried (one edit/build): rp = src; rp++; -> identical fold, no change, reverted. An earlier session recorded the same fold as unsolved, everything else equal. The +1 induction-variable rebase is VC6's optimizer with no source lever -> deep. Readability done: Q1 fixed (converts rgb bytes to 16 bit pixels, sibling 0x478c50 is the reverse). */

void d2_ConvertRgbTo16(int a, int b, unsigned int w, unsigned int h, unsigned char *src, int c, int d, D2SURF *s)
{
    unsigned int x, y, stride;
    unsigned char *p, *rp;

    y = 0;
    if (h > 0) {
        stride = w * 3;
        rp = src + 1;
        do {
            x = 0;
            p = rp;
            if (w > 0) {
                do {
                    s->bits[s->pitch * y + x] = (unsigned short)dd_RGB(p[-1], p[0], p[1]);
                    x++;
                    p += 3;
                } while (x < w);
            }
            y++;
            rp += stride;
        } while (y < h);
    }
}
/* 0x478c50, 16 bit pixels of src (w x h) to rgb bytes @deep B3 29 code + 12 slots hunks (60/112 bytes): the same +1 pointer fold as its sibling 0x478bd0 (escalated this session). The original keeps the row pointer at dst+1 (lea esi,[eax+0x1] at 0x478c65, slot [esp+0x1c] = the dead w parameter slot) and passes p-1,p,p+1 (lea edx,[esi-1]; push esi; lea ecx,[esi+1]); ours rebases p to dst (dec esi at 0x478c73), pushes [esi+2],[esi+1],esi and stores rp in the dead dst parameter slot [esp+0x30] - the 12 slot hunks are that different dead-parameter slot and follow from the fold. Meaning read against 0x478c50..0x478cbc: y<h and x<w unsigned loops, rp=dst+1, p=rp, dd_SplitRGB(src[dd_PitchPixels*y+x], p-1, p, p+1), p+=3, rp+=w*3, all correspond; the 8 instructions around the call are equal. Q2 fixed: dd_PitchPixels is the screen pitch in pixels, now dd_PitchPixels (dd_vesa.h) in gfx.c; its duplicate extern in d2.h removed and d2.c's two uses renamed. spell.py: no hunk matched the menu; the sibling's tried spelling (rp=src;rp++) changed nothing. The induction-variable +1 rebase is VC6's optimizer with no source lever -> deep. */

void d2_Convert16ToRgb(int a, int b, unsigned int w, unsigned int h, unsigned short *src, int c, int d, unsigned char *dst)
{
    unsigned int x, y;
    unsigned char *p, *rp;

    y = 0;
    if (h > 0) {
        rp = dst + 1;
        do {
            x = 0;
            p = rp;
            if (w > 0) {
                do {
                    dd_SplitRGB(src[dd_PitchPixels * y + x], p - 1, p, p + 1);
                    x++;
                    p += 3;
                } while (x < w);
            }
            rp += w * 3;
            y++;
        } while (y < h);
    }
}

#line 1 "D:\\develop\\Units\\d2.v1\\GFX.C"

/* 0x478cc0, creates a surface of the description desc (0x40 bytes); flags 1: depth of the screen, 2: a DirectDraw
   surface, 4: wraps an existing one, 8: system memory
   @deep B3 3 hunk areas / 54 differing instructions (393/752 bytes), all codegen; meaning read against 0x478cc0..0x478fb1 (mem_Alloc(0x40), *s=*desc, flags&4 -> external, flags&1 -> dd_BitPlanes, own-bit alloc, wrap description, flags&2 CreateSurface path with the format switch, D3DCHECK, SetColorKey, second GetSurfaceDesc, clip/pitch tail): no meaning error. (1) the ddsCaps expression in both branches: original emits movsx eax,bl; and eax,8; shl eax,8; or al,0x40 (sign-extends the low flags byte BEFORE the mask), ours folds the cast into and ebx,8 / shl ebx,8 / or ebx,0x40; (2) the else branch's OR wants 0x40 materialised in ecx (or eax,ecx), ours or ebx,0x40; (3) tail order: original stores pitch then left/top=0 after the idiv and pops edi/esi after the last desc->h store, ours hoists top=0 before the pitch store and pops early. Tried, one build each, all reverted (identical code or worse): signed char flags parameter (frame 0x108 vs 0x104, byte reload, REGRESSED d2_CopySurface), unsigned int flags (identical), char local f=(char)flags used as (f&8) (identical - VC6 propagates the cast), f=flags implicit (identical). The old d2q note recorded the same movsx and the char/signed char/unsigned char param + int local + shift cast attempts. Making VC6 keep the char conversion needs variant rounds. Readability done: Q2 unk38 -> D2SURF.external (flags&4 sets it, it skips the bit alloc and the free; the screen surface is passed flags 4). */
D2SURF *d2_MakeSurface(D2SURF *desc, int flags)
{
    LPDIRECTDRAWSURFACE4 dd_DDSOne;
    DDCOLORKEY ck;
    DDSURFACEDESC2 ddsd;
    DDSURFACEDESC2 sd;
    D2SURF *s;

    ck.dwColorSpaceLowValue = 0;
    ck.dwColorSpaceHighValue = 0;
    s = (D2SURF *)mem_Alloc(0x40, "gfx:gfx_CreateSurface:s");
    if (s == 0)
        return 0;
    *s = *desc;
    if (flags & 4)
        s->external = 1;
    if (flags & 1)
        s->bpp = dd_BitPlanes;
    if (s->DDSurface == 0) {
        if (s->external == 0) {
            if (!(flags & 2)) {
                s->rowbytes = (s->bpp >> 3) * s->w;
                s->bits = (unsigned short *)mem_Alloc(s->h * s->rowbytes, "gfx:gfx_CreateSurface:s->data");
            }
        }
    }
    if (s->external != 0 && s->DDSurface != 0) {
        sd.dwSize = 0x7c;
        s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, &sd);
        s->h = sd.dwHeight;
        s->w = sd.dwWidth;
        s->bpp = (unsigned char)sd.ddpfPixelFormat.dwRGBBitCount;
        s->rowbytes = sd.lPitch;
        s->bits = 0;
    }
    if ((flags & 2) && s->external == 0) {
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = 0x7c;
        if (flags & 1) {
            dd_BackBuffer->lpVtbl->GetSurfaceDesc(dd_BackBuffer, &ddsd);
            ddsd.dwFlags = 0x1007;
            ddsd.ddsCaps.dwCaps = ((char)flags & 8) << 8 | 0x40;
            ddsd.dwHeight = s->h;
            ddsd.dwWidth = s->w;
        } else {
            ddsd.dwFlags = 0x1007;
            ddsd.ddsCaps.dwCaps = ((char)flags & 8) << 8 | 0x40;
            ddsd.dwHeight = s->h;
            ddsd.dwWidth = s->w;
            ddsd.ddpfPixelFormat.dwSize = 0x20;
            switch (s->bpp) {
            case 8:
                ddsd.ddpfPixelFormat.dwFlags = 0x60;
                break;
            case 0xf:
                ddsd.ddpfPixelFormat.dwFlags = 0x40;
                ddsd.ddpfPixelFormat.dwRBitMask = 0x7c00;
                ddsd.ddpfPixelFormat.dwGBitMask = 0x3e0;
                ddsd.ddpfPixelFormat.dwBBitMask = 0x1f;
                ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
                break;
            case 0x10:
                ddsd.ddpfPixelFormat.dwFlags = 0x40;
                ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
                ddsd.ddpfPixelFormat.dwGBitMask = 0x7e0;
                ddsd.ddpfPixelFormat.dwBBitMask = 0x1f;
                ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
                break;
            }
            ddsd.ddpfPixelFormat.dwRGBBitCount = s->bpp;
        }
#line 1087
        D3DCHECK(dd_DD->lpVtbl->CreateSurface( dd_DD, &ddsd, &dd_DDSOne, NULL ));
        dd_SetSurfaceName(dd_DDSOne, "gfx_CreateSurface", 0);
        s->DDSurface = dd_DDSOne;
        dd_DDSOne->lpVtbl->SetColorKey(dd_DDSOne, 8, &ck);
        if (dd_DDSOne == 0) {
            mem_Free(s);
            return 0;
        }
        sd.dwSize = 0x7c;
        s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, &sd);
        s->h = sd.dwHeight;
        s->w = sd.dwWidth;
        s->bpp = (unsigned char)sd.ddpfPixelFormat.dwRGBBitCount;
        s->rowbytes = sd.lPitch;
    }
    s->left = 0;
    s->pitch = s->rowbytes / (s->bpp >> 3);
    s->top = 0;
    s->right = desc->w;
    s->bottom = desc->h;
    return s;
}
/* 0x478fc0, frees a surface */

int d2_FreeSurface(D2SURF *s)
{
    if (s == 0)
        return 0;
    if (s->locked != 0)
        d2_UnlockSurface(s);
    if (s->external == 0) {
        if (s->bits != 0) {
            mem_Free(s->bits);
            s->bits = 0;
        }
        if (s->DDSurface != 0)
            dd_ReleaseSurface(s->DDSurface);
    }
    mem_Free(s);
    return 1;
}
/* 0x479020, locks a surface: its pixels are at bits */

int d2_LockSurface(D2SURF *s)
{
    DDSURFACEDESC2 ddsd;

    if (s == 0)
        return 0;
    if (s->locked != 0)
        return 1;
    if (s->DDSurface != 0) {
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = 0x7c;
        if (dd_Mode == 3) {
#line 1178
            D3DCHECK(s->DDSurface->lpVtbl->Lock( s->DDSurface, NULL, &ddsd, DDLOCK_WAIT, NULL ));
        } else {
#line 1180
            D3DCHECK(s->DDSurface->lpVtbl->Lock( s->DDSurface, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL ));
        }
        s->bits = (unsigned short *)ddsd.lpSurface;
    }
    s->locked = 1;
    return 1;
}
/* 0x4790e0, unlocks a surface */

int d2_UnlockSurface(D2SURF *s)
{
    if (s->locked == 0)
        return 0;
    if (s->DDSurface != 0) {
        dd_UnlockSurface(s->DDSurface);
        s->bits = 0;
    }
    s->locked = 0;
    return 1;
}
/* 0x479120, fills desc with the description of the DirectDraw surface of s */

int d2_GetSurfaceDesc(D2SURF *s, DDSURFACEDESC2 *desc)
{
    if (s->DDSurface == 0)
        return 0;
    desc->dwSize = 0x7c;
    return s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, desc) == 0;
}
/* 0x479150, copies the part (sx, sy, w, h) of src to (x, y) of dst, clipped to both; mode 1: with the colour key @deep B3 far draft: 223 code hunks (681/816 bytes, 125 differing instructions of 289). Meaning read against 0x479150..0x47947d: the clip chain (y/dst->top, x/dst->left, sy/src->top, sx/src->left, then the four bottom/right clamps), so/d from bpp>>3 and rowbytes, the manual memcpy path when a surface lacks a DirectDraw surface or manualBlit is set, the Blt with DDBLT_WAIT / DDBLT_WAIT|DDBLT_KEYSRC, the 0x80004001 fallback that marks both surfaces manualBlit and memcpies, and the lock restore at the end - no meaning error. What differs is register/slot assignment and scheduling: the original keeps dst in ebx, src in ebp and y in edx, hoists the first sy/top load before the cmp and keeps sy/h in slots 0x3c/0x44; ours swaps those roles, reorders the loads and splits so/d differently. spell.py: cmp-test at 0x479430 tried (branch polarity dstLocked: the original is 'if (dstLocked != 0) Lock else Unlock', source flipped - kept, 225 -> 223 code hunks); the dup-tail entries at 0x479162 / 0x4792c6 / 0x47947a are the load/store scheduling of the clip blocks and of so/d (the source has no shared tail to pull), no concrete source form left. Readability done: Q1 named (blits a clipped part of a surface with optional colour key and manual fallback), Q2 D2SURF.unk3c -> manualBlit (set when the hardware Blt returns 0x80004001, checked to force the memcpy path), Q3 0x1000000/0x1008000 -> DDBLT_WAIT / DDBLT_WAIT|DDBLT_KEYSRC. */

int d2_BlitPartMode(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst, unsigned char mode)
{
    RECT rs, rd;
    int so, d, i, srcLocked, dstLocked, n, bf;
    unsigned int len;
    unsigned int *ps, *pd;
    unsigned char *bs, *bd;

    if (y < dst->top) {
        sy = sy + (dst->top - y);
        h = h + (y - dst->top);
        y = dst->top;
    }
    if (x < dst->left) {
        sx = sx + (dst->left - x);
        w = w + (x - dst->left);
        x = dst->left;
    }
    if (sy < src->top) {
        h = h + (sy - src->top);
        sy = src->top;
    }
    if (sx < src->left) {
        w = w + (sx - src->left);
        sx = src->left;
    }
    if (h + y > dst->bottom)
        h = dst->bottom - y;
    if (h + sy > src->bottom)
        h = src->bottom - sy;
    if (w + x > dst->right)
        w = dst->right - x;
    if (w + sx > src->right)
        w = src->right - sx;
    if (h < 1 || w < 1)
        return 0;
    so = (src->bpp >> 3) * sx + src->rowbytes * sy;
    d = (dst->bpp >> 3) * x + dst->rowbytes * y;
    srcLocked = src->locked;
    dstLocked = dst->locked;
#ifdef REA_FRAME_DUMP
    if (w > 300)
        REA_DUMP_TRACE((rd_f, "%u BlitPart %dx%d from %d,%d to %d,%d srcbpp=%d dstbpp=%d dd=%d,%d manual=%d,%d mode=%d\n",
                        GetTickCount(), w, h, sx, sy, x, y, src->bpp, dst->bpp, src->DDSurface != 0, dst->DDSurface != 0,
                        src->manualBlit, dst->manualBlit, mode));
#endif
    if (dst->DDSurface == 0 || src->DDSurface == 0 || dst->manualBlit != 0 || src->manualBlit != 0) {
        d2_LockSurface(dst);
        d2_LockSurface(src);
        {
            for (i = 0; i < h; i++) {
                len = (src->bpp >> 3) * w;
                bs = (unsigned char *)src->bits + src->rowbytes * i + so;
                bd = (unsigned char *)dst->bits + dst->rowbytes * i + d;
                memcpy(bd, bs, len);
            }
        }
    } else {
        if (dstLocked != 0)
            d2_UnlockSurface(dst);
        if (src->locked != 0)
            d2_UnlockSurface(src);
        rs.top = sy;
        rs.bottom = sy + h;
        rs.right = sx + w;
        rd.right = w + x;
        rd.bottom = h + y;
        rd.left = x;
        rs.left = sx;
        rd.top = y;
        bf = DDBLT_WAIT;
        if (mode & 1)
            bf = DDBLT_WAIT | DDBLT_KEYSRC;
        if (dst->DDSurface->lpVtbl->Blt(dst->DDSurface, &rd, src->DDSurface, &rs, bf, 0) != 0)
            return 0;
        if (d3d_hr == 0x80004001) {
            dst->manualBlit = 1;
            src->manualBlit = 1;
            d2_LockSurface(dst);
            d2_LockSurface(src);
            {
                for (i = 0; i < h; i++) {
                    len = (src->bpp >> 3) * w;
                    bs = (unsigned char *)src->bits + src->rowbytes * i + so;
                    bd = (unsigned char *)dst->bits + dst->rowbytes * i + d;
                    memcpy(bd, bs, len);
                }
            }
        }
    }
    if (dstLocked != 0)
        d2_LockSurface(dst);
    else
        d2_UnlockSurface(dst);
    if (srcLocked != 0) {
        d2_LockSurface(src);
        return 1;
    }
    d2_UnlockSurface(src);
    return 1;
}
/* 0x479480, d2_BlitPartMode without the colour key */

void d2_BlitPart(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst)
{
    d2_BlitPartMode(sx, sy, w, h, src, x, y, dst, 0);
}
/* 0x4794c0, blits the part (sx, sy, w, h) of the surface src to (x, y) of the DirectDraw surface dst @deep B3 20 code + 6 slots hunks (136/336 bytes), one hunk area: the RECT stores before the Blt. Original stores rs.left/rs.top (and rd.left/rd.top) early - mov [esp+0x10],esi; mov [esp+0x14],ebp - so sx/sy are dead when rs.right = sx+w / rs.bottom = sy+h are computed, and it adds them in place (add esi,edi); ours stores in the current order and recomputes with lea (lea edx,[esi+edi*1]); the 6 slot hunks are the RECT local homes ([esp+0x30]/0x34 vs 0x40). An earlier session brute-forced all 70 orders of the 8 rect stores, all identical, so store order is not the lever. Meaning read against 0x4794c0..0x47960e: the four negative-offset clips, the src bottom/right clamps, src->locked save + restore around the Blt, the DDSurface/manualBlit guard, rs/rd fill, Blt(dst, &rd, src->DDSurface, &rs, DDBLT_WAIT, 0) and the 0/1 returns - no meaning error. Readability done: Q1 named (blits a clipped part of a D2SURF onto a raw DirectDraw surface, the d2.c texture uploads call it), Q3 0x1000000 -> DDBLT_WAIT. */

int d2_BlitPartToDD(int sx, int sy, int w, int h, D2SURF *src, int x, int y, LPDIRECTDRAWSURFACE4 dst)
{
    RECT rs, rd;
    int locked;

    if (sy < 0) {
        h = h + sy;
        sy = 0;
    }
    if (sx < 0) {
        w = w + sx;
        sx = 0;
    }
    if (x < 0) {
        sx = sx - x;
        w = w + x;
        x = 0;
    }
    if (y < 0) {
        sy = sy - y;
        h = h + y;
        y = 0;
    }
    if (h + sy > src->bottom)
        h = src->bottom - sy;
    if (sx + w > src->right)
        w = src->right - sx;
    if (h != 0 && w != 0) {
        locked = src->locked;
        if (src->DDSurface != 0 && src->manualBlit == 0) {
            if (locked != 0)
                d2_UnlockSurface(src);
            rs.right = sx + w;
            rd.left = x;
            rd.right = w + x;
            rs.bottom = sy + h;
            rd.bottom = h + y;
            rs.left = sx;
            rs.top = sy;
            rd.top = y;
            if (dst->lpVtbl->Blt(dst, &rd, src->DDSurface, &rs, DDBLT_WAIT, 0) != 0)
                return 0;
        }
        if (locked != 0) {
            d2_LockSurface(src);
            return 1;
        }
        d2_UnlockSurface(src);
        return 1;
    }
    return 0;
}
/* 0x479610, blits the part (sx, sy, w, h) of the DirectDraw surface img to (x, y) of the surface surf, clipped to
   the surface edges; 1 on success, 0 when the clip leaves nothing or img is NULL (the reverse of
   d2_BlitPartToDD, the d2.c texture upload calls it) */

int d2_BlitPartFromDD(int sx, int sy, int w, int h, LPDIRECTDRAWSURFACE4 img, int x, int y, D2SURF *surf)
{
    RECT src, dst;

    if (sy < 0) {
        h += sy;
        sy = 0;
    }
    if (sx < 0) {
        w += sx;
        sx = 0;
    }
    if (x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if (y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if (h != 0 && w != 0) {
        if (img != 0) {
            src.left = sx;
            src.top = sy;
            dst.top = y;
            src.bottom = sy + h;
            src.right = sx + w;
            dst.left = x;
            dst.bottom = y + h;
            dst.right = x + w;
            surf->DDSurface->lpVtbl->Blt(surf->DDSurface, &dst, img, &src, DDBLT_WAIT, 0);
        }
        return 1;
    }
    return 0;
}
/* 0x4796c0, clears the pixels of surf to black: a memset for a system memory surface, a colour fill Blt for a
   DirectDraw one; returns 1 (0 only for a NULL surf) */

int d2_ClearSurface(D2SURF *surf)
{
    int locked;
    DDBLTFX fx;

    if (surf == 0)
        return 0;
    locked = surf->locked;
    if (surf->DDSurface == 0) {
        if (locked != 0)
            d2_LockSurface(surf);
        memset(surf->bits, 0, surf->rowbytes * surf->h);
    } else {
        if (locked != 0)
            d2_UnlockSurface(surf);
        memset(&fx, 0, sizeof(fx));
        fx.dwFillColor = 0;
        fx.dwSize = 100;
        surf->DDSurface->lpVtbl->Blt(surf->DDSurface, 0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
    }
    if (locked != 0) {
        d2_LockSurface(surf);
        return 1;
    }
    d2_UnlockSurface(surf);
    return 1;
}
/* 0x479780, makes a copy of a surface: the same description and make flags, the whole source image blitted over;
   returns the new surface */

D2SURF *d2_CopySurface(D2SURF *surf)
{
    D2SURF desc;
    D2SURF *ns;

    desc = *surf;
    desc.DDSurface = 0;
    desc.bits = 0;
    ns = d2_MakeSurface(&desc, surf->makeFlags);
    d2_BlitPart(0, 0, ns->w, ns->h, surf, 0, 0, ns);
    return ns;
}
/* 0x4797d0, clears the rectangle (x, y, w, h) of surf to black, clipped to the surface's width / height: a colour
   fill Blt for a DirectDraw surface, while a system memory surface is cleared whole (the rectangle is ignored);
   returns 1 (0 only for a NULL surf) */

int d2_ClearRect(int x, int y, int w, int h, D2SURF *surf)
{
    int locked;
    int l;
    RECT r;
    DDBLTFX fx;

    if (surf == 0)
        return 0;
    locked = surf->locked;
    if (locked != 0)
        d2_UnlockSurface(surf);
    if (surf->DDSurface == 0) {
        memset(surf->bits, 0, surf->rowbytes * surf->h);
    } else {
        if (x < surf->left)
            x = surf->left;
        r.left = x;
        l = x;
        r.top = y;
        r.right = l + w;
        r.bottom = y + h;
        if (r.right > surf->w - 1)
            r.right = surf->w;
        if (r.bottom > surf->h - 1)
            r.bottom = surf->h;
        if (l < surf->w && y < surf->h) {
            memset(&fx, 0, sizeof(fx));
            fx.dwFillColor = 0;
            fx.dwSize = 100;
            surf->DDSurface->lpVtbl->Blt(surf->DDSurface, &r, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
        }
    }
    if (locked != 0) {
        d2_LockSurface(surf);
        return 1;
    }
    d2_UnlockSurface(surf);
    return 1;
}
/* 0x4798f0, reads the colour of the pixel (x, y) of surf into rgb[0..2] (r, g, b): a 15 / 16 bpp pixel through
   dd_SplitRGB, the 3 bytes for 24 bpp, one dword for 32 bpp (r / g / b from the dword's bytes 0 / 4 / 8, the
   original's odd offsets); any other depth leaves rgb untouched by the switch (it gets the caller's stack bytes)
   @deep B3 far draft: 99 code hunks, 77 of 89 instructions / 51 of 240 bytes differ, all one register permutation plus bits-load scheduling. Read against 0x4798f0..0x4799df (jump table 0x4799b0, byte index table 0x4799c0, bpp 15/16 -> dd_SplitRGB, 24 -> 3 bytes, 32 -> dword bytes 0/4/8, default skips to the rgb store): same algorithm, same call, same stores, no meaning error. The original keeps surf in eax, bpp in edx, bytes (bpp>>3) in ecx; ours surf in ecx, bpp in eax, bytes in edx, and loads surf->bits before the imul where the original loads it after; with --regs ecx=eax,edx=ecx,eax=edx the remaining diffs are that bits-load / x-in-register scheduling in the 15/16 and 0x18 arms. spell.py: reload at 0x4798f0 (drop the local / re-read at the use) tried as (a) a local i = surf->pitch*y+x in every arm, as the original's edx/esi pattern suggested, and (b) all index expressions inlined with the i local dropped - both identical (51/240, 77 lines), reverted; dup-tail 0x47995f is the shared rgb[0..2] store epilogue, already one statement list after the switch, nothing to pull or duplicate. No source lever left for the parameter / bytes register roles -> register allocation, deep's toolbox. */

void d2_GetPixelColour(int x, int y, D2SURF *surf, unsigned char *rgb)
{
    unsigned char r, g, b;
    unsigned char *p;
    int i;
    unsigned int bytes = surf->bpp >> 3;

    switch (surf->bpp) {
    case 0xf:
    case 0x10:
        dd_SplitRGB(((unsigned short *)surf->bits)[surf->pitch * y + x], &r, &g, &b);
        break;
    case 0x18:
        p = (unsigned char *)surf->bits + (surf->pitch * y + x) * bytes;
        r = p[0];
        g = p[1];
        b = p[2];
        break;
    case 0x20:
        i = surf->pitch * y + x;
        r = ((unsigned char *)surf->bits)[i * 4];
        g = ((unsigned char *)surf->bits)[i * 4 + 4];
        b = ((unsigned char *)surf->bits)[i * 4 + 8];
        break;
    }
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
/* 0x4799e0, plots the pixel (x, y) of surf in the colour (r, g, b) at the surface's depth (8 bpp: the grey
   average; 15 / 16 bpp: dd_RGB; 24 bpp: 3 bytes; 32 bpp: a dword), clipped to the surface's clip rectangle;
   1 if plotted, 0 when outside the clip or surf is NULL (the pen d2_Plot and the outline below use it) */

int d2_PutPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b, D2SURF *surf)
{
    unsigned char bpp;
    DWORD c;

    if (surf != 0 && x >= surf->left && x < surf->right && y >= surf->top && y < surf->bottom) {
        bpp = surf->bpp >> 3;
        c = dd_RGB((unsigned char)r, (unsigned char)g, (unsigned char)b);
        switch (surf->bpp) {
        case 8:
            ((unsigned char *)surf->bits)[surf->pitch * y + x] =
                (unsigned char)((int)((r & 0xff) + (g & 0xff) + (b & 0xff)) / 3);
            break;
        case 0xf:
        case 0x10:
            ((unsigned short *)surf->bits)[surf->pitch * y + x] = (unsigned short)c;
            break;
        case 0x18:
            x = bpp * x;
            ((unsigned char *)surf->bits)[surf->pitch * y + x] = (unsigned char)r;
            ((unsigned char *)surf->bits)[surf->pitch * y + x + 1] = (unsigned char)g;
            ((unsigned char *)surf->bits)[surf->pitch * y + x + 2] = (unsigned char)b;
            break;
        case 0x20:
            ((DWORD *)surf->bits)[surf->pitch * y + x] = c;
        }
        return 1;
    }
    return 0;
}
/* 0x479b60, plots a 16 bit pixel */

int d2_PutPixel(int x, int y, unsigned short color, D2SURF *surf)
{
    ((unsigned short *)surf->bits)[surf->pitch * y + x] = color;
    return 1;
}
/* 0x479b90, the same, clipped */

int d2_PutPixelClipped(int x, int y, unsigned short color, D2SURF *surf)
{
    if (x >= 0 && x < surf->w && y >= 0 && y < surf->h && x >= surf->left && x < surf->right && y >= surf->top
        && y < surf->bottom) {
        ((unsigned short *)surf->bits)[surf->pitch * y + x] = color;
        return 1;
    }
    return 0;
}
/* 0x479bf0, draws the outline of the w x h rectangle at (x, y) in the colour (r, g, b): the left / right columns,
   then the top / bottom rows, pixel by pixel through d2_PutPixelRGB; nothing at all when the blue byte is 0 or
   h < 1 (the d2.c window border with 0xff, 0xff, 0xff comes through here)
   @deep B3 close: 16 differing instructions of 92 (12 of 224 bytes), one register pair plus the stack slot it frees. Original: surf=esi, r=edi, g=ebp, and r's dead slot [esp+0x24] takes x-1+w (lea ecx,[edx+ecx-1]); ours: surf=esi, g=edi, r=ebp, x-1+w goes to g's slot [esp+0x28]. Two instructions at 0x479c11 / 0x479c15 (mov edi / mov ebp from r and g) swap the roles; everything else follows (the push pairs push ebp,edi vs edi,ebp at 0x479c3f, 0x479c52, 0x479c98, 0x479cab and the [esp+0x3c]/[esp+0x40] reload). Meaning read against 0x479bf0..0x479cd0: b loaded as a byte and tested < 1 first, h >= 1, then the h loop (two d2_PutPixelRGB calls per row) and the w loop (two per column), the x-1+w hoist and the h/w entry tests - same statements, same order, no meaning error. spell.py 0x479c11 / 0x479c98 dup-tail: no if/else arm to duplicate or pull (the body is two plain loops) and the 2-instruction 'tail' is exactly the r/g load pair; the branch-polarity form tried - early return 'if ((unsigned char)b < 1 || h < 1) return;' - compiles identical (12/224, 16 lines), reverted; the old note's param types / decl order / nested ifs / j removed stay without effect. The edi/ebp choice for two equally used unsigned int parameters is the allocator's tie-break -> deep (variant rounds). */

void d2_DrawRectOutline(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf)
{
    int i, j;

    if ((unsigned char)b >= 1 && h >= 1) {
        for (i = 0; i < h; i++) {
            d2_PutPixelRGB(x, y + i, r, g, b, surf);
            d2_PutPixelRGB(x - 1 + w, y + i, r, g, b, surf);
        }
        for (i = 0; i < w; i++) {
            d2_PutPixelRGB(x + i, y, r, g, b, surf);
            d2_PutPixelRGB(x + i, y - 1 + h, r, g, b, surf);
        }
    }
}
/* 0x479cd0, sets the clip rectangle of surf to the w x h rectangle at (x, y): left / top / right / bottom (the
   same rectangle d2_SetClipRect sets on the screen globals); a NULL surf is ignored */

void d2_SetSurfaceClip(int x, int y, int w, int h, D2SURF *surf)
{
    if (surf != 0) {
        surf->left = x;
        surf->top = y;
        surf->right = x + w;
        surf->bottom = y + h;
    }
}

int d2_Unk6ed990 = 0;  /* 0x6ed990 */
int d2_DragStartValue = 0;  /* 0x6ed9c4 */
void * d2_PointerPic = 0;  /* 0x6ed9cc, "_MOUSE_CURSOR" data */
int d2_Unk6ed9e4 = 0;  /* 0x6ed9e4 */
int d2_DragStartX = 0;  /* 0x6ed9e8 */
int d2_DragStartY = 0;  /* 0x6ed9ec */
int d2_ClickUsed = 0;  /* 0x6ed9f0 */
int d2_DragActive = 0;  /* 0x6ed9f4 */
short d2_Unkf8a650 = 0;  /* 0xf8a650 */
short d2_Unkf8a652 = 0;  /* 0xf8a652 */
int d2_Unkf8a654 = 0;  /* 0xf8a654 */
int d2_FocusObjIndex = 0;  /* 0xf8a658 */
int d2_Unkf8a65c = 0;  /* 0xf8a65c */
int d2_InputDragOffsetY = 0;  /* 0xf8a660 */
int d2_SavedMaxY = 0;  /* 0xf8a664 */
int d2_SavedMinY = 0;  /* 0xf8a66c */
D2OBJ * d2_ClickedObj = 0;  /* 0xf8a670 */
D2HIT d2_HitRects[0x40] = {0};  /* 0xf8a680 (d2_AddHitRect allows i = 0x40, one past: the original's latent overrun) */
int d2_Unkf8aa80 = 0;  /* 0xf8aa80 */
int d2_HitCount = 0;  /* 0xf8aa84 */
D2SLOT d2_ItemListSurf256 = {0};  /* 0xf8aa90 */
int d2_Unkf8aa9c = 0;  /* 0xf8aa9c */
D2SLOT d2_ItemListSurf128[3] = {0};  /* 0xf8aaa0 */
int d2_ClickedForm = 0;  /* 0xf8aac4 */
int d2_RightHeldObj = 0;  /* 0xf8aac8 */
int d2_Unkf8aacc = 0;  /* 0xf8aacc */
int d2_TextCursorX = 0;  /* 0xf8aad4 */
int d2_TextCursorY = 0;  /* 0xf8aad8 */
void * d2_ItemListSurf64 = 0;  /* 0xf8aadc */
int d2_InputDragValue = 0;  /* 0xf8ab60 */
int d2_Unkf8ab64 = 0;  /* 0xf8ab64 */
int d2_InputDragReleased = 0;  /* 0xf8ab68 */
int d2_PointerSprite = -1;  /* 0x5febc8, sprite of the pointer, -1: none */
int d2_TextCursorOffset = -7;  /* 0x5febcc, -7 or 0 */
int d2_HitClicked = -1;  /* 0x5febd0 */
int d2_HitUnderPointer = -1;  /* 0x5febd4 */
int d2_Unk5febdc = -1;  /* 0x5febdc */
int d2_Unk5febe0 = -1;  /* 0x5febe0 */
int d2_LimitsChanged = -1;  /* 0x5febe4 */
int d2_TextCursorSprite = -1;  /* 0x5febe8, sprite of the text cursor, -1: none */
int d2_Unk5febec = -1;  /* 0x5febec */
